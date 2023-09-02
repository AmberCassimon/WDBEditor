/*
 * @author amber
 * @date 31/08/2023
 */

#pragma once

#include <array>
#include <string>

namespace libWDB
{
	struct SubItemPresenterData
	{
			std::string presenter_title;
			std::array<unsigned char, 37> unknown;
	};
} // namespace WDBEditor