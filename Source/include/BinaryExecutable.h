#ifndef BBP_LIB_BINARYEXEC
#define BBP_LIB_BINARYEXEC

// This header defines what RAE actually loads into memory.

#include "ELF.h"

namespace BBP
{
	namespace userspace
	{

		// Tracks what memory goes where
		struct MemoryRegion
		{
			// Read-write-execute permissions
			bool readable;
			bool writeable;
			bool executable;

			// Virtual and physical memory pointer
			std::address_t virtualMemoryHead;
			std::address_t physicalMemoryHead;

			// Size of this block
			std::size_t size;

			// Check if allocated
			bool allocated;
		};

		// A generic executable
		class BinaryExecutable
		{

			// Where this binary was allocated
			std::ResourceManager *allocator;

			// Is anything even allocated?
			bool allocated;

			// A copy of the TLS Data
			std::PAGE<std::string_element> TLSData;

		public:

			// Functions to get index of virtual page
			std::index_t findIndexOfVirtualMemory(std::address_t);
			std::index_t findIndexOfPhysicalMemory(std::address_t);
			
			// The binary file as is
			std::PAGE<std::string_element> BinaryData;
			std::size_t totalSize;

			// Memory mappings
			std::STATIC_PAGE<MemoryRegion, 8> mapping;

			// Load an executable into memory
			void loadExecutable(std::ELF::ELFBuilder &data, std::ResourceManager *allocator);

			// Unload an executable from memory
			void unloadExecutable();

			// Get a mapping from virtual to physical memory
			std::address_t virtualToPhysical(std::address_t);
			std::address_t physicalToVirtual(std::address_t);

			// Also retrieve mapping but do so with a cached index
			std::address_t virtualToPhysical(std::address_t, std::index_t);
			std::address_t physicalToVirtual(std::address_t, std::index_t);

			// Create a function to copy over TLS Data
			void createTLS(std::PAGE<std::string_element> &);
			void loadTLS(std::PAGE<std::string_element> &);
			void unloadTLS(std::PAGE<std::string_element> &);

		};
	}
}

#endif