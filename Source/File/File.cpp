#include "../include/Kernel.h"
#include "../include/stdio.h"

BBP::std::FILE::FILE()
	: node(max_open_files)
{
	// Empty file, no nothing.
}

BBP::std::FILE::FILE(std::Stack<std::string_element> &data, std::conststring path)
{
	// Get INode info
	std::PATH p(path);
	getINodeIndex(p);

	// Check if it already exists
	_unload_inode = !is_open();

#ifndef BBP_STDLIB_FILESYS_OVERWRITEEXISTINGFILES
	// If BBP_STDLIB_FILESYS_OVERWRITEEXISTINGFILES is set, check if file already exists and throw exception if it does
	if (doesFileExistOnDisk(_path))
		throw std::exception("FILE_ALREADY_EXISTS", EEXIST);
#endif

	// Create file node at corresponding INode
	system::kernelSS()->activeContext->fileTable.data[this->node] = BBP::std::FileNode(data, BBP::system::kernelSS()->activeContext->primaryVolume, path);
}

void BBP::std::FILE::getINodeIndex(std::PATH &path)
{
	// Resolve names
	path.makeAbsolutePath(system::kernelSS()->activeContext->workingDirectory);

	// Check if file is open
	BBP::std::index_t idx = checkIfIsOpen(path.relName());

	if (idx == system::kernelSS()->activeContext->fileTable.dataSize)
	{
		// Look for next empty file
		std::index_t emptyIndex = findClosedFile();

		// Check if there are empty files
		if (emptyIndex == system::kernelSS()->activeContext->fileTable.dataSize)
			throw std::exception("ERR_NO_EMPTYFILENODE", ENFILE);

		// Save node index
		this->node = emptyIndex;
	}
	else
	{
		this->node = idx;
	}
}

BBP::std::FILE::FILE(std::PATH path)
{
	getINodeIndex(path);

	// Check if file exists on disk
	bool exists = doesFileExistOnDisk(path);

	// If file does not exist on disk or in memory, throw error
	if (!is_open() && !on_disk())
		throw std::exception("ERR_FILE_NOEXIST", ENOENT);

	// Only read from disk if file exists on disk, but is also not already in memory
	if (exists && !is_open())
	{
		// Read file Size
		std::size_t fileSize = getFileSizeFromDisk(path);

		// Create new node
		system::kernelSS()->activeContext->fileTable.data[this->node] = std::FileNode(system::kernelSS()->activeContext->activemem, fileSize + 1, path);
		_dealloc_page = true;
		_unload_inode = true;

		// Read data from file
		readFileFromDisk(&system::kernelSS()->activeContext->fileTable.data[this->node], path);
	}

	system::kernelSS()->activeContext->fileTable.data[this->node].fileData.page->prevPage = nullptr;
	system::kernelSS()->activeContext->fileTable.data[this->node].fileData.page->nextPage = nullptr;

}


BBP::std::FILE::FILE(std::conststring _path)
	: FILE(std::PATH(_path))
{
}

BBP::std::index_t BBP::std::FILE::checkIfIsOpen(std::conststring path)
{
	// Get path hash
	std::hash_t hash = std::strhsh(path);

	// Fetch each table hash
	for (std::index_t idx = 0; idx < system::kernelSS()->activeContext->fileTable.dataSize; idx++)
	{
		// Get reference to node
		std::FileNode *node = &__UNSAFE__(read)(&system::kernelSS()->activeContext->fileTable, idx);

		// If entry is nullptr, skip
		if (node == nullptr)
			continue;

		// Get hash
		std::hash_t file_hash = node->filePathHash;

		if (hash == file_hash)
			return idx;
	}

	// Nothing found, return false.
	return system::kernelSS()->activeContext->fileTable.dataSize;
}

BBP::std::index_t BBP::std::FILE::findClosedFile()
{
	// Fetch each table hash
	for (std::index_t idx = 0; idx < system::kernelSS()->activeContext->fileTable.dataSize; idx++)
	{
		// Get reference to node
		std::FileNode *node = &__UNSAFE__(read)(&system::kernelSS()->activeContext->fileTable, idx);

		// If entry is nullptr, skip
		if (node == nullptr)
			return idx;

		// Get hash
		std::hash_t file_hash = node->filePathHash;

		if (file_hash == 0)
			return idx;
	}

	// Nothing found, return false.
	return system::kernelSS()->activeContext->fileTable.dataSize;
}

BBP::std::FileNode *BBP::std::FILE::data()
{
	return &std::read(&system::kernelSS()->activeContext->fileTable, this->node);
}

void BBP::std::FILE::close()
{
	// Deallocate the page if marked allocated
	if (_dealloc_page)
	{
		BBP::system::kernelSS()->activeContext->activemem->free(data()->fileData.page->data);
		BBP::system::kernelSS()->activeContext->activemem->_delete(data()->fileData.page);
	}

	if (_unload_inode)
		BBP::system::kernelSS()->activeContext->fileTable.data[this->node].fileData.page = nullptr;

	this->node = BBP::system::kernelSS()->activeContext->fileTable.dataSize;
}

bool BBP::std::FILE::is_open()
{
	if (data() == nullptr)
		return false;
	return BBP::system::kernelSS()->activeContext->fileTable.data[this->node].fileData.page != nullptr;
}

BBP::std::Stack<BBP::std::string_element> &BBP::std::FILE::b()
{
	return data()->fileData;
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