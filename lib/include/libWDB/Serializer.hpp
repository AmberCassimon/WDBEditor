/*
 * @author amber
 * @date 02/09/2023
 */

#pragma once

#include <cstdio>

#include "libWDB/WorldDatabase.hpp"

namespace libWDB
{
	namespace __detail
	{
		auto SaveSubItemPresenterData(
			const SubItemPresenterData& presenter_data,
			FILE* fileptr
		) -> void;

		auto SaveSubItem(
			const BinaryTreeNode<WorldDatabaseNode>* subitem_node,
			FILE* fileptr
		) -> void;

		auto SaveSubItems(
			const BinaryTreeNode<WorldDatabaseNode>* subgroup_node,
			FILE* fileptr
		) -> void;

		auto SaveSubGroup(
			const BinaryTreeNode<WorldDatabaseNode>* subgroup_node,
			FILE* fileptr
		) -> void;

		auto SaveSubGroups(
			const BinaryTreeNode<WorldDatabaseNode>* group_node,
			FILE* fileptr
		) -> void;

		auto SaveGroup(
			const BinaryTreeNode<WorldDatabaseNode>* group_node,
			FILE* fileptr
		) -> void;

		auto SaveGroups(
			const WorldDatabase& database,
			FILE* fileptr
		) -> void;

		auto SaveLooseGIFChunk(
			const WorldDatabase& database,
			FILE* fileptr
		) -> void;
	} // namespace __detail

	auto Save(
		const WorldDatabase& database,
		FILE* fileptr
	) -> void;
} // namespace libWDB