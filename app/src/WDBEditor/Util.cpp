/*
 * @author amber
 * @date 02/09/2023
 */

#include "WDBEditor/Util.hpp"

#include <stdexcept>

namespace WDBEditor
{
	auto HomeDirectory() noexcept(false) -> std::string
	{
		std::string path {};

#if ((defined(WIN32) || defined(_WIN32) || defined(__WIN32)) && (!defined(__CYGWIN__)))
		char* tmp_path_buffer;
		std::size_t path_string_size = 0;

		const errno_t dupenv_error = _dupenv_s(&tmp_path_buffer, &path_string_size, "HOMEPATH");

		path = std::string {tmp_path_buffer, path_string_size};

		if (nullptr != tmp_path_buffer)
		{
			free(tmp_path_buffer);
		}

		if (0 != dupenv_error)
		{
			throw std::runtime_error("Failed to fetch \"HOMEPATH\" environment variable!");
		}

#elif defined(UNIX)
		path = std::string {getenv("PATH")};
#else
#error                                                                                                                 \
	"No compatible way to get user home directory has been implemented yet! Please feel free to report this as a bug!"
#endif

		return path;
	}
} // namespace WDBEditor