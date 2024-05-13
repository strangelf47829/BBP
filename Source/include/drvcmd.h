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

		

	}

}

#endif