#ifndef BBP_FILE_H
#define BBP_FILE_H

#include "../../../include/FileSys.h"
#include "../../../include/EFI.h"

namespace Environment
{
	namespace Drivers
	{
		namespace Filesystem
		{

			// File specific stuff
			BBP::std::word sendDataToFileSystem(BBP::std::size_t, BBP::std::PAGE<BBP::std::string_element> &);		// Writes data from page into file
			BBP::std::word receiveDataFromFileSystem(BBP::std::size_t, BBP::std::PAGE<BBP::std::string_element> &);	// Reads data from file into page
			BBP::std::word receiveFileMetadata(BBP::std::size_t, BBP::std::PAGE<BBP::std::string_element> &);		// Currently does nothing

			// Load driver
			void loadFileSystem(BBP::system::DeviceDriver &driver);

			// File commands
			bool initializeFilesystem(BBP::std::size_t, BBP::std::word *);			// 0
			bool deinitializeFilesystem(BBP::std::size_t, BBP::std::word *);		// 1

			// File specific commands
			bool setModeLoadPath(BBP::std::size_t, BBP::std::word *);				// loadPath
			bool queryFileMetadata(BBP::std::size_t, BBP::std::word *);

			extern BBP::std::PATH activeFile;
		}
	}
}

#endif
