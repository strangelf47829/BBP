#ifndef BBP_STD_STDDRV_H
#define BBP_STD_DTDDRV_H

// This file defines functions used to interface with drivers.
// This allows for standard functions to interface with drivers at a higher level.

// printf -> (print this string to this output stream) -> (interface with the driver) -> (driver logic)
// getKey -> (get a single character from the stream) -> (interface with the driver) -> (driver logic)

#include "FileSys.h"
#include "FileSysInfo.h"
#include "cstdio.h"

// Screen interface
namespace BBP
{
	namespace std
	{

	}
}

// File interface
namespace BBP
{
	namespace std
	{

		// Query if file is on disk or not
		bool isFileOnDisk(PATH &);

		// Query if path is of type file
		bool isPathOfTypeFile(PATH &);
		
		// Query if path is of type directory
		bool isPathOfTypeDirectory(PATH &);

		// Does anything even exist at path location
		bool isPathRealObject(PATH &);

		// Query file size for file on disk
		std::size_t getFilesizeFromDisk(PATH &);

		// Read file from disk (this is to stream the entire contents of a file into memory and then dump it)
		std::size_t readFileFromDisk(stream_t, PATH &);

		// Write file to disk (Takes all the data from the file and dumps it into a file driver)
		void writeFileToDisk(PATH &, fstream_t);

		// Inspect a given path. Returns 0 if success, 
		std::errno_t Inspect(PATH &);

		// Get file and directory count for a given path
		std::size_t fileCount();
		std::size_t directoryCount();

		// Get file name given an index
		std::c_string getFileName(std::index_t);

		// Get directory name given an index
		std::c_string getDirectoryName(std::index_t);

	}
}

#endif
