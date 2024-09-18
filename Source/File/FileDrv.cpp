#include "../include/FileSys.h"
#include "../include/Kernel.h"

bool BBP::std::FILE::doesFileExistOnDisk(BBP::std::PATH &_path)
{
	_is_on_disk = system::Kernel::isFileOnDisk(_path);

	return _is_on_disk;
}

BBP::std::size_t BBP::std::FILE::getFileSizeFromDisk(BBP::std::PATH &_path)
{
	// Check if file exists
	if (system::Kernel::isPathOfTypeFile(_path) == false)
		throw std::exception("ERR_FILE_NOEXIST", ENOENT);

	// Return bytecount
	return system::Kernel::getFilesizeFromDisk(_path);
}

BBP::std::size_t BBP::std::FILE::readFileFromDisk(BBP::std::FileNode *n, BBP::std::PATH &_path)
{

	// Check if file exists
	if (system::Kernel::isPathOfTypeFile(_path))
		throw std::exception("ERR_FILE_NOEXIST", ENOENT);

	// Read file
	std::size_t bytesRead = system::Kernel::readFileFromDisk(n->fileData, _path);

	// Set _is_on_disk
	_is_on_disk = true;

	// Return byte count
	return bytesRead;
}

void BBP::std::FILE::writeFileToDisk(std::PATH &path)
{
	// If nothing to write do nothing
	if (data == nullptr)
		return;

	// Get data length
	std::string *_data = data->page;
	std::size_t dataSize = std::seqlen(*_data);

	// Store old max_elements and atElement
	std::size_t oldMax = data->max_elements;
	std::size_t oldIdx = data->atElement;

	// Resize stack to that size
	data->max_elements = dataSize;
	data->atElement = dataSize;

	// Write file
	system::Kernel::writeFileToDisk(path, b());

	// Restore old values
	data->max_elements = oldMax;
	data->atElement = oldIdx;
}

void BBP::std::FILE::writeFileToDisk()
{
	writeFileToDisk(system::Kernel::getNodePath(this->node));
}