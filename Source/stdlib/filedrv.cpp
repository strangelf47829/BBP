#include "../include/stddrv.h"
#include "../include/drvcmd.h"
#include "../include/Kernel.h"

// File driver.
// File driver has following commands:

// 0 - LOAD PATH (the next data received should be written into path field)

// Query if file is on disk or not
bool BBP::std::isFileOnDisk(PATH &path)
{
	// Get file driver
	BBP::system::DeviceDriver *driver = &system::getKernelInstance().getFileDriver();

	// Tell the file driver to load a path
	driver->hardwareDriver.executeCommand(loadPath, 0, 0);
	
	// Then send path
	driver->writeData(path.relName());

	// Create buffer of 2 arguments to be supplied to the command. Then, send back
	std::word args[1];

	// Then query metadata for file
	driver->hardwareDriver.executeCommand(queryFileMetadata, 1, args);

	// Then return first arg
	return args[0];
}

// Query if path is of type file
bool BBP::std::isPathOfTypeFile(PATH &)
{

}

// Query if path is of type directory
bool BBP::std::isPathOfTypeDirectory(PATH &)
{

}

// Does anything even exist at path location
bool BBP::std::isPathRealObject(PATH &)
{

}

// Query file size for file on disk
BBP::std::size_t BBP::std::getFilesizeFromDisk(PATH &)
{

}

// Read file from disk (this is to stream the entire contents of a file into memory and then dump it)
BBP::std::size_t BBP::std::readFileFromDisk(stream_t, PATH &)
{

}

// Write file to disk (Takes all the data from the file and dumps it into a file driver)
void BBP::std::writeFileToDisk(PATH &, fstream_t)
{

}

// Inspect a given path. Returns 0 if success, 
BBP::std::errno_t BBP::std::Inspect(PATH &)
{

}

// Get file and directory count for a given path
BBP::std::size_t BBP::std::fileCount()
{

}

BBP::std::size_t BBP::std::directoryCount()
{

}

// Get file name given an index
BBP::std::c_string BBP::std::getFileName(std::index_t)
{

}

// Get directory name given an index
BBP::std::c_string BBP::std::getDirectoryName(std::index_t)
{

}