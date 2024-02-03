/*
#include "../include/FileSys.h"
#include "../include/stdio.h"

#define _HAS_CXX17
#include <filesystem>
#include <cstdio>


BBP::std::DIRECTORY &BBP::std::DIRECTORY::info()
{
	// Get the iterator for the directory
	auto iterator = ::std::filesystem::directory_iterator(std::read_a(&this->DirectoryDescriptor.path, 0));

	// Count the amount of files
	this->DirectoryDescriptor.fileCount = 0;
	this->DirectoryDescriptor.directoryCount = 0;
	for (auto &p : iterator)
		if (p.is_directory())
			this->DirectoryDescriptor.directoryCount++;
		else
			this->DirectoryDescriptor.fileCount++;

	// Create the file and directory pages
	this->DirectoryDescriptor.files = std::PAGE<std::FILE>(this->DirectoryDescriptor.fileCount, (std::FILE *)std::progmem.calloc(this->DirectoryDescriptor.fileCount, sizeof(std::FILE)));
	this->DirectoryDescriptor.directories = std::PAGE<std::DIRECTORY>(this->DirectoryDescriptor.directoryCount, (std::DIRECTORY *)std::progmem.calloc(this->DirectoryDescriptor.directoryCount, sizeof(std::DIRECTORY)));

	// Re-assign the iterator
	iterator = ::std::filesystem::directory_iterator(std::read_a(&this->DirectoryDescriptor.path, 0));

	// Then load the files
	std::size_t file_index = 0;
	std::size_t dir_index = 0;
	for (auto &p : iterator)
		if (p.is_directory())
		{
			this->DirectoryDescriptor.directories.data[dir_index].hold((char *)p.path().c_str());
			this->DirectoryDescriptor.directories.data[dir_index++].parent = this;
		}
		else
		{
			this->DirectoryDescriptor.files.data[file_index++].hold((char *)p.path().c_str());
		}

	// Set populated flag to true
	is_populated = true;

	return *this;
}

BBP::std::DIRECTORY &BBP::std::DIRECTORY::navigate(std::conststring _path)
{
	// Get target directory
	::std::filesystem::path tgt(this->DirectoryDescriptor.path.data);
	tgt.operator+=("/") += _path;
	std::conststring path = (std::conststring) tgt.c_str();

	// Loop over each directory 
	for (std::index_t index = 0; index < this->DirectoryDescriptor.directoryCount; index++)
	{
		// Get path name
		std::string str = std::read(&this->DirectoryDescriptor.directories, index).DirectoryDescriptor.path;

		// Compare path names
		bool match = std::strcmp(&str, path);

		if (!match)
			continue;

		return this->DirectoryDescriptor.directories.data[index];
	}

	std::STDOUT << "Could not find directory \"" << path << "\"." <<= std::endl;
}

BBP::std::FILE &BBP::std::DIRECTORY::file(std::conststring fileName)
{
	// Get target file
	::std::filesystem::path tgt(this->DirectoryDescriptor.path.data);
	tgt.operator+=("/") += fileName;
	std::conststring path = (std::conststring)tgt.c_str();

	// Loop over each file 
	for (std::index_t index = 0; index < this->DirectoryDescriptor.fileCount; index++)
	{
		// Get path name
		std::string str = std::read(&this->DirectoryDescriptor.files, index).FileDescriptor.fileName;

		printf("Str: %s\n", str.data);

		// Compare path names
		bool match = std::strcmp(&str, path);

		if (!match)
			continue;
		return this->DirectoryDescriptor.files.data[index];
	}

	std::STDOUT << "Could not find directory \"" << path << "\"." <<= std::endl;
}
*/