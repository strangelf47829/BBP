#include "../include/stddrv.h"
#include "../include/drvcmd.h"
#include "../include/Kernel.h"
#include "../include/filedrv.h"

// Load standard stuff
void BBP::std::setPath(std::PATH &path)
{
	// Get file driver
	BBP::system::DeviceDriver *driver = &system::getKernelInstance().getFileDriver();

	// Tell the file driver to load a path
	driver->hardwareDriver.executeCommand(loadPath, 0, 0);

	// Then send path
	driver->writeData(path.relName());
}

void BBP::std::loadFileMetadata(std::PATH &path, std::size_t argc, std::word *argv)
{
	// Get file driver
	BBP::system::DeviceDriver *driver = &system::getKernelInstance().getFileDriver();

	// Set path
	setPath(path);

	// Then query metadata for file
	driver->hardwareDriver.executeCommand(queryFileMetadata, argc, argv);
}

BBP::std::FileSysInfo::FileSysEntryType BBP::std::queryFileType(std::PATH &path)
{
	// Get file driver
	BBP::system::DeviceDriver *driver = &system::getKernelInstance().getFileDriver();

	// Set path
	setPath(path);

	// Where to store result
	std::word args[1];

	// Then query metadata for file
	driver->hardwareDriver.executeCommand(queryEntityType, 1, args);

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
bool BBP::std::isFileOnDisk(PATH &path)
{
	// Create a buffer
	std::word buff[1] = {0};

	// Query
	loadFileMetadata(path, 1, buff);

	// Return first result
	return !!(buff[0]);
}

// Query if path is of type file
bool BBP::std::isPathOfTypeFile(PATH &path)
{
	// Simple query
	return queryFileType(path) == BBP::std::FileSysInfo::File;
}

// Query if path is of type directory
bool BBP::std::isPathOfTypeDirectory(PATH &path)
{
	// Simple query
	return queryFileType(path) == BBP::std::FileSysInfo::Directory;
}

// Does anything even exist at path location
bool BBP::std::isPathRealObject(PATH &path)
{
	// Simple query
	return queryFileType(path) != BBP::std::FileSysInfo::None;
}

// Query file size for file on disk
BBP::std::size_t BBP::std::getFilesizeFromDisk(PATH &path)
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
BBP::std::size_t BBP::std::readFileFromDisk(stream_t stream, PATH &path)
{
	// Get driver
	BBP::system::DeviceDriver *driver = &system::getKernelInstance().getFileDriver();

	// Set path
	setPath(path);

	// set input stream
	driver->softwareDriver.setInputPage(&stream);

	// Get file size
	std::word fileMetadata[2];
	driver->hardwareDriver.executeCommand(queryFileMetadata, 2, fileMetadata);
	std::size_t fileSize = fileMetadata[1];

	// Set read mode
	driver->hardwareDriver.executeCommand(setModeRead, 0, 0);

	// Open file
	driver->hardwareDriver.executeCommand(openFile, 0, 0);

	// Then request bytes
	std::size_t bytesRead = driver->receiveData(fileSize);

	// associate the driver
	driver->Associate();

	// Then close file.
	driver->hardwareDriver.executeCommand(closeFile, 0, 0);

	// Return bytes read
	return bytesRead;
}

// Write file to disk (Takes all the data from the file and dumps it into a file driver)
void BBP::std::writeFileToDisk(PATH &path, fstream_t file)
{
	// If no valid data or no file do nothing
	if (file == nullptr)
		return;

	if (file->b().page == nullptr)
		return;

	// Get driver
	BBP::system::DeviceDriver *driver = &system::getKernelInstance().getFileDriver();

	// Set path
	setPath(path);

	// Set write mode
	driver->hardwareDriver.executeCommand(setModeWrite, 0, 0);

	// Open file
	driver->hardwareDriver.executeCommand(openFile, 0, 0);

	// Send data
	driver->writeData(*file->b().page, file->b().atElement);

	// Close file
	driver->hardwareDriver.executeCommand(closeFile, 0, 0);
}

// Inspect a given path. Returns 0 if success, 
BBP::std::errno_t BBP::std::Inspect(PATH &path)
{
	// Get file driver
	BBP::system::DeviceDriver *driver = &system::getKernelInstance().getFileDriver();

	// Set path
	setPath(path);

	// Then query metadata for file
	driver->hardwareDriver.executeCommand(inspectPath, 0, 0);

}

// Step iterator
bool BBP::std::stepInspectionIterator()
{
	// Get file driver
	BBP::system::DeviceDriver *driver = &system::getKernelInstance().getFileDriver();

	// Get arguments
	std::word args[1] = { 0 };

	// Then query metadata for file. Returns true if iterator still has values
	driver->hardwareDriver.executeCommand(inspectStep, 1, args);

	// Return result
	return args[0];
}

// Step iterator
bool BBP::std::canStepInspector()
{
	// Get file driver
	BBP::system::DeviceDriver *driver = &system::getKernelInstance().getFileDriver();

	// Get arguments
	std::word args[1] = { 0 };

	// Then query metadata for file. Returns true if iterator still has values
	driver->hardwareDriver.executeCommand(inspectCanStep, 1, args);

	// Return result
	return args[0];
}

// Get name of path
void BBP::std::getInspectorPath(std::string &str)
{
	// Get file driver
	BBP::system::DeviceDriver *driver = &system::getKernelInstance().getFileDriver();

	// Store string length
	std::word args[1];

	// Tell it to emit 
	driver->hardwareDriver.executeCommand(inspectEmitName, 1, args);

	// Create stack
	std::Stack<std::string_element> stream(&str);

	// Then set input to that
	driver->softwareDriver.setInputPage(&stream);

	// Then receive string 
	driver->receiveData(args[0]);

	// Associate driver
	driver->Associate();
}

// Get inspector file type
BBP::std::FileSysInfo::FileSysEntryType BBP::std::getInspectorFileType()
{
	// Get file driver
	BBP::system::DeviceDriver *driver = &system::getKernelInstance().getFileDriver();

	// Store string length
	std::word args[1];

	// Tell it to emit 
	driver->hardwareDriver.executeCommand(inspectGetType, 1, args);
	
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