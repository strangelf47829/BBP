#include "../Drivers/FileSystem/Filedrvhdr.h"

#include <fstream>

#define _HAS_CXX17
#include <filesystem>
namespace fs = std::filesystem;

BBP::std::PATH Host::Drivers::FileSystem::activeFile("/mnt/v/");

bool Host::Drivers::FileSystem::doesPathExist()
{
	// Open fstream
	::std::ifstream _file(activeFile.relName());

	// Store file isopen
	bool exists = _file.is_open();

	// Close file
	_file.close();

	// Return result
	return exists;
}


BBP::std::word Host::Drivers::FileSystem::getFileSize()
{
	// Open fstream
	::std::ifstream _file(activeFile.relName());

	// Check if file is open. Return -1 if nothing
	if (!_file.is_open())
		return -1;

	// Get bytecount
	_file.seekg(0, ::std::ios::end);
	BBP::std::size_t size = _file.tellg();
	_file.seekg(0, ::std::ios::beg);

	// Close file
	_file.close();

	// Return bytecount
	return size;
}

bool Host::Drivers::FileSystem::isDirectory()
{
	// Get C string representation
	BBP::std::c_string pathName = activeFile.relName();

	// Then get path
	fs::path path(pathName);

	// Then return is directory
	return fs::is_directory(path);
}

bool Host::Drivers::FileSystem::isFile()
{
	// Get C string representation
	BBP::std::c_string pathName = activeFile.relName();

	// Then get path
	fs::path path(pathName);

	// Then return is file
	return fs::is_regular_file(path);
}

bool Host::Drivers::FileSystem::doesEntityExist()
{
	// Get C string representation
	BBP::std::c_string pathName = activeFile.relName();

	// Then get path
	fs::path path(pathName);

	// Then return is file
	return fs::exists(path);
}