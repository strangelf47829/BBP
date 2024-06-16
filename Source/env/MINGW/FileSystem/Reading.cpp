#include "../Drivers/FileSystem/Filedrvhdr.h"

#include <fstream>

#define _HAS_CXX17
#include <filesystem>
namespace fs = std::filesystem;

// Reading
std::ifstream readStream;

// Open for reading
void Host::Drivers::FileSystem::openFileReading()
{
	// Open stream
	readStream = std::ifstream(activeFile.relName(), ::std::ios_base::binary);
}

// Close for reading
void Host::Drivers::FileSystem::closeFileReading()
{
	// Close stream
	readStream.close();
}

// Read a character
bool Host::Drivers::FileSystem::readCharacter(char &c)
{
	// Stupid workaround ?
	if (readStream.get(c))
		return true;
	return false;
}