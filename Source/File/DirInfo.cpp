#include "../include/FileSysInfo.h"

bool BBP::std::DirectoryInfo::loadEntity(std::index_t idx, std::string &str)
{
	// If not populated, return false
	if (populated == false)
		return false;

	// If index is out of bounds, return false and invalidate string
	if (idx >= entityCount)
	{
		str = std::string(0, nullptr);
		return false;
	}

	// If within file range, read file
	if (idx < fileCount)
		return loadFile(idx, str);
	
	// Otherwise read dir
	return loadDirectory(idx - fileCount, str);
}

// Load a file
bool BBP::std::DirectoryInfo::loadFile(std::index_t idx, std::string &str)
{
	// If not populated, return false
	if (populated == false)
		return false;

	// If index is out of bounds, return false and invalidate string
	if (idx >= fileCount)
	{
		str = std::string(0, nullptr);
		return false;
	}

	// Get string
	std::c_string file = stringTable.data + fileIndicies[idx];

	// Load file into string
	str = std::string(std::strlen(file), file);

	// Success
	return true;
}

// Load a directory
bool BBP::std::DirectoryInfo::loadDirectory(std::index_t idx, std::string &str)
{
	// If not populated, return false
	if (populated == false)
		return false;

	// If index is out of bounds, return false and invalidate string
	if (idx >= dirCount)
	{
		str = std::string(0, nullptr);
		return false;
	}

	// Get string
	std::c_string file = stringTable.data + dirIndicies[idx];

	// Load directory into string
	str = std::string(std::strlen(file), file);

	// Success
	return true;
}

// Get entity count
BBP::std::size_t BBP::std::DirectoryInfo::entity_count()
{
	return entityCount;
}

// Get file count
BBP::std::size_t BBP::std::DirectoryInfo::file_count()
{
	return fileCount;
}

// Get directory count
BBP::std::size_t BBP::std::DirectoryInfo::dir_count()
{
	return dirCount;
}

// Does this exist?
bool BBP::std::DirectoryInfo::path_exists()
{
	return exists;
}

// Is this a directory?
bool BBP::std::DirectoryInfo::is_directory()
{
	return isDirectory;
}

void BBP::std::DirectoryInfo::release()
{
	// If not populated, do nothing
	if (populated == false)
		return;

	// If allocator is not set, do nothing
	if (allocator == nullptr)
		return;

	// No longer populated
	populated = false;

	// Release memory, if it exists
	if (stringTable.data)
		allocator->free(stringTable.data);

	if (fileIndicies.data)
		allocator->free(fileIndicies.data);

	if (dirIndicies.data)
		allocator->free(dirIndicies.data);
}