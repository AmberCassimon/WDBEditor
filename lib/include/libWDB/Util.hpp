/*
 * @author amber
 * @date 30/08/2023
 */

#pragma once

#include <string>
#include <vector>

namespace libWDB
{
	auto Filesize(FILE* fileptr) noexcept(false) -> long;

	// Read binary data
	auto Uint32FromLEBytes(unsigned char** ptr) -> std::uint32_t;

	auto ByteArrayFromLEBytes(unsigned char** ptr, std::uint32_t length) -> std::vector<unsigned char>;

	auto ASCIIStringFromLEBytes(unsigned char** ptr, std::uint32_t length) -> std::string;

	// Write binary data
	auto Uint32ToLEBytes(std::uint32_t u32, FILE* fileptr) -> void;

	auto ByteArrayToLEBytes(const std::vector<unsigned char>& bytes, FILE* fileptr) -> void;

	auto ASCIIStringToLEBytes(const std::string& str, FILE* fileptr) -> void;
} // namespace libWDB