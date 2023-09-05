/*
 * @author amber
 * @date 30/08/2023
 */

#pragma once

#include <optional>

#include "libWDB/BinaryTreeNode.hpp"
#include "libWDB/GIFChunk.hpp"
#include "libWDB/Group.hpp"
#include "libWDB/WorldDatabaseNode.hpp"

namespace libWDB
{
	// At least partially based on:
	// http://fileformats.archiveteam.org/wiki/Lego_Island_World_Database
	// WDBRipper has also been a great help:
	// https://github.com/LiamBrandt/WDBRipper/
	class WorldDatabase
	{
		public:
			WorldDatabase() = default;

			WorldDatabase(const WorldDatabase&) = delete;

			WorldDatabase(WorldDatabase&& wdb) noexcept;

			auto operator= (const WorldDatabase&) -> WorldDatabase& = delete;

			auto operator= (WorldDatabase&& wdb) noexcept -> WorldDatabase&;

			~WorldDatabase();

			auto AddGroup(Group&& group) -> BinaryTreeNode<WorldDatabaseNode>*;

			[[nodiscard]] auto Groups() const -> std::vector<const BinaryTreeNode<WorldDatabaseNode>*>;

			[[nodiscard]] auto LooseGIFChunk() const -> std::optional<std::reference_wrapper<const GIFChunk>>;

			[[nodiscard]] auto LooseGIFChunk() -> std::optional<std::reference_wrapper<GIFChunk>>;

			auto SetLooseGIFChunk(GIFChunk&& new_gif_chunk) -> void;

		private:
			std::optional<BinaryTreeNode<WorldDatabaseNode>*> first_group {std::nullopt};

			std::optional<GIFChunk> loose_gif_chunk {std::nullopt};
	};
} // namespace libWDB