/*
 * @author amber
 * @date 30/08/2023
 */

#include "libWDB/WDBParseException.hpp"

namespace libWDB
{
	WDBParseException::WDBParseException(const std::string& message): std::runtime_error(message) {}
} // namespace libWDB
