#ifndef HOST_CONFIG_H
#define HOST_CONFIG_H

//
#include "../../../include/Strings.h"
#include "../../../include/EFI.h"

namespace Host
{

	// Get device name
	BBP::std::errno_t getHostValue(BBP::std::string &, BBP::std::conststring);
	BBP::std::errno_t getHostValue(BBP::std::word &, BBP::std::conststring);

	// Get heap and system memory
	BBP::std::word getSystemMemory();
	BBP::std::word getSystemHeap();

	// Get CPU data
	void getCPUData(BBP::system::EFI *);

	// Load bootrecord data
	BBP::std::size_t loadBootrecords(BBP::std::PAGE<BBP::system::BootRecord *> &records);


}

#endif