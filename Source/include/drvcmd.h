#ifndef BBP_STD_DRVCMD_H
#define BBP_STD_DRVCMD_H

#include "stddef.h"
#include "Time.h"

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
		constexpr std::index_t inspectPath = 9;				// Inspect a path (used to count items)
		constexpr std::index_t inspectStep = 10;			// Move to the next path
		constexpr std::index_t inspectEmitName = 11;		// Receive the name of the path
		constexpr std::index_t inspectGetType = 12;			// Get the file type
		constexpr std::index_t inspectCanStep = 13;			// Can step?

		// System commands
		constexpr std::index_t systemCallCmd = 2;			// Do a system call
		constexpr std::index_t initClock = 3;
		constexpr std::index_t getMillis = 4;
		constexpr std::index_t getMicros = 5;

		// Keyboard commands
		constexpr std::index_t getKeyboardKeyCount = 2;
		constexpr std::index_t getKeyboardKeyInput = 3;

		// Screen commands
		constexpr BBP::std::index_t screenClearScreen = 2;
		constexpr BBP::std::index_t screenBIOSSplashScreen = 3;
		

	}

}

#endif