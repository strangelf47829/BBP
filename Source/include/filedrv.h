#ifndef BBP_STD_FILEDRV_H
#define BBP_STD_FILEDRV_H

#include "FileSys.h"

namespace BBP
{
	namespace std
	{
		
		// Load the metadata for a path
		void loadFileMetadata(std::PATH &, std::size_t, std::word *);

	}
}

#endif