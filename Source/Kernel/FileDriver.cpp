#include "../include/drvcmd.h"
#include "../include/Kernel.h"


// Query if file is on disk or not
bool BBP::system::Kernel::isFileOnDisk(std::PATH &path)
{
	// Core implemented
	return singleton.Core().firmware->isFileOnDisk(path);
}

// Query if path is of type file
bool BBP::system::Kernel::isPathOfTypeFile(std::PATH &path)
{
	// Core implemented
	return singleton.Core().firmware->isPathOfTypeFile(path);
}

// Query if path is of type directory
bool BBP::system::Kernel::isPathOfTypeDirectory(std::PATH &path)
{
	// Core implemented
	return singleton.Core().firmware->isPathOfTypeDirectory(path);
}

// Does anything even exist at path location
bool BBP::system::Kernel::isPathRealObject(std::PATH &path)
{
	// Core implemented
	return singleton.Core().firmware->isPathRealObject(path);
}

// Query file size for file on disk
BBP::std::size_t BBP::system::Kernel::getFilesizeFromDisk(std::PATH &path)
{
	// Core implemented
	return singleton.Core().firmware->getFilesizeFromDisk(path);
}

// Read file from disk (this is to stream the entire contents of a file into memory and then dump it)
BBP::std::size_t BBP::system::Kernel::readFileFromDisk(std::stream_t stream, std::PATH &path)
{
	// Core implemented
	return singleton.Core().firmware->readFileFromDisk(stream, path);
}

// Write file to disk (Takes all the data from the file and dumps it into a file driver)
void BBP::system::Kernel::writeFileToDisk(std::PATH &path, std::stream_t stream)
{
	// Core implemented
	return singleton.Core().firmware->writeFileToDisk(path, stream);
}

// Inspect a given path. Returns 0 if success, 
BBP::std::errno_t BBP::system::Kernel::Inspect(std::PATH &path)
{
	// Core implemented
	return singleton.Core().firmware->Inspect(path);
}

// Step iterator
bool BBP::system::Kernel::stepInspectionIterator()
{
	// Core implemented
	return singleton.Core().firmware->stepInspectionIterator();
}

// Step iterator
bool BBP::system::Kernel::canStepInspector()
{
	// Core implemented
	return singleton.Core().firmware->canStepInspector();
}

// Get name of path
void BBP::system::Kernel::getInspectorPath(std::string &str)
{
	// Core implemented
	return singleton.Core().firmware->getInspectorPath(str);
}

// Get inspector file type
BBP::std::FileSysInfo::FileSysEntryType BBP::system::Kernel::getInspectorFileType()
{
	// Core implemented
	return singleton.Core().firmware->getInspectorFileType();
}