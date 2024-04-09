#include "../../../include/FileSysInfo.h"

#define _HAS_CXX17
#include <filesystem>
namespace fs = std::filesystem;

// 

bool BBP::std::populateVolumeInfo(std::VolumeInfo &info, std::PATH &path)
{
	return false;
}

bool BBP::std::DirectoryInfo::scarce_populate(std::PATH &path)
{
	// Get full-path name
	std::PATH absPath;
	absPath.copyFrom(path);
	absPath.makeAbsolutePath();

	// Get string thereof
	std::c_string absPathString = absPath.relName();

	// Get C++17 path
	fs::path filePath(absPathString);

	// Declare error code
	::std::error_code errorCode;

	// Check if path actually exists
	this->exists = fs::exists(filePath, errorCode);

	// if does not exist...
	if (this->exists == false || errorCode)
		return false;

	// Check if is directory
	this->isDirectory = fs::is_directory(filePath, errorCode);

	// If no such directory...
	if (this->isDirectory == false || errorCode)
		return false;

	// Initialize values
	fileCount = 0;
	dirCount = 0;

	// Count the amount of bytes taken per string (excluding null-terminators)
	fileLength = 0;
	dirLength = 0;

	// Get directory iterator.
	::std::string pathString = absPathString;
	fs::directory_iterator iterator(pathString);

	// Get current string length
	std::size_t pathString_length = std::strlen(absPathString);

	// Count
	for (const auto &entry : iterator)
	{
		// Get string
		std::c_string entryPath_cstr = (std::c_string)entry.path().c_str();

		// Get length to string
		std::size_t entryPath_length = std::strlen(entryPath_cstr);

		// If entryPath is smaller than or equal to pathString, skip the entry.
		if (entryPath_length <= pathString_length)
			continue;

		// Set offset (Fix directory issues later)
		entryPath_cstr = entryPath_cstr + pathString_length;
		entryPath_length -= pathString_length;

		// Check for entry type
		if (entry.is_directory())
		{
			// Increase dircount, and add (pathString_length + 1 (for trailing '/')) to dirLength
			dirCount++;
			dirLength += 1 + entryPath_length;
		}
		else if (entry.is_regular_file())
		{
			// Increase filecount, and add pathString_length to fileLength
			fileCount++;
			fileLength += entryPath_length;
		}
	}

	// Set entity count
	entityCount = fileCount + dirCount;

	// Add null terminators
	fileLength += fileCount;
	dirLength += dirCount;

	// Return success
	return true;
}

bool BBP::std::DirectoryInfo::populate(std::PATH &path, std::ResourceManager *allocator)
{
	// Check if populated. If it is, release resources
	if (populated)
		release();

	// If allocator does not exist, return false already.
	if (allocator == nullptr)
		return false;

	// Set allocator
	this->allocator = allocator;

	// Scarcely populate. 
	bool scarce_result = scarce_populate(path);
	
	// If could not scarcely populate, return error
	if (scarce_result == false)
		return false;

	// Get full-path name
	std::PATH absPath;
	absPath.copyFrom(path);
	absPath.makeAbsolutePath();

	// Get string thereof
	std::c_string absPathString = absPath.relName();

	// Get C++17 path
	fs::path filePath(absPathString);

	// Declare error code
	::std::error_code errorCode;

	// Allocate memory
	allocator->page_calloc(stringTable, dirLength + fileLength);
	allocator->page_calloc(fileIndicies, fileCount);
	allocator->page_calloc(dirIndicies, dirCount);

	// Mark populated
	populated = true;

	// Keep track of current file and directory index
	std::index_t currentFileIndex = 0;
	std::index_t currentDirIndex = 0;

	// Keep track of current length in string table
	std::offset_t currentStringTableLength = 0;

	// Create stack for string table
	std::Stack<std::string_element> stringTableStack(&stringTable);

	// Create second iterator, and iterate.
	::std::string pathString = absPathString;
	fs::directory_iterator iterator2(pathString);

	// Get current string length
	std::size_t pathString_length = std::strlen(absPathString);

	// Iterate over everything
	for (const auto &entry : iterator2)
	{
		// Get string
		std::c_string entryPath_cstr = (std::c_string)entry.path().c_str();

		// Get length to string
		std::size_t entryPath_length = std::strlen(entryPath_cstr);

		// If entryPath is smaller than or equal to pathString, skip the entry.
		if (entryPath_length <= pathString_length)
			continue;

		// Set offset (Fix directory issues later)
		entryPath_cstr = entryPath_cstr + pathString_length;
		entryPath_length -= pathString_length;

		// Get current stack index
		currentStringTableLength = stringTableStack.atElement;

		// Check for entry type
		if (entry.is_directory())
		{
			// Directory type, so set index for directory to be ..
			std::write(&dirIndicies, currentStringTableLength, currentDirIndex);

			// Increase directory index
			currentDirIndex++;

			// Write .. into stack
			stringTableStack << entryPath_cstr <<= "/";

		}
		else if (entry.is_regular_file())
		{
			// File type, so set index for file to be ..
			std::write(&fileIndicies, currentStringTableLength, currentFileIndex);

			// Increase file index
			currentFileIndex++;

			// Write .. into stack
			stringTableStack <<= entryPath_cstr;
		}
	}

	return true;
}