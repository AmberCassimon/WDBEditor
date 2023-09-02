/*
 * @author amber
 * @date 30/08/2023
 */

#include "libWDB/Util.hpp"

#include <sstream>
#include <stdexcept>

namespace libWDB
{
	auto Filesize(FILE* fileptr) noexcept(false) -> long
	{
		const long current_pos = ftell(fileptr);

		if (-1L == current_pos)
		{
			char error_buffer[255];

			strerror_s(&error_buffer[0], sizeof(error_buffer), errno);

			throw std::runtime_error("Failed to get current position in file: " + std::string {error_buffer});
		}

		if (fseek(fileptr, 0, SEEK_END))
		{
			throw std::runtime_error("Failed to seek to end of file!");
		}

		const long filesize = ftell(fileptr);

		if (-1L == filesize)
		{
			char error_buffer[255];

			strerror_s(&error_buffer[0], sizeof(error_buffer), errno);

			throw std::runtime_error("Call to ftell() failed: " + std::string {error_buffer});
		}

		if (fseek(fileptr, current_pos, SEEK_SET))
		{
			throw std::runtime_error("Failed to re-seek to position at the beginning of function!");
		}

		return filesize;
	}

	auto Uint32FromLEBytes(unsigned char** byte_ptr) -> std::uint32_t
	{
		const std::uint32_t u32 =
			(*byte_ptr)[0] | ((*byte_ptr)[1] << 8) | ((*byte_ptr)[2] << 16) | ((*byte_ptr)[3] << 24);

		if (nullptr != (*byte_ptr))
		{
			*byte_ptr += 4;
		}

		return u32;
	}

	auto ByteArrayFromLEBytes(unsigned char** ptr, std::uint32_t length) -> std::vector<unsigned char>
	{
		std::vector<unsigned char> bytes {*ptr, *ptr + length};
		*ptr += length;

		return bytes;
	}

	auto ASCIIStringFromLEBytes(unsigned char** ptr, std::uint32_t length) -> std::string
	{
		std::string string {*ptr, *ptr + length};
		*ptr += length;

		return string;
	}
} // namespace libWDB