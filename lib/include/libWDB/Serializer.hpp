/*
 * @author amber
 * @date 02/09/2023
 */

#pragma once

#include <cstdio>

#include "libWDB/WorldDatabase.hpp"

namespace libWDB
{
	auto Save(
		const WorldDatabase& database,
		FILE* fileptr
	) -> void;
} // namespace libWDB