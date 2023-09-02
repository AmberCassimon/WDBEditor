/*
 * @author amber
 * @date 30/08/2023
 */

#include "libWDB/Util.hpp"

#include <cassert>
#include <cstdio>

#include <array>
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

	auto Uint32ToLEBytes(std::uint32_t u32, FILE* fileptr) -> void
	{
		const std::array<unsigned char, 4> bytes = {
			static_cast<unsigned char>(u32 & 0x000000FFU),
			static_cast<unsigned char>((u32 & 0x0000FF00U) >> 8U),
			static_cast<unsigned char>((u32 & 0x00FF0000U) >> 16U),
			static_cast<unsigned char>((u32 & 0xFF000000U) >> 24U),
		};

		const int elems_written = fwrite(
			bytes.data(),
			sizeof(unsigned char),
			bytes.size(),
			fileptr
		);

		assert(4 == elems_written);
	}

	auto ByteArrayToLEBytes(const std::vector<unsigned char>& bytes, FILE* fileptr) -> void
	{
		const int chars_written = fwrite(
			bytes.data(),
			sizeof(char),
			bytes.size(),
			fileptr
		);

		assert(bytes.size() == chars_written);
	}

	auto ASCIIStringToLEBytes(const std::string& str, FILE* fileptr) -> void
	{
		const int chars_written = fwrite(
			str.c_str(),
			sizeof(char),
			str.size(),
			fileptr
		);

		assert(str.size() == chars_written);
	}
} // namespace libWDB