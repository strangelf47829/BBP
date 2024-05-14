#include "../../include/FileDriver.h"
#include "../../include/Environment.h"
#include "../../../../include/drvcmd.h"



BBP::std::word Environment::Drivers::Filesystem::sendDataToFileSystem(BBP::std::size_t amount, BBP::std::PAGE<BBP::std::string_element> &page)
{
	// If in READPATH mode, set path
	switch(mode)
	{
	case FileSystemMode::READPATH:
		activeFile = page;
		return amount;

	case FileSystemMode::READ:
		return 0; // No-op
		
	case FileSystemMode::WRITE:
		// TODO: Implement
		return 0;
	}
}

BBP::std::word Environment::Drivers::Filesystem::receiveDataFromFileSystem(BBP::std::size_t requested, BBP::std::PAGE<BBP::std::string_element> &page)
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
	}
}

BBP::std::word Environment::Drivers::Filesystem::receiveFileMetadata(BBP::std::size_t, BBP::std::PAGE<BBP::std::string_element> &)
{

	return 0;
}