#ifndef BBP_STD_FILEDRV_H
#define BBP_STD_FILEDRV_H

#include "FileSys.h"
#include "FileSysInfo.h"

namespace BBP
{
	namespace std
	{
		
		// Load a path
		void setPath(std::PATH &);

		// Load the metadata for a path
		void loadFileMetadata(std::PATH &, std::size_t, std::word *);

		// Load the file type of a path
		FileSysInfo::FileSysEntryType queryFileType(std::PATH &);

	}
}

#endif