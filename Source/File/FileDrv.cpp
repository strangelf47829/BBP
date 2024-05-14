#include "../include/FileSys.h"
#include "../include/stddrv.h"

bool BBP::std::FILE::doesFileExistOnDisk(BBP::std::PATH &_path)
{
	_is_on_disk = std::isFileOnDisk(_path);

	return _is_on_disk;
}

BBP::std::size_t BBP::std::FILE::getFileSizeFromDisk(BBP::std::PATH &_path)
{
	// Check if file exists
	if (std::isPathOfTypeFile(_path) == false)
		throw std::exception("ERR_FILE_NOEXIST", ENOENT);

	// Return bytecount
	return std::getFilesizeFromDisk(_path);
}

BBP::std::size_t BBP::std::FILE::readFileFromDisk(BBP::std::FileNode *n, BBP::std::PATH &_path)
{

	// Check if file exists
	if (std::isPathOfTypeFile(_path))
		throw std::exception("ERR_FILE_NOEXIST", ENOENT);

	// Read file
	std::size_t bytesRead = std::readFileFromDisk(n->fileData, _path);

	// Set _is_on_disk
	_is_on_disk = true;

	// Return byte count
	return bytesRead;
}

void BBP::std::FILE::writeFileToDisk(std::PATH &path)
{
	// Get data length
	std::string *_data = data()->fileData.page;
	std::size_t dataSize = std::seqlen(*_data);

	// Write file
	std::writeFileToDisk(path, b());
}

void BBP::std::FILE::writeFileToDisk()
{
	writeFileToDisk(data()->filePath);
}