/*
 * @author amber
 * @date 02/09/2023
 */

#include "libWDB/Parser.hpp"

#include "libWDB/Util.hpp"
#include "libWDB/WDBParseException.hpp"

namespace libWDB
{
	namespace __detail
	{
		auto ParseSubItem(unsigned char** byte_ptr, const unsigned char* end, BinaryTreeNode<WorldDatabaseNode>* subgroup_node, bool read_presenter_data) -> void
		{
			// We're at the end of the buffer
			if (end <= (*byte_ptr))
			{
				return;
			}

			const std::uint32_t subitem_title_length = Uint32FromLEBytes(byte_ptr);
			const std::string subitem_title = ASCIIStringFromLEBytes(byte_ptr, subitem_title_length);
			const std::uint32_t subitem_size = Uint32FromLEBytes(byte_ptr);
			const std::uint32_t subitem_offset = Uint32FromLEBytes(byte_ptr);
			std::optional<SubItemPresenterData> extra_data {std::nullopt};

			if (read_presenter_data)
			{
				const std::uint32_t presenter_title_length = Uint32FromLEBytes(byte_ptr);
				const std::string presenter_title = ASCIIStringFromLEBytes(byte_ptr, presenter_title_length);

				const std::vector<unsigned char> unknown_bytevec = ByteArrayFromLEBytes(byte_ptr, 37);
				std::array<unsigned char, 37> unknown_bytes {};
				std::copy(unknown_bytevec.begin(), unknown_bytevec.end(), unknown_bytes.begin());

				extra_data = SubItemPresenterData {presenter_title, unknown_bytes};
			}

			const SubItem subitem {subitem_title, subitem_size, subitem_offset, extra_data};

			subgroup_node->AddChild(WorldDatabaseNode{subitem});
		}

		auto ParseSubItems(unsigned char** byte_ptr, const unsigned char* end, BinaryTreeNode<WorldDatabaseNode>* subgroup_node, bool read_presenter_data) -> void
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

		auto ParseSubGroup(unsigned char** byte_ptr, const unsigned char* end, BinaryTreeNode<WorldDatabaseNode>* group_node, bool read_presenter_data) -> void
		{
			// We're at the end of the buffer
			if (end <= (*byte_ptr))
			{
				return;
			}

			BinaryTreeNode<WorldDatabaseNode>* subgroup_node = group_node->AddChild(WorldDatabaseNode{SubGroup{}});
			ParseSubItems(byte_ptr, end, subgroup_node, read_presenter_data);
		}

		auto ParseSubGroups(unsigned char** byte_ptr, const unsigned char* end, BinaryTreeNode<WorldDatabaseNode>* group_node) -> void
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

		auto ParseGroup(unsigned char** byte_ptr, const unsigned char* end, WorldDatabase& wdb) -> void
		{
			// We're at the end of the buffer
			if (end <= (*byte_ptr))
			{
				return;
			}

			const std::uint32_t group_title_length = Uint32FromLEBytes(byte_ptr);
			const std::string group_title = ASCIIStringFromLEBytes(byte_ptr, group_title_length);

			BinaryTreeNode<WorldDatabaseNode>* bt_node = wdb.AddGroup(Group{group_title});
			ParseSubGroups(byte_ptr, end, bt_node);
		}

		auto ParseGroups(unsigned char** byte_ptr, const unsigned char* end, WorldDatabase& wdb) -> void
		{
			// We're at the end of the buffer
			if (end <= (*byte_ptr))
			{
				return;
			}

			const std::uint32_t num_groups = Uint32FromLEBytes(byte_ptr);

			for (std::uint32_t i = 0; i < num_groups; ++i)
			{
				ParseGroup(byte_ptr, end, wdb);
			}
		}
	} // namespace __detail

	auto ParseWDB(FILE* fileptr) -> WorldDatabase
	{
		const long filesize = Filesize(fileptr);

		std::vector<unsigned char> data_buffer(filesize, 0);

		const std::size_t bytes_read =
			fread_s(data_buffer.data(), data_buffer.size(), sizeof(unsigned char), data_buffer.size(), fileptr);

		if (filesize != bytes_read)
		{
			throw WDBParseException("Failed to read entire .wdb file!");
		}

		unsigned char* buffer_start = data_buffer.data();
		const unsigned char* buffer_end = buffer_start + data_buffer.size();

		WorldDatabase wdb;

		__detail::ParseGroups(&buffer_start, buffer_end, wdb);

		return wdb;
	}
} // namespace libWDB