#include "../include/Kernel.h"
#include "../include/Shell.h"

BBP::std::FILE::FILE()
	: node(max_open_files), data(nullptr)
{
	// Empty file, no nothing.
}

BBP::std::FILE::FILE(std::Stack<std::string_element> &Stack, std::conststring path)
	: data(&Stack)
{
	// Get INode info
	std::PATH p(path);
	this->node = system::Kernel::allocateINode(p);

	// Check if it already exists
	_unload_inode = !is_open();

#ifndef BBP_STDLIB_FILESYS_OVERWRITEEXISTINGFILES
	// If BBP_STDLIB_FILESYS_OVERWRITEEXISTINGFILES is set, check if file already exists and throw exception if it does
	if (doesFileExistOnDisk(_path))
		throw std::exception("FILE_ALREADY_EXISTS", EEXIST);
#endif

	// Create file node at corresponding INode
	system::Kernel::openVirtualFile(this->node, Stack, &system::Shell::getPrimaryVolume(), p);
}

BBP::std::FILE::FILE(std::PATH path)
	: data(nullptr)
{
	this->node = system::Kernel::allocateINode(path);

	// Check if file exists on disk
	bool exists = doesFileExistOnDisk(path);

	// If file does not exist on disk or in memory, throw error
	if (!is_open() && !on_disk())
		throw std::exception("ERR_FILE_NOEXIST", ENOENT);

	// Only read from disk if file exists on disk, but is also not already in memory
	if (exists && !is_open())
	{
		// Read data from file
		data = system::Kernel::readFileFromDisk(this->node, path);

		// Set flags
		_dealloc_page = true;
		_unload_inode = true;
	}
}


BBP::std::FILE::FILE(std::conststring _path)
	: FILE(std::PATH(_path))
{
}

void BBP::std::FILE::close()
{
	// Deallocate INode stuff as appropriate
	this->node = system::Kernel::deallocateINode(this->node, _dealloc_page, _unload_inode);

	// Unload
	data = nullptr;
}

bool BBP::std::FILE::is_open()
{
	if (data == nullptr)
		return false;

	return data->page != nullptr;
}

BBP::std::Stack<BBP::std::string_element> &BBP::std::FILE::b()
{
	// If no active data
	if (data == nullptr)
		throw std::exception("ERR_NO_ACTIVEFILE", ENODATA);

	// Return data
	return *data;
}

BBP::std::FILE &BBP::std::FILE::write(std::conststring str)
{
	b() << str;
	return *this;
}

BBP::std::FILE &BBP::std::FILE::write(std::c_string &str)
{
	b() << str;
	return *this;
}

BBP::std::FILE &BBP::std::FILE::write(std::string &str)
{
	b() << str;
	return *this;
}

BBP::std::FILE &BBP::std::FILE::finish(std::conststring str)
{
	b() <<= str;
	return *this;
}

BBP::std::FILE &BBP::std::FILE::finish(std::c_string &str)
{
	b() <<= str;
	return *this;
}

BBP::std::FILE &BBP::std::FILE::finish(std::string &str)
{
	b() <<= str;
	return *this;
}

BBP::std::FILE &BBP::std::FILE::finish()
{
	b() <<= std::endl;
	return *this;
}