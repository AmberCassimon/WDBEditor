/*
 * @author amber
 * @date 02/09/2023
 */

#pragma once

#include <array>
#include <vector>

#include "libWDB/Group.hpp"
#include "libWDB/ParseResult.hpp"
#include "libWDB/SubGroup.hpp"
#include "libWDB/SubItem.hpp"
#include "libWDB/WorldDatabase.hpp"

namespace libWDB
{
	namespace __detail
	{
		// Hierarchical Structure
		auto ParseSubItem(unsigned char** byte_ptr, const unsigned char* end, BinaryTreeNode<WorldDatabaseNode>* subgroup_node, bool read_presenter_data) -> void;

		auto ParseSubItems(unsigned char** byte_ptr, const unsigned char* end, BinaryTreeNode<WorldDatabaseNode>* subgroup_node, bool read_presenter_data) -> void;

		auto ParseSubGroup(unsigned char** byte_ptr, const unsigned char* end, BinaryTreeNode<WorldDatabaseNode>* group_node, bool read_presenter_data) -> void;

		auto ParseSubGroups(unsigned char** byte_ptr, const unsigned char* end, BinaryTreeNode<WorldDatabaseNode>* group_node) -> void;

		auto ParseGroup(unsigned char** current, const unsigned char* end) -> std::optional<BinaryTreeNode<WorldDatabaseNode>*>;

		auto ParseGroups(unsigned char** byte_ptr, const unsigned char* end) -> std::optional<BinaryTreeNode<WorldDatabaseNode>*>;

		// GIF Chunks
		auto ParseColorPalette(unsigned char** byte_ptr, const unsigned char* end, GIFImage& image) -> void;

		auto ParseGIFImage(unsigned char** byte_ptr, const unsigned char* end) -> std::optional<GIFImage>;

		auto ParseGIFChunk(unsigned char** byte_ptr, const unsigned char* end) -> std::optional<GIFChunk>;

		auto ParseLooseGIFChunk(unsigned char** byte_ptr, const unsigned char* end) -> std::optional<GIFChunk>;
	} // namespace __detail

	auto ParseWDB(FILE* fileptr) -> ParseResult;
} // namespace libWDB