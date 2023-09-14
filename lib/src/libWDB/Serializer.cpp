/*
 * @author amber
 * @date 02/09/2023
 */

#include "libWDB/Serializer.hpp"

#include <cassert>
#include <cstdint>

#include "libWDB/Util.hpp"

namespace libWDB
{
	namespace __detail
	{
		auto SaveSubItemPresenterData(
			const SubItemPresenterData& presenter_data,
			FILE* fileptr
		) -> void
		{
			// Title size + string
			// Add 1 for NULL terminator
			Uint32ToLEBytes(static_cast<std::uint32_t>(presenter_data.presenter_title.size()) + 1, fileptr);
			ASCIIStringToLEBytes(presenter_data.presenter_title, fileptr);

			std::vector<unsigned char> byte_vec;
			byte_vec.resize(presenter_data.unknown.size());

			std::copy(presenter_data.unknown.begin(), presenter_data.unknown.end(), byte_vec.begin());

			// 37 Unknown Bytes
			ByteArrayToLEBytes(byte_vec, fileptr);
		}

		auto SaveSubItem(
			const BinaryTreeNode<WorldDatabaseNode>* subitem_node,
			FILE* fileptr
		) -> void
		{
			assert(NodeType::SubItem == subitem_node->Data().Type());

			const SubItem subitem = subitem_node->Data().GetSubItem().value();

			// Title size + string
			// Add 1 for NULL terminator
			Uint32ToLEBytes(static_cast<std::uint32_t>(subitem.title.size()) + 1, fileptr);
			ASCIIStringToLEBytes(subitem.title, fileptr);

			// Size and offset of subitem
			Uint32ToLEBytes(subitem.size, fileptr);
			Uint32ToLEBytes(subitem.offset, fileptr);

			if (subitem.extra_data.has_value())
			{
				SaveSubItemPresenterData(subitem.extra_data.value(), fileptr);
			}
		}

		auto SaveSubItems(
			const BinaryTreeNode<WorldDatabaseNode>* subgroup_node,
			FILE* fileptr
		) -> void
		{
			assert(NodeType::SubGroup == subgroup_node->Data().Type());

			// Number of subitems
			Uint32ToLEBytes(static_cast<std::uint32_t>(subgroup_node->Children().size()), fileptr);

			for (const BinaryTreeNode<WorldDatabaseNode>* subitem_node: subgroup_node->Children())
			{
				SaveSubItem(subitem_node, fileptr);
			}
		}

		auto SaveSubGroup(
			const BinaryTreeNode<WorldDatabaseNode>* subgroup_node,
			FILE* fileptr
		) -> void
		{
			assert(NodeType::SubGroup == subgroup_node->Data().Type());

			SaveSubItems(subgroup_node, fileptr);
		}

		auto SaveSubGroups(
			const BinaryTreeNode<WorldDatabaseNode>* group_node,
			FILE* fileptr
		) -> void
		{
			assert(2 == group_node->Children().size());

			for (const BinaryTreeNode<WorldDatabaseNode>* subgroup_node: group_node->Children())
			{
				SaveSubGroup(
					subgroup_node,
					fileptr
				);
			}
		}

		auto SaveGroup(
			const BinaryTreeNode<WorldDatabaseNode>* group_node,
			FILE* fileptr
		) -> void
		{
			assert(NodeType::Group == group_node->Data().Type());

			const Group group = group_node->Data().GetGroup().value();

			// String length + string
			// Add 1 for NULL terminator
			Uint32ToLEBytes(static_cast<std::uint32_t>(group.title.size()) + 1, fileptr);
			ASCIIStringToLEBytes(group.title, fileptr);

			SaveSubGroups(group_node, fileptr);
		}

		auto SaveGroups(
			const WorldDatabase& database,
			FILE* fileptr
		) -> void
		{
			// Write the number of groups
			Uint32ToLEBytes(
				static_cast<std::uint32_t>(database.Groups().size()),
				fileptr
			);

			for (const BinaryTreeNode<WorldDatabaseNode>* bt_node: database.Groups())
			{
				SaveGroup(bt_node, fileptr);
			}
		}

		auto SaveLooseGIFChunk(
			const WorldDatabase& database,
			FILE* fileptr
		) -> void
		{
			// TODO: Serialize GIF Image
		}
	} // namespace __detail

	auto Save(
		const WorldDatabase& database,
		FILE* fileptr
	) -> void
	{
		__detail::SaveGroups(database, fileptr);
		__detail::SaveLooseGIFChunk(database, fileptr);
	}
} // namespace libWDB