#include "../include/BIOS.h"

#include <fstream>

// Input stream
std::ifstream inputFile;

// Output stream
std::ofstream outputFile;

// Basic input: get file 
BBP::std::errno_t Host::BIOS::fopen(BBP::std::c_string path)
{
	// If file already opened, return error
	if (inputFile.is_open())
		return EBUSY;

	// if 'path' is nullptr return error
	if (path == nullptr)
		return EINVAL;

	// open file
	inputFile = std::ifstream(path, std::ios_base::binary);

	// if not open, return error
	if (inputFile.is_open() == false)
		return ENOENT;

	// Return no error
	return ENONE;
}

// Basic input: get bios stuff
BBP::std::errno_t Host::BIOS::fopen_b(BBP::std::conststring path)
{
	// if 'path' is nullptr return error
	if (path == nullptr)
		return EINVAL;

	// Get C++ string
	std::string str = "/v/boot/";

	// Add
	str += path;

	// open
	return fopen((BBP::std::c_string)str.c_str());
}


BBP::std::errno_t Host::BIOS::fclose()
{
	// If not open, return error
	if (inputFile.is_open() == false)
		return EPERM;

	// Otherwise close and return no error
	inputFile.close();
	return ENONE;
}

// Basic input: get file size
BBP::std::size_t Host::BIOS::fsize()
{
	// If file not open, return 0
	if (inputFile.is_open() == false)
		return 0;

	// Otherwise, read size
	inputFile.seekg(0, ::std::ios::end);
	BBP::std::size_t size = inputFile.tellg();
	inputFile.seekg(0, ::std::ios::beg);

	// Return size
	return size;
}

// Basic input: read file element
BBP::std::errno_t Host::BIOS::fread(BBP::std::string_element &ch)
{
	// If no input file, get errno
	if (inputFile.is_open() == false)
		return EPERM;

	// Otherwise, get character and return success
	if (inputFile.get(ch))
		return ENONE;
	
	// Otherwise return Error
	return EIO;
}

// Basic output: write entire file
BBP::std::errno_t Host::BIOS::fwrite(BBP::std::c_string path, BBP::std::Stack<BBP::std::string_element> &stream)
{
	// If path is nullptr, return error
	if (path == nullptr)
		return EINVAL;

	// If stream is invalid, return error
	if (stream.page == nullptr)
		return EPIPE;

	// Open file
	outputFile = std::ofstream(path, std::ios::out | std::ios_base::binary);

	// If not open, return error
	if (outputFile.is_open() == false)
		return EIO;

	// Otherwise get stream count
	BBP::std::size_t len = BBP::std::seqlen(*(stream.page));

	// Write file
	for (BBP::std::index_t idx = 0; idx < len; idx++)
		outputFile << stream[idx];

	// Close file
	outputFile.close();

	// Return no error
	return ENONE;
}