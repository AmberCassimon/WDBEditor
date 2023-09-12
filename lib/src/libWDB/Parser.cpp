/*
 * @author amber
 * @date 02/09/2023
 */

#include "libWDB/Parser.hpp"

#include <cstdint>
#include <cstdio>

#include "libWDB/Util.hpp"
#include "libWDB/WDBParseException.hpp"

namespace libWDB
{
	namespace __detail
	{
		auto ParseSubItem(
			unsigned char** byte_ptr,
			const unsigned char* end,
			BinaryTreeNode<WorldDatabaseNode>* subgroup_node,
			bool read_presenter_data
		) -> void
		{
			// We're at the end of the buffer
			if (end <= (*byte_ptr))
			{
				return;
			}

			const std::uint32_t subitem_title_length = Uint32FromLEBytes(byte_ptr);
			const std::string subitem_title = ASCIIStringFromLEBytes(byte_ptr, subitem_title_length, true);
			const std::uint32_t subitem_size = Uint32FromLEBytes(byte_ptr);
			const std::uint32_t subitem_offset = Uint32FromLEBytes(byte_ptr);
			std::optional<SubItemPresenterData> extra_data {std::nullopt};

			if (read_presenter_data)
			{
				const std::uint32_t presenter_title_length = Uint32FromLEBytes(byte_ptr);
				const std::string presenter_title = ASCIIStringFromLEBytes(byte_ptr, presenter_title_length, true);

				const std::vector<unsigned char> unknown_bytevec = ByteArrayFromLEBytes(byte_ptr, 37);
				std::array<unsigned char, 37> unknown_bytes {};
				std::copy(unknown_bytevec.begin(), unknown_bytevec.end(), unknown_bytes.begin());

				extra_data = SubItemPresenterData {presenter_title, unknown_bytes};
			}

			const SubItem subitem {subitem_title, subitem_size, subitem_offset, extra_data};

			subgroup_node->AddChild(WorldDatabaseNode {subitem});
		}

		auto ParseSubItems(
			unsigned char** byte_ptr,
			const unsigned char* end,
			BinaryTreeNode<WorldDatabaseNode>* subgroup_node,
			bool read_presenter_data
		) -> void
		{
			// We're at the end of the buffer
			if (end <= (*byte_ptr))
			{
				return;
			}

			const std::uint32_t num_sub_items = Uint32FromLEBytes(byte_ptr);

			for (std::uint32_t i = 0; i < num_sub_items; ++i)
			{
				ParseSubItem(byte_ptr, end, subgroup_node, read_presenter_data);
			}
		}

		auto ParseSubGroup(
			unsigned char** byte_ptr,
			const unsigned char* end,
			BinaryTreeNode<WorldDatabaseNode>* group_node,
			bool read_presenter_data
		) -> void
		{
			// We're at the end of the buffer
			if (end <= (*byte_ptr))
			{
				return;
			}

			BinaryTreeNode<WorldDatabaseNode>* subgroup_node = group_node->AddChild(WorldDatabaseNode {SubGroup {}});
			ParseSubItems(byte_ptr, end, subgroup_node, read_presenter_data);
		}

		auto ParseSubGroups(
			unsigned char** byte_ptr, const unsigned char* end, BinaryTreeNode<WorldDatabaseNode>* group_node
		) -> void
		{
			// We're at the end of the buffer
			if (end <= (*byte_ptr))
			{
				return;
			}

			// 2 Subgroups per group
			for (std::uint32_t i = 0; i < 2; ++i)
			{
				ParseSubGroup(byte_ptr, end, group_node, 1 == i);
			}
		}

		auto ParseGroup(unsigned char** byte_ptr, const unsigned char* end) -> std::optional<WorldDatabaseNode>
		{
			// We're at the end of the buffer
			if (end <= (*byte_ptr))
			{
				return std::nullopt;
			}

			const std::uint32_t group_title_length = Uint32FromLEBytes(byte_ptr);
			const std::string group_title = ASCIIStringFromLEBytes(byte_ptr, group_title_length, true);

			BinaryTreeNode<WorldDatabaseNode>* bt_node = wdb.AddGroup(Group {group_title});
			ParseSubGroups(byte_ptr, end, bt_node);
		}

		auto ParseGroups(unsigned char** byte_ptr, const unsigned char* end) -> std::optional<BinaryTreeNode<WorldDatabaseNode>*>
		{
			// We're at the end of the buffer
			if (end <= (*byte_ptr))
			{
				return std::nullopt;
			}

			const std::uint32_t num_groups = Uint32FromLEBytes(byte_ptr);

			if (0 == num_groups)
			{
				return std::nullopt;
			}

			std::optional<WorldDatabaseNode> first_group_data = ParseGroup(byte_ptr, end);

			if (!first_group_data.has_value())
			{
				return std::nullopt;
			}

			BinaryTreeNode<WorldDatabaseNode>* first_group_node = new BinaryTreeNode<WorldDatabaseNode>(std::move(first_group_data.value()));

			for (std::uint32_t i = 1; i < num_groups; ++i)
			{
				std::optional<WorldDatabaseNode> wdbn_opt= ParseGroup(byte_ptr, end);

				// Return empty if we failed to parse a group
				// TODO: Error reporting!
				if (!wdbn_opt.has_value())
				{
					return std::nullopt;
				}

				first_group_node->AddSibling(std::move(wdbn_opt.value()));
			}

			return std::make_optional<BinaryTreeNode<WorldDatabaseNode>*>(first_group_node);
		}

		auto ParseImage(unsigned char** byte_ptr, const unsigned char* end, GIFImage& image) -> void
		{
			// We're at the end of the buffer
			if (end <= (*byte_ptr))
			{
				return;
			}

			image.color_index.reserve(static_cast<std::size_t>(image.height * image.width));

			for (std::uint32_t row = 0; row < image.height; ++row)
			{
				for (std::uint32_t col = 0; col < image.width; ++col)
				{
					image.color_index.push_back(Uint8FromLEBytes(byte_ptr));
				}
			}
		}

		auto ParseColorPalette(unsigned char** byte_ptr, const unsigned char* end, GIFImage& image) -> void
		{
			// We're at the end of the buffer
			if (end <= (*byte_ptr))
			{
				return;
			}

			const std::uint32_t num_colors = Uint32FromLEBytes(byte_ptr);

			for (std::uint32_t color_index = 0; color_index < num_colors; ++color_index)
			{
				image.colors.emplace_back(Color {
					Uint8FromLEBytes(byte_ptr), // Red
					Uint8FromLEBytes(byte_ptr), // Green
					Uint8FromLEBytes(byte_ptr)	// Blue
				});
			}
		}

		auto ParseGIFImage(unsigned char** byte_ptr, const unsigned char* end) -> std::optional<GIFImage>
		{
			// We're at the end of the buffer
			if (end <= (*byte_ptr))
			{
				return std::nullopt;
			}

			// GIFs don't have a NULL terminator, so we add 1 to the length.
			const std::uint32_t gif_name_length = Uint32FromLEBytes(byte_ptr);
			const std::string gif_name = ASCIIStringFromLEBytes(byte_ptr, gif_name_length, false);

			const std::uint32_t width = Uint32FromLEBytes(byte_ptr);
			const std::uint32_t height = Uint32FromLEBytes(byte_ptr);

			GIFImage image {gif_name, width, height, {}, {}};

			ParseColorPalette(byte_ptr, end, image);
			ParseImage(byte_ptr, end, image);

			return std::make_optional<GIFImage>(image);
		}

		auto ParseGIFChunk(unsigned char** byte_ptr, const unsigned char* end) -> std::optional<GIFChunk>
		{
			// We're at the end of the buffer
			if (end <= (*byte_ptr))
			{
				return std::nullopt;
			}

			GIFChunk chunk {};

			const std::uint32_t num_images = Uint32FromLEBytes(byte_ptr);
			chunk.images.reserve(num_images);

			for (std::uint32_t i = 0; i < num_images; ++i)
			{
				std::optional<GIFImage> opt_img = ParseGIFImage(byte_ptr, end);

				// Return empty if we failed to parse an image
				// TODO: Error reporting!
				if (!opt_img.has_value())
				{
					return std::nullopt;
				}

				chunk.images.push_back(opt_img.value());
			}

			return std::make_optional<GIFChunk>(chunk);
		}

		auto ParseLooseGIFChunk(unsigned char** byte_ptr, const unsigned char* end) -> std::optional<GIFChunk>
		{
			// We're at the end of the buffer
			if (end <= (*byte_ptr))
			{
				return std::nullopt;
			}

			const std::uint32_t gif_chunk_bytes = Uint32FromLEBytes(byte_ptr);
			const unsigned char* gif_chunk_end = end + gif_chunk_bytes;

			return ParseGIFChunk(byte_ptr, gif_chunk_end);
		}
	} // namespace __detail

	auto ParseWDB(FILE* fileptr) -> ParseResult
	{
		const long filesize = Filesize(fileptr);

		std::vector<unsigned char> data_buffer(filesize, 0);

#if defined(__STDC_WANT_LIB_EXT1__)
		const std::size_t bytes_read =
			fread_s(data_buffer.data(), data_buffer.size(), sizeof(unsigned char), data_buffer.size(), fileptr);
#else
		const std::size_t bytes_read = fread(data_buffer.data(), sizeof(unsigned char), data_buffer.size(), fileptr);
#endif

		if (filesize != bytes_read)
		{
			throw WDBParseException("Failed to read entire .wdb file!");
		}

		unsigned char* buffer_start = data_buffer.data();
		const unsigned char* buffer_end = buffer_start + data_buffer.size();

		unsigned char* in_progress_ptr = buffer_start;

		ParseResult result {
			__detail::ParseGroups(&in_progress_ptr, buffer_end),
			__detail::ParseLooseGIFChunk(&in_progress_ptr, buffer_end)
		};

		return result;
	}
} // namespace libWDB