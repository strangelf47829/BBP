#ifndef ENV_DRIVERS_FILEDRIVER_H
#define ENV_DRIVERS_FILEDRIVER_H

#include "../../include/DeviceDriver.h"
#include "../../include/drvcmd.h"

namespace Host
{
	namespace Drivers
	{
		namespace FileSystem
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
			bool queryFileMetadata(BBP::std::size_t, BBP::std::word *);				// Query
			bool getFileType(BBP::std::size_t, BBP::std::word *);					// queryEntityType
			bool openFile(BBP::std::size_t, BBP::std::word *);						// open a file
			bool closeFile(BBP::std::size_t, BBP::std::word *);						// close a file
			bool setReadMode(BBP::std::size_t, BBP::std::word *);					// Set read mode
			bool setWriteMode(BBP::std::size_t, BBP::std::word *);					// Set read mode
			bool pathInspect(BBP::std::size_t, BBP::std::word *);					// Inspect a path
			bool stepInspect(BBP::std::size_t, BBP::std::word *);					// Step over path
			bool emitNameInspect(BBP::std::size_t, BBP::std::word *);				// Emit name of an inspection
			bool emitTypeInspect(BBP::std::size_t, BBP::std::word *);				// Emit type of an inspection
			bool canStepInspect(BBP::std::size_t, BBP::std::word *);				// Check if able to step

		}
	}
}

#endif