#include "../include/FileSysInfo.h"
#include "../include/stddrv.h"

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

	// Check if path actually exists
	this->exists = std::isPathRealObject(absPath);

	// if does not exist...
	if (this->exists == false)
		return false;

	// Check if is directory
	this->isDirectory = std::isPathOfTypeDirectory(absPath);

	// If no such directory...
	if (this->isDirectory == false)
		return false;

	// Initialize values
	fileCount = 0;
	dirCount = 0;

	// Count the amount of bytes taken per string (excluding null-terminators)
	fileLength = 0;
	dirLength = 0;

	// Get directory iterator.
	std::Inspect(absPath);

	// Get current string length
	std::size_t pathString_length = std::strlen(absPathString);

	// Count
	while (std::canStepInspector())
	{
		// Allocate static string for path
		std::static_string<std::max_path_length> entryPath;

		// Get the name of this path
		std::getInspectorPath(entryPath);

		// Get length to string
		std::size_t entryPath_length = std::strlen(entryPath);

		// If entryPath is smaller than or equal to pathString, skip the entry.
		if (entryPath_length <= pathString_length)
			continue;

		// Set offset (Fix directory issues later)
		std::string entryPathAdjusted = std::string(entryPath_length - pathString_length, entryPath.data + pathString_length);

		// Check for entry type
		if (std::getInspectorFileType() == std::FileSysInfo::Directory)
		{
			// Increase dircount, and add (pathString_length + 1 (for trailing '/')) to dirLength
			dirCount++;
			dirLength += 1 + entryPath_length;
		}
		else if (std::getInspectorFileType() == std::FileSysInfo::File)
		{
			// Increase filecount, and add pathString_length to fileLength
			fileCount++;
			fileLength += entryPath_length;
		}

		// Step over
		std::stepInspectionIterator();
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

	// Path with root attached
	std::PATH rootPath = path.makeAbsolutePath();

	// Scarcely populate. 
	bool scarce_result = scarce_populate(rootPath);
	
	// If could not scarcely populate, return error
	if (scarce_result == false)
		return false;

	// Get full-path name
	std::PATH absPath;
	absPath.copyFrom(rootPath);
	absPath.makeAbsolutePath();

	// Get string thereof
	std::c_string absPathString = absPath.relName();

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

	// Iterate over path
	std::Inspect(path);

	// Get current string length
	std::size_t pathString_length = std::strlen(absPathString);

	// Iterate over everything
	while (std::canStepInspector())
	{
		// Allocate static string for path
		std::static_string<std::max_path_length> entryPath;

		// Get the name of this path
		std::getInspectorPath(entryPath);

		// Get length to string
		std::size_t entryPath_length = std::strlen(entryPath);

		// If entryPath is smaller than or equal to pathString, skip the entry.
		if (entryPath_length <= pathString_length)
			continue;

		// Set offset (Fix directory issues later)
		std::string entryPathAdjusted = std::string(entryPath_length - pathString_length, entryPath.data + pathString_length);

		// Get current stack index
		currentStringTableLength = stringTableStack.atElement;

		// Check for entry type
		if (std::getInspectorFileType() == FileSysInfo::FileSysEntryType::Directory)
		{
			// Directory type, so set index for directory to be ..
			std::write(&dirIndicies, currentStringTableLength, currentDirIndex);

			// Increase directory index
			currentDirIndex++;

			// Write .. into stack
			stringTableStack << entryPathAdjusted <<= "/";

		}
		else if (std::getInspectorFileType() == FileSysInfo::FileSysEntryType::File)
		{
			// File type, so set index for file to be ..
			std::write(&fileIndicies, currentStringTableLength, currentFileIndex);

			// Increase file index
			currentFileIndex++;

			// Write .. into stack
			stringTableStack <<= entryPathAdjusted;
		}

		// Step over
		std::stepInspectionIterator();
	}

	return true;
}