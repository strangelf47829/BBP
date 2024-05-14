#ifndef BBP_STD_DRVCMD_H
#define BBP_STD_DRVCMD_H

#include "stddef.h"

namespace BBP
{

	namespace std
	{

		// File system commands
		constexpr std::index_t loadPath = 2;				// Load a path into driver to manipulate later
		constexpr std::index_t queryFileMetadata = 3;		// Query file metadata in this order: (exists?, filesize)
		constexpr std::index_t queryEntityType = 4;			// Query file type (as defined in filesysinfo)
		constexpr std::index_t openFile = 5;				// Open a file to read or write from
		constexpr std::index_t closeFile = 6;				// Close a file
		constexpr std::index_t setModeRead = 7;				// Set mode to reading
		constexpr std::index_t setModeWrite = 8;			// Set mode to writing
		

		

	}

}

#endif