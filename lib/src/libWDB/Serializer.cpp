/*
 * @author amber
 * @date 02/09/2023
 */

#include "libWDB/Serializer.hpp"

#include "libWDB/Util.hpp"

namespace libWDB
{
	auto Save(
		const WorldDatabase& database,
		FILE* fileptr
	) -> void
	{
		// Write the number of groups
		Uint32ToLEBytes(
			static_cast<std::uint32_t>(database.Groups().size()),
			fileptr
		);
	}
} // namespace libWDB