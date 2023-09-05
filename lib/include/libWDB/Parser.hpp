/*
 * @author amber
 * @date 02/09/2023
 */

#pragma once

#include <array>
#include <vector>

#include "libWDB/Group.hpp"
#include "libWDB/SubGroup.hpp"
#include "libWDB/SubItem.hpp"
#include "libWDB/WorldDatabase.hpp"

namespace libWDB
{
	namespace __detail
	{
		auto ParseSubItem(unsigned char** byte_ptr, const unsigned char* end, BinaryTreeNode<WorldDatabaseNode>* subgroup_node, bool read_presenter_data) -> void;

		auto ParseSubItems(unsigned char** byte_ptr, const unsigned char* end, BinaryTreeNode<WorldDatabaseNode>* subgroup_node, bool read_presenter_data) -> void;

		auto ParseSubGroup(unsigned char** byte_ptr, const unsigned char* end, BinaryTreeNode<WorldDatabaseNode>* group_node, bool read_presenter_data) -> void;

		auto ParseSubGroups(unsigned char** byte_ptr, const unsigned char* end, BinaryTreeNode<WorldDatabaseNode>* group_node) -> void;

		auto ParseGroup(unsigned char** current, const unsigned char* end, WorldDatabase& wdb) -> void;

		auto ParseGroups(unsigned char** byte_ptr, const unsigned char* end, WorldDatabase& wdb) -> void;

		auto ParseLooseGIFChunk(unsigned char** byte_ptr, const unsigned char* end, WorldDatabase& wdb) -> void;
	} // namespace __detail

	auto ParseWDB(FILE* fileptr) -> WorldDatabase;
} // namespace libWDB