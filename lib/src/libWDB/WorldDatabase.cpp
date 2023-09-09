/*
 * @author amber
 * @date 30/08/2023
 */

#include "libWDB/WorldDatabase.hpp"

#include <cassert>

namespace libWDB
{
	WorldDatabase::WorldDatabase(WorldDatabase&& wdb) noexcept: first_group(wdb.first_group), loose_gif_chunk(wdb.loose_gif_chunk)
	{
		wdb.first_group = std::nullopt;
		wdb.loose_gif_chunk = std::nullopt;
	}

	auto WorldDatabase::operator=(WorldDatabase&& wdb) noexcept -> WorldDatabase&
	{
		this->first_group = wdb.first_group;
		this->loose_gif_chunk = wdb.loose_gif_chunk;

		wdb.first_group = std::nullopt;
		wdb.loose_gif_chunk = std::nullopt;

		return *this;
	}

	WorldDatabase::~WorldDatabase()
	{
		if (this->first_group.has_value())
		{
			delete this->first_group.value();
		}
	}

	auto WorldDatabase::AddGroup(Group&& group) -> BinaryTreeNode<WorldDatabaseNode>*
	{
		WorldDatabaseNode wdb_node {group};

		if (!this->first_group.has_value())
		{
			BinaryTreeNode<WorldDatabaseNode>* bt_node = new BinaryTreeNode<WorldDatabaseNode>(std::move(wdb_node));

			this->first_group = std::make_optional<BinaryTreeNode<WorldDatabaseNode>*>(bt_node);

			return bt_node;
		}

		return this->first_group.value()->AddSibling(std::move(wdb_node));
	}

	auto WorldDatabase::Groups() const -> std::vector<const BinaryTreeNode<WorldDatabaseNode>*>
	{
		if (!this->first_group.has_value())
		{
			return std::vector<const BinaryTreeNode<WorldDatabaseNode>*> {};
		}

		std::vector<const BinaryTreeNode<WorldDatabaseNode>*> groups;

		assert(NodeType::Group == this->first_group.value()->Data().Type());
		groups.push_back(this->first_group.value());

		for (const BinaryTreeNode<WorldDatabaseNode>* bt_node : this->first_group.value()->Siblings())
		{
			assert(NodeType::Group == bt_node->Data().Type());
			groups.push_back(bt_node);
		}

		return groups;
	}

	auto WorldDatabase::LooseGIFChunk() const -> std::optional<std::reference_wrapper<const GIFChunk>>
	{
		if (!this->loose_gif_chunk.has_value())
		{
			return std::nullopt;
		}

		return std::make_optional<std::reference_wrapper<const GIFChunk>>(this->loose_gif_chunk.value());
	}

	auto WorldDatabase::LooseGIFChunk() -> std::optional<std::reference_wrapper<GIFChunk>>
	{
		if (!this->loose_gif_chunk.has_value())
		{
			return std::nullopt;
		}

		return std::make_optional<std::reference_wrapper<GIFChunk>>(this->loose_gif_chunk.value());
	}

	auto WorldDatabase::SetLooseGIFChunk(GIFChunk&& new_gif_chunk) -> void {
		this->loose_gif_chunk = std::make_optional<GIFChunk>(new_gif_chunk);
	}
} // namespace libWDB