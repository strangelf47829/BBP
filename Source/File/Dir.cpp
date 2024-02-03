/*
#include "../include/FileSys.h"

BBP::std::DIRECTORY &BBP::std::DIRECTORY::root()
{
	// Navigate to root
	return info(this->volume->path.data);
}

void BBP::std::DIRECTORY::hold(std::conststring path)
{
	// Copy string into file descriptor 
	std::size_t nameLength = std::strlen(path);
	this->DirectoryDescriptor.path = std::string(nameLength + 1, (std::string_element *)std::progmem.calloc(nameLength + 1, sizeof(std::string_element)));
	std::strcpy(&this->DirectoryDescriptor.path, path);
}

BBP::std::DIRECTORY &BBP::std::DIRECTORY::info(std::conststring path)
{
	hold(path);
	return info();
}

void BBP::std::DIRECTORY::close()
{
	// Free path name if it exists
	if (this->DirectoryDescriptor.path.data)
		BBP::std::progmem.free(this->DirectoryDescriptor.path.data);

	// If not populated, just return
	if (!this->is_populated)
		return;

	// Close any files
	for (std::index_t index = 0; index < this->DirectoryDescriptor.fileCount; index++)
		this->DirectoryDescriptor.files.data[index].close();

	// Close any directories
	for (std::index_t index = 0; index < this->DirectoryDescriptor.directoryCount; index++)
		this->DirectoryDescriptor.directories.data[index].close();

	// Close file and directory stack
	BBP::std::progmem.free(this->DirectoryDescriptor.directories.data);
	BBP::std::progmem.free(this->DirectoryDescriptor.files.data);
}

void BBP::std::DIRECTORY::addVirtualFile(std::conststring str, std::PAGE<std::mem_t> page)
{
	// Create buffer
	FILE *newBuffer = (FILE *)std::progmem.calloc(DirectoryDescriptor.fileCount + 1, sizeof(FILE));

	// Copy memory
	for (index_t index = 0; index < DirectoryDescriptor.fileCount; index++)
		newBuffer[index] = DirectoryDescriptor.files.data[index];

	// Free buffer and overwrite old buffer
	BBP::std::progmem.free(DirectoryDescriptor.files.data);
	DirectoryDescriptor.files.data = newBuffer;

	// Make new file
	DirectoryDescriptor.files.data[DirectoryDescriptor.fileCount] = std::FILE();
	DirectoryDescriptor.files.data[DirectoryDescriptor.fileCount].FileDescriptor.Vector = page;
	DirectoryDescriptor.files.data[DirectoryDescriptor.fileCount].FileDescriptor.BlockCount = 1;
	DirectoryDescriptor.files.data[DirectoryDescriptor.fileCount].FileDescriptor.BlockSize = std::seqlen(&page);
	DirectoryDescriptor.files.data[DirectoryDescriptor.fileCount].FileDescriptor.fileSize = DirectoryDescriptor.files.data[DirectoryDescriptor.fileCount].FileDescriptor.BlockSize;
	DirectoryDescriptor.files.data[DirectoryDescriptor.fileCount].is_virt = true;

	// Increase file count by one
	DirectoryDescriptor.fileCount++;
}
*/