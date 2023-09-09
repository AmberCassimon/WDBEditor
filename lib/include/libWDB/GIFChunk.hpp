/*
 * @author amber
 * @date 05/09/2023
 */

#pragma once

#include <vector>

#include "libWDB/GIFImage.hpp"

namespace libWDB
{
	struct GIFChunk
	{
			std::vector<GIFImage> images;
	};
} // namespace libWDB