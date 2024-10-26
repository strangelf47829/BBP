#include "../include/Kernel.h"
#include "../include/Shell.h"

bool BBP::system::Kernel::isFileOpened(std::PATH &path)
{
	// Hash the path
	std::hash_t hash = std::strhsh(path.relName());

	// For each file node,
	for (std::index_t idx = 0; idx < singleton.Core().fileTable.dataSize; idx++)
	{
		// Get hash
		std::hash_t nodeHash = singleton.Core().fileTable[idx].filePathHash;

		// If not equal, continue
		if (nodeHash != hash)
			continue;

		// Otherwise check both against eachother. If not the same, continue
		if (std::strcmp(path.relName(), singleton.Core().fileTable[idx].filePath.relName()) == false)
			continue;

		// Found, set node
		singleton.Core().activeNodeRef = idx;
		return true;
	}

	// Nothing found.
	singleton.Core().activeNodeRef = singleton.Core().fileTable.dataSize;
	return false;
}

bool BBP::system::Kernel::findEmptyNode()
{
	// Go over each filenode
	for (std::index_t idx = 0; idx < singleton.Core().fileTable.dataSize; idx++)
	{
		// If hash is null, return true
		if (singleton.Core().fileTable[idx].filePathHash == 0)
		{
			singleton.Core().activeNodeRef = idx;
			return true;
		}
	}

	// Nothing found.
	singleton.Core().activeNodeRef = singleton.Core().fileTable.dataSize;
	return false;
}

BBP::std::noderef_t BBP::system::Kernel::deallocateINode(std::noderef_t node, bool _dealloc, bool _unload)
{
	// Deallocate the page if marked allocated
	if (_dealloc)
	{
		singleton.Core().allocator.free(singleton.Core().fileTable[node].fileData.page->data);
		singleton.Core().allocator._delete(singleton.Core().fileTable[node].fileData.page);
	}

	// If also needing to unload the page itself, do this
	if (_unload)
		singleton.Core().fileTable[node].fileData.page = nullptr;

	// Return invalid INode
	return singleton.Core().fileTable.dataSize;
}

BBP::std::noderef_t BBP::system::Kernel::allocateINode(std::PATH &path, bool &isOpen)
{
	// Resolve names
	path.makeAbsolutePath(&system::Shell::getWorkingDirectory());

	// Check if file is open
	isOpen = system::Kernel::isFileOpened(path);

	// If not open, set node and exit
	if (isOpen)
		return singleton.Core().activeNodeRef;

	// Otherwise look for empty node
	bool emptyNodeExists = system::Kernel::findEmptyNode();

	// If no empty node exists
	if (emptyNodeExists == false)
		throw std::exception("No Empty Filenode", ENFILE);

	// Otherwise set index
	return singleton.Core().activeNodeRef;
}

BBP::std::PATH &BBP::system::Kernel::getNodePath(std::noderef_t node)
{
	return singleton.Core().fileTable[node].filePath;
}