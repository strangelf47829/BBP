#include "../Drivers/FileSystem/Filedrvhdr.h"

#include <fstream>

#define _HAS_CXX17
#include <filesystem>
namespace fs = std::filesystem;

// Writing
std::ofstream writeStream;


// Open for writing
void Host::Drivers::FileSystem::openFileWriting()
{
	// Open stream
	writeStream = std::ofstream(activeFile.relName(), ::std::ios::out | ::std::ios_base::binary);
}

// Close for writing
void Host::Drivers::FileSystem::closeFileWriting()
{
	// Close stream
	writeStream.close();
}

// Write a file into
void Host::Drivers::FileSystem::writeFile(BBP::std::size_t amount, BBP::std::PAGE<BBP::std::string_element> &data)
{
	// Write a file
	for (BBP::std::index_t idx = 0; idx < amount; idx++)
		writeStream << data[idx];
}