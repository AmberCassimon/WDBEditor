/*
 * @author amber
 * @date 31/08/2023
 */

#pragma once

#include <optional>
#include <string>

#include "libWDB/SubItemPresenterData.hpp"

namespace libWDB
{
	struct SubItem
	{
			std::string title;
			std::uint32_t size;
			std::uint32_t offset;
			std::optional<SubItemPresenterData> extra_data;
	};
} // namespace WDBEditor