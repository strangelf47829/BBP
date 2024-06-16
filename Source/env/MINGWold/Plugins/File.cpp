#include "../../../include/FileSys.h"

#include <fstream>
#include <iostream>

bool BBP::std::FILE::doesFileExistOnDisk(BBP::std::PATH &_path)
{
	// Open fstream
	::std::ifstream _file(_path.relName());

	// Store file isopen
	_is_on_disk = _file.is_open();

	// Close file
	_file.close();

	return _is_on_disk;
}

BBP::std::size_t BBP::std::FILE::getFileSizeFromDisk(BBP::std::PATH &_path)
{
	// Open fstream
	::std::ifstream _file(_path.relName());

	// Check if file is open
	if (!_file.is_open())
		throw std::exception("ERR_FILE_NOEXIST", ENOENT);

	// Get bytecount
	_file.seekg(0, ::std::ios::end);
	BBP::std::size_t size = _file.tellg();
	_file.seekg(0, ::std::ios::beg);

	// Close file
	_file.close();

	// Return bytecount
	return size;
}

BBP::std::size_t BBP::std::FILE::readFileFromDisk(BBP::std::FileNode *n, BBP::std::PATH &_path)
{
	// Open fstream
	::std::ifstream _file(_path.relName(), ::std::ios::binary);

	// Check if file is open
	if (!_file.is_open())
		throw std::exception("ERR_FILE_NOEXIST", ENOENT);

	// Get bytecount 
	_file.seekg(0, ::std::ios::end);
	BBP::std::size_t size = _file.tellg();
	_file.seekg(0, ::std::ios::beg);

	// Read from file, and put into n.data
	char next;
	while (_file.get(next))
		n->fileData << next;

	// Close file
	_file.close();

	// Set _is_on_disk
	_is_on_disk = true;

	// Return byte count
	return size;
}

void BBP::std::FILE::writeFileToDisk(std::PATH &path)
{
	// open ofstream
	::std::ofstream file(path.relName(), ::std::ios::out | ::std::ios::binary);

	// Get data length
	std::string *_data = data()->fileData.page;
	std::size_t dataSize = std::seqlen(*_data);

	// Write data into file stream
	for (std::index_t idx = 0; idx < dataSize; idx++)
		file << __UNSAFE__(read)(_data, idx);

	// Close file
	file.close();
}

void BBP::std::FILE::writeFileToDisk()
{
	writeFileToDisk(data()->filePath);
}



/*
#include "../include/FileSys.h"

#include <fstream>
#include <iostream>

void BBP::std::FILE::hold(std::conststring fileName)
{
	// Copy string into file descriptor
	std::size_t nameLength = std::strlen(fileName);
	this->FileDescriptor.fileName = std::string(nameLength + 1, (std::string_element *)std::progmem.calloc(nameLength + 1, sizeof(std::string_element)));
	std::strcpy(&this->FileDescriptor.fileName, fileName);
}

void BBP::std::FILE::open()
{
	this->open(std::read_a(&this->FileDescriptor.fileName, 0));
}

void BBP::std::FILE::open(int mode)
{
	this->open(std::read_a(&this->FileDescriptor.fileName, 0), mode);
}

void BBP::std::FILE::open(std::conststring fileName)
{
	// Hold filename
	this->hold(fileName);

	// Read file
	::std::ifstream input = ::std::ifstream(fileName);

	// Check if open
	if (!input.is_open())
	{
		printf("Input: %d\n", input.is_open());
		// File is not open, set sizes to 0
		this->FileDescriptor.BlockCount = 0;
		this->FileDescriptor.BlockSize = 0;
		this->FileDescriptor.fileSize = 0;

		// Set is_open to false, and close file
		this->is_open = false;
		input.close();

		return;
	}

	// Set Block count
	this->FileDescriptor.BlockSize = std::max_page_elements;

	// Get bytecount
	input.seekg(0, ::std::ios::end);
	this->FileDescriptor.fileSize = input.tellg();
	input.seekg(0, ::std::ios::beg);

	// Calculate amount of blocks needed
	this->FileDescriptor.BlockCount = 0;
	for (std::size_t size = this->FileDescriptor.fileSize; size >= this->FileDescriptor.BlockSize; size -= this->FileDescriptor.BlockSize)
		this->FileDescriptor.BlockCount++;
	this->FileDescriptor.BlockCount++;

	// Allocate first block in memory
	this->FileDescriptor.Vector = std::PAGE<std::mem_t>(this->FileDescriptor.BlockSize, (std::mem_t *)std::progmem.calloc(this->FileDescriptor.BlockSize, sizeof(std::mem_t)));

	// Then allocate consequent blocks
	std::PAGE<mem_t> *previous = &this->FileDescriptor.Vector;
	for (std::size_t block = 1; block < this->FileDescriptor.BlockCount; block++)
	{
		// Allocate page
		std::PAGE<mem_t> *page = new std::PAGE<mem_t>(this->FileDescriptor.BlockSize, (std::mem_t *)std::progmem.calloc(this->FileDescriptor.BlockSize, sizeof(std::mem_t)));
		std::progmem.add_object((void *)page);

		// Link this and previous page
		page->prevPage = previous;
		previous->nextPage = page;
	}

	// Read actual bytes
	char next;
	int index = 0;
	while (input.get(next))
		__UNSAFE__(std::write)(&this->FileDescriptor.Vector, (std::mem_t)next, index++);

	// Update flags
	this->is_open = true;

	// Close file
	input.close();
}

void BBP::std::FILE::open(std::conststring fileName, int mode)
{
	// Hold filename
	this->hold(fileName);

	// Read file
	::std::ifstream input = ::std::ifstream(fileName, (::std::ios_base::openmode)mode);

	// Check if open
	if (!input.is_open())
	{
		printf("Input: %d\n", input.is_open());
		// File is not open, set sizes to 0
		this->FileDescriptor.BlockCount = 0;
		this->FileDescriptor.BlockSize = 0;
		this->FileDescriptor.fileSize = 0;

		// Set is_open to false, and close file
		this->is_open = false;
		input.close();

		return;
	}

	// Set Block count
	this->FileDescriptor.BlockSize = std::max_page_elements;

	// Get bytecount
	input.seekg(0, ::std::ios::end);
	this->FileDescriptor.fileSize = input.tellg();
	input.seekg(0, ::std::ios::beg);

	// Calculate amount of blocks needed
	this->FileDescriptor.BlockCount = 0;
	for (std::size_t size = this->FileDescriptor.fileSize; size >= this->FileDescriptor.BlockSize; size -= this->FileDescriptor.BlockSize)
		this->FileDescriptor.BlockCount++;
	this->FileDescriptor.BlockCount++;

	// Allocate first block in memory
	this->FileDescriptor.Vector = std::PAGE<std::mem_t>(this->FileDescriptor.BlockSize, (std::mem_t *)std::progmem.calloc(this->FileDescriptor.BlockSize, sizeof(std::mem_t)));

	// Then allocate consequent blocks
	std::PAGE<mem_t> *previous = &this->FileDescriptor.Vector;
	for (std::size_t block = 1; block < this->FileDescriptor.BlockCount; block++)
	{
		// Allocate page
		std::PAGE<mem_t> *page = new std::PAGE<mem_t>(this->FileDescriptor.BlockSize, (std::mem_t *)std::progmem.calloc(this->FileDescriptor.BlockSize, sizeof(std::mem_t)));
		std::progmem.add_object((void *)page);

		// Link this and previous page
		page->prevPage = previous;
		previous->nextPage = page;
	}

	// Read actual bytes
	char next;
	int index = 0;
	while (input.get(next))
		__UNSAFE__(std::write)(&this->FileDescriptor.Vector, (std::mem_t)next, index++);

	// Update flags
	this->is_open = true;

	// Close file
	input.close();
}


void BBP::std::FILE::close()
{
	// Free file name, if it exists
	if (this->FileDescriptor.fileName.data)
		std::progmem.free(this->FileDescriptor.fileName.data);

	// If file is not even open in the first place, don't close
	if (!this->is_open)
		return;

	// If file is virtual, do not close
	if (this->is_virt)
		return;

	// Since 'Vector' is statically allocated, do not free it
	// But instead free data
	std::progmem.free(this->FileDescriptor.Vector.data);

	// Then free pages and data within
	std::PAGE<std::mem_t> *page = &this->FileDescriptor.Vector;
	for (std::size_t pageIndex = 1; pageIndex < this->FileDescriptor.BlockCount; pageIndex++)
	{
		// If page is null, break.
		if (page == nullptr)
			break;

		// Get next page
		std::PAGE<std::mem_t> *nextPage = page->nextPage;

		// Free data
		std::progmem.free(page->data);

		// Mark deleted
		std::progmem.mark_deleted((void *)page);

		// Delete page
		delete(page);

		// Set page to nextPage
		page = nextPage;
	}

	// Set size to 0
	this->FileDescriptor.BlockCount = 0;
	this->FileDescriptor.fileSize = 0;
}
*/