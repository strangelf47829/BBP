#include "../../../include/FileSystemDriver.h"
#include "../../../../include/drvcmd.h"
#include "Filedrvhdr.h"


BBP::std::word Host::Drivers::FileSystem::sendDataToFileSystem(BBP::std::size_t amount, BBP::std::PAGE<BBP::std::string_element> &page)
{
	// If in READPATH mode, set path
	switch (mode)
	{
	case FileSystemMode::READPATH:

		// Convert to windows thingy
		for (BBP::std::index_t idx = 0; page[idx]; idx++)
			if (page[idx] == '/')
				page[idx] = '\\';

		// Set file
		activeFile = page;
		return amount;

	case FileSystemMode::READ:
		return 0; // No-op

	case FileSystemMode::WRITE:
		// Pass on data
		writeFile(amount, page);
		return amount;
	}
}

BBP::std::word Host::Drivers::FileSystem::receiveDataFromFileSystem(BBP::std::size_t requested, BBP::std::PAGE<BBP::std::string_element> &page)
{
	// If in READPATH mode, set path
	switch (mode)
	{
	case FileSystemMode::READPATH:
		return 0; // No-op

	case FileSystemMode::WRITE:
		return 0; // No-op

	case FileSystemMode::READ:

		// Read requested bytes
		for (BBP::std::index_t idx = 0; idx < requested; idx++)
		{
			// Allocate character
			char c = 0;

			// Then read.
			bool cycle = readCharacter(page[idx]);

			// If cycle, continue
			if (cycle)
				continue;

			// Return amount of bytes written
			return idx;
		}

		// Read requested amount
		return requested;

	case FileSystemMode::INSPECT:

		// Read the amount of requested bytes into page
		emitName(requested, page);

		// Return amount
		return requested;
	}
}

BBP::std::word Host::Drivers::FileSystem::receiveFileMetadata(BBP::std::size_t, BBP::std::PAGE<BBP::std::string_element> &)
{

	return 0;
}