/*
 * @author amber
 * @date 09/09/2023
 */

#pragma once

#include <cstdint>

#include <string>
#include <vector>

#include "libWDB/Color.hpp"

namespace libWDB
{
	struct GIFImage
	{
			// Metadata
			std::string name;
			std::uint32_t width;
			std::uint32_t height;

			// Color Palette
			std::vector<Color> colors;

			// Actual image
			std::vector<std::uint8_t> color_index;
	};
} // namespace libWDB