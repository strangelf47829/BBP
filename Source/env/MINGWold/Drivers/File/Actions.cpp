#include "../../include/FileDriver.h"
#include "../../include/Environment.h"

BBP::std::word Environment::Drivers::Filesystem::sendDataToFileSystem(BBP::std::size_t, BBP::std::PAGE<BBP::std::string_element> &page)
{
	// Check for existing file

}

BBP::std::word Environment::Drivers::Filesystem::receiveDataFromFileSystem(BBP::std::size_t, BBP::std::PAGE<BBP::std::string_element> &page)
{

}

BBP::std::word Environment::Drivers::Filesystem::receiveFileMetadata(BBP::std::size_t, BBP::std::PAGE<BBP::std::string_element> &)
{

	return 0;
}