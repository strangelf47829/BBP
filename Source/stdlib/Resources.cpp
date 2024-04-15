#include "../include/Resources.h"
#include "../include/stdio.h"
#include "../include/Signal.h"
#include "../include/Kernel.h"

// malloc stuff
void *BBP::std::ResourceManager::malloc(std::size_t bytes)
{
	return pageManager.malloc(bytes);
}
void *BBP::std::ResourceManager::calloc(std::size_t bytes, std::size_t amount)
{
	return pageManager.calloc(bytes, amount);
}

// Free stuff
void BBP::std::ResourceManager::free(void *ptr)
{
	return pageManager.free(ptr);
}

// Delete stuff
void BBP::std::ResourceManager::_delete(void *ptr)
{
	return objectManager.Delete(ptr);
}

// Free everything
BBP::std::size_t BBP::std::ResourceManager::freeAll()
{
	return pageManager.freeAll();
}

// Delete everything
BBP::std::size_t BBP::std::ResourceManager::deleteAll()
{
	return objectManager.deleteAll();
}

// Complete clear everything
BBP::std::size_t BBP::std::ResourceManager::clearAll()
{
	return pageManager.freeAll() + objectManager.deleteAll();
}