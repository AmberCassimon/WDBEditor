/*
 * @author amber
 * @date 12/09/2023
 */

#pragma once

#include <optional>

#include "libWDB/BinaryTreeNode.hpp"
#include "libWDB/GIFChunk.hpp"
#include "libWDB/WorldDatabase.hpp"

namespace libWDB
{
	struct ParseResult
	{
			std::optional<BinaryTreeNode<WorldDatabaseNode>*> first_group {std::nullopt};

			std::optional<GIFChunk> loose_gif_chunk {std::nullopt};
	};
} // namespace libWDB