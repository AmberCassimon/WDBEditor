/*
 * @author amber
 * @date 30/08/2023
 */

#pragma once

#include <stdexcept>
#include <string>

namespace libWDB
{
	class WDBParseException : public std::runtime_error
	{
		public:
			explicit WDBParseException(const std::string& message);
	};
} // namespace libWDB