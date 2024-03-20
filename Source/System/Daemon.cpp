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