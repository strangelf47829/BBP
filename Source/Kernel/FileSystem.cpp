#include "../include/Kernel.h"

BBP::std::Stack<BBP::std::string_element> *BBP::system::Kernel::openVirtualFile(std::noderef_t node, std::Stack<std::string_element> &data, std::VOLUME *v, std::PATH &path)
{
	// Create file node
	singleton.fileTable[node] = BBP::std::FileNode(data, v, path);

	// Return file node data
	return &singleton.fileTable[node].fileData;
}

BBP::std::Stack<BBP::std::string_element> *BBP::system::Kernel::readFileFromDisk(std::noderef_t node, std::PATH &path)
{
	// Create path from absolute
	std::PATH absPath = path;
	absPath.makeAbsolutePath();

	// Get file size
	std::size_t fileSize = getFilesizeFromDisk(absPath);

	// Create new file node
	singleton.fileTable[node] = std::FileNode(&singleton.allocator, fileSize + 1, path);

	// then actually read
	readFileFromDisk(singleton.fileTable[node].fileData, absPath);

	// Return data
	return &singleton.fileTable[node].fileData;
}