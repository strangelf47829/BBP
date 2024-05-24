#include "../include/drvcmd.h"
#include "../include/Kernel.h"

// Load standard stuff
void BBP::system::Kernel::setPath(std::PATH &path)
{
	// Tell the file driver to load a path
	singleton.fileDriver.hardwareDriver.executeCommand(std::loadPath, 0, 0);

	// Then send path
	singleton.fileDriver.writeData(path.relName());
}

void BBP::system::Kernel::loadFileMetadata(std::PATH &path, std::size_t argc, std::word *argv)
{
	// Set path
	setPath(path);

	// Then query metadata for file
	singleton.fileDriver.hardwareDriver.executeCommand(std::queryFileMetadata, argc, argv);
}

BBP::std::FileSysInfo::FileSysEntryType BBP::system::Kernel::queryFileType(std::PATH &path)
{
	// Set path
	setPath(path);

	// Where to store result
	std::word args[1];

	// Then query metadata for file
	singleton.fileDriver.hardwareDriver.executeCommand(std::queryEntityType, 1, args);

	// Switch statement
	switch (args[0])
	{
	case 0:
		return BBP::std::FileSysInfo::None;
	case 1:
		return BBP::std::FileSysInfo::File;
	case 2:
		return BBP::std::FileSysInfo::Directory;
	default:
	case 3:
		return BBP::std::FileSysInfo::Unkown;
	}
}



// Query if file is on disk or not
bool BBP::system::Kernel::isFileOnDisk(std::PATH &path)
{
	// Create a buffer
	std::word buff[1] = {0};

	// Query
	loadFileMetadata(path, 1, buff);

	// Return first result
	return !!(buff[0]);
}

// Query if path is of type file
bool BBP::system::Kernel::isPathOfTypeFile(std::PATH &path)
{
	// Simple query
	return queryFileType(path) == BBP::std::FileSysInfo::File;
}

// Query if path is of type directory
bool BBP::system::Kernel::isPathOfTypeDirectory(std::PATH &path)
{
	// Simple query
	return queryFileType(path) == BBP::std::FileSysInfo::Directory;
}

// Does anything even exist at path location
bool BBP::system::Kernel::isPathRealObject(std::PATH &path)
{
	// Simple query
	return queryFileType(path) != BBP::std::FileSysInfo::None;
}

// Query file size for file on disk
BBP::std::size_t BBP::system::Kernel::getFilesizeFromDisk(std::PATH &path)
{
	// Create a buffer
	std::word buff[2] = { 0, 0 };

	// Query
	loadFileMetadata(path, 2, buff);

	// If non-existant, return 0
	if (buff[0] == 0)
		return 0;

	// Return second result
	return buff[1];
}

// Read file from disk (this is to stream the entire contents of a file into memory and then dump it)
BBP::std::size_t BBP::system::Kernel::readFileFromDisk(std::stream_t stream, std::PATH &path)
{
	// Set path
	setPath(path);

	// set input stream
	singleton.fileDriver.softwareDriver.setInputPage(&stream);

	// Get file size
	std::word fileMetadata[2];
	singleton.fileDriver.hardwareDriver.executeCommand(std::queryFileMetadata, 2, fileMetadata);
	std::size_t fileSize = fileMetadata[1];

	// Set read mode
	singleton.fileDriver.hardwareDriver.executeCommand(std::setModeRead, 0, 0);
	
	// Open file
	singleton.fileDriver.hardwareDriver.executeCommand(std::openFile, 0, 0);

	// Then request bytes
	std::size_t bytesRead = singleton.fileDriver.receiveData(fileSize);

	// associate the driver
	singleton.fileDriver.Associate();

	// Then close file.
	singleton.fileDriver.hardwareDriver.executeCommand(std::closeFile, 0, 0);

	// Return bytes read
	return bytesRead;
}

// Write file to disk (Takes all the data from the file and dumps it into a file driver)
void BBP::system::Kernel::writeFileToDisk(std::PATH &path, std::stream_t stream)
{
	// Set path
	setPath(path);

	// Set write mode
	singleton.fileDriver.hardwareDriver.executeCommand(std::setModeWrite, 0, 0);

	// Open file
	singleton.fileDriver.hardwareDriver.executeCommand(std::openFile, 0, 0);

	// Send data
	singleton.fileDriver.writeData(*(stream.page), stream.atElement);

	// Close file
	singleton.fileDriver.hardwareDriver.executeCommand(std::closeFile, 0, 0);
}

// Inspect a given path. Returns 0 if success, 
BBP::std::errno_t BBP::system::Kernel::Inspect(std::PATH &path)
{
	// Set path
	setPath(path);

	// Then query metadata for file
	singleton.fileDriver.hardwareDriver.executeCommand(std::inspectPath, 0, 0);

}

// Step iterator
bool BBP::system::Kernel::stepInspectionIterator()
{
	// Get arguments
	std::word args[1] = { 0 };

	// Then query metadata for file. Returns true if iterator still has values
	singleton.fileDriver.hardwareDriver.executeCommand(std::inspectStep, 1, args);

	// Return result
	return args[0];
}

// Step iterator
bool BBP::system::Kernel::canStepInspector()
{
	// Get arguments
	std::word args[1] = { 0 };

	// Then query metadata for file. Returns true if iterator still has values
	singleton.fileDriver.hardwareDriver.executeCommand(std::inspectCanStep, 1, args);

	// Return result
	return args[0];
}

// Get name of path
void BBP::system::Kernel::getInspectorPath(std::string &str)
{
	// Store string length
	std::word args[1];

	// Tell it to emit 
	singleton.fileDriver.hardwareDriver.executeCommand(std::inspectEmitName, 1, args);

	// Create stack
	std::Stack<std::string_element> stream(&str);

	// Then set input to that
	singleton.fileDriver.softwareDriver.setInputPage(&stream);

	// Then receive string 
	singleton.fileDriver.receiveData(args[0]);

	// Associate driver
	singleton.fileDriver.Associate();
}

// Get inspector file type
BBP::std::FileSysInfo::FileSysEntryType BBP::system::Kernel::getInspectorFileType()
{
	// Store string length
	std::word args[1];

	// Tell it to emit 
	singleton.fileDriver.hardwareDriver.executeCommand(std::inspectGetType, 1, args);
	
	// Then return result
	switch (args[0])
	{
	case 0:
		return BBP::std::FileSysInfo::None;
	case 1:
		return BBP::std::FileSysInfo::File;
	case 2:
		return BBP::std::FileSysInfo::Directory;
	default:
	case 3:
		return BBP::std::FileSysInfo::Unkown;
	}
}