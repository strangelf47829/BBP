#ifndef HOST_FILEDRVHDR_H
#define HOST_FILEDRVHDR_H

#include "../../../../include/DeviceDriver.h"
#include "../../../../include/drvcmd.h"
#include "../../../../include/FileSysInfo.h"
#include "../../../include/FileSystemDriver.h"

namespace Host
{
	namespace Drivers
	{
		namespace FileSystem
		{
			// Actual driver stuff
			bool doesPathExist();
			BBP::std::size_t getFileSize();

			// Entity type checking
			bool isFile();
			bool isDirectory();
			bool doesEntityExist();

			// File reading
			void openFileReading();
			void closeFileReading();
			bool readCharacter(char &);

			// File writing
			void openFileWriting();
			void closeFileWriting();
			void writeFile(BBP::std::size_t, BBP::std::PAGE<BBP::std::string_element> &);

			// Inspection
			void openInspection();
			bool stepInspection();
			BBP::std::size_t nameSizeInspection();
			int getEntityTypeInspection();
			BBP::std::size_t prepareNameEmission();
			void emitName(BBP::std::size_t, BBP::std::PAGE<BBP::std::string_element> &);
			bool inspectorStatus();

			// Mode type
			enum FileSystemMode
			{
				READPATH,
				READ, WRITE,
				INSPECT
			};

			// Current mode
			extern FileSystemMode mode;

			// Path to currently active file
			extern BBP::std::PATH activeFile;
		}
	}
}

#endif