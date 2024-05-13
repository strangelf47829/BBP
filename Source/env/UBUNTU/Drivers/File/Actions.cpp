#include "../../include/FileDriver.h"
#include "../../include/Environment.h"
#include "../../../../include/drvcmd.h"



BBP::std::word Environment::Drivers::Filesystem::sendDataToFileSystem(BBP::std::size_t amount, BBP::std::PAGE<BBP::std::string_element> &page)
{
	// If in READPATH mode, set path
	if (mode == READPATH)
	{
		activeFile = page;
		return amount;
	}
}

BBP::std::word Environment::Drivers::Filesystem::receiveDataFromFileSystem(BBP::std::size_t, BBP::std::PAGE<BBP::std::string_element> &page)
{

}

BBP::std::word Environment::Drivers::Filesystem::receiveFileMetadata(BBP::std::size_t, BBP::std::PAGE<BBP::std::string_element> &)
{

	return 0;
}