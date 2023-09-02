/*
 * @author amber
 * @date 30/08/2023
 */

#pragma once

#include <string>

#include "libWDB/SubGroup.hpp"

namespace libWDB
{
	struct Group
	{
			std::string title;

			Group(const Group&) = default;

			Group(Group&&) noexcept = default;
	};
} // namespace libWDB