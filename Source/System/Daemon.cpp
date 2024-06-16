#include "../include/Daemon.h"

// Static record stuff
BBP::std::STATIC_PAGE<BBP::system::DaemonRecord, 10> static_records = BBP::std::STATIC_PAGE<BBP::system::DaemonRecord, 10>();
BBP::std::PAGE<BBP::system::DaemonRecord> BBP::system::DaemonRecord::records = std::PAGE<system::DaemonRecord>(10, static_records.static_data);

BBP::system::DaemonRecord::DaemonRecord(system::Daemon *daemonPtr, std::conststring str)
{
	// Set owner
	this->owner = daemonPtr;

	// Set name if it exists
	if (str)
		std::strcpy(this->daemonName.data, str);
	else
		std::strcpy(this->daemonName.data, "(null)");
}

BBP::system::DaemonRecord::DaemonRecord()
{

}

bool BBP::system::DaemonRecord::executeService(std::string name, std::string service, std::errno_t &errno, std::size_t argc, std::c_string *argv)
{

	// Look through each record
	for (std::index_t idx = 0; idx < records.dataSize; idx++)
	{
		// If hashes don't match don't search
		if (records[idx].daemonName != name)
			continue;

		// Found something, look up functor
		DaemonService *serviceRecord = records[idx].owner->lookupFunctor(service);

		// If service not available do nothing
		if (serviceRecord == nullptr)
			return false; // Not executed

		// Otherwise execute
		errno = records[idx].owner->functorFunctor(serviceRecord->hash, argc, argv);

		// Return success
		return true;
	}
	
	// Return error: not found
	return false;
}