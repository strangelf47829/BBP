#include "../include/Kernel.h"
#include "../include/Shell.h"

bool BBP::system::Kernel::isFileOpened(std::PATH &path)
{
	// Hash the path
	std::hash_t hash = std::strhsh(path.relName());

	// For each file node,
	for (std::index_t idx = 0; idx < singleton.fileTable.dataSize; idx++)
	{
		// Get hash
		std::hash_t nodeHash = singleton.fileTable[idx].filePathHash;

		// If not equal, continue
		if (nodeHash != hash)
			continue;

		// Otherwise check both against eachother
		if (std::strcmp(path.relName(), singleton.fileTable[idx].filePath.relName()))
			continue;

		// Found, set node
		singleton.activeNodeRef = idx;
		return true;
	}

	// Nothing found.
	singleton.activeNodeRef = singleton.fileTable.dataSize;
	return false;
}

bool BBP::system::Kernel::findEmptyNode()
{
	// Go over each filenode
	for (std::index_t idx = 0; idx < singleton.fileTable.dataSize; idx++)
	{
		// If hash is null, return true
		if (singleton.fileTable[idx].filePathHash == 0)
		{
			singleton.activeNodeRef = idx;
			return true;
		}
	}

	// Nothing found.
	singleton.activeNodeRef = singleton.fileTable.dataSize;
	return false;
}

BBP::std::noderef_t BBP::system::Kernel::deallocateINode(std::noderef_t node, bool _dealloc, bool _unload)
{
	// Deallocate the page if marked allocated
	if (_dealloc)
	{
		singleton.allocator.free(singleton.fileTable[node].fileData.page->data);
		singleton.allocator._delete(singleton.fileTable[node].fileData.page);
	}

	// If also needing to unload the page itself, do this
	if (_unload)
		singleton.fileTable[node].fileData.page = nullptr;

	// Return invalid INode
	return singleton.fileTable.dataSize;
}

BBP::std::noderef_t BBP::system::Kernel::allocateINode(std::PATH &path)
{
	// Resolve names
	path.makeAbsolutePath(&system::Shell::getWorkingDirectory());

	// Check if file is open
	bool isOpen = system::Kernel::isFileOpened(path);

	// If not open, set node and exit
	if (isOpen)
		return singleton.activeNodeRef;

	// Otherwise look for empty node
	bool emptyNodeExists = system::Kernel::findEmptyNode();

	// If no empty node exists
	if (emptyNodeExists == false)
		throw std::exception("No Empty Filenode", ENFILE);

	// Otherwise set index
	return singleton.activeNodeRef;
}

BBP::std::PATH &BBP::system::Kernel::getNodePath(std::noderef_t node)
{
	return singleton.fileTable[node].filePath;
}