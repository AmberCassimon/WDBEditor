/*
 * @author amber
 * @date 06/09/2023
 */

#include <cstdio>

#include <libWDB/Parser.hpp>
#include <libWDB/Serializer.hpp>
#include <libWDB/WorldDatabase.hpp>

auto main(const int argc, const char* argv[]) -> int
{
	if (3 != argc)
	{
		std::fprintf(stderr, "Usage: Reconstruct <PATH TO WORLD.WDB> <PATH TO RECONSTRUCTED FILE>");
		return 255;
	}

	const char* world_wdb_filename = argv[1];
	const char* reconnstructed_filename = argv[2];

	// Parse the file
	FILE* read_file = fopen(world_wdb_filename, "rb");

	if (nullptr == read_file)
	{
		std::fprintf(stderr, "Failed to open WORLD.wdb!");
		return 255;
	}

	libWDB::WorldDatabase wdb = libWDB::ParseWDB(read_file);

	fclose(read_file);

	// Serialize the file
	FILE* write_file = fopen(reconnstructed_filename, "wb");

	if (nullptr == write_file)
	{
		std::fprintf(stderr, "Failed to open WORLD2.wdb!");
		return 255;
	}

	libWDB::Save(wdb, write_file);

	fclose(write_file);
}