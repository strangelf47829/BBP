#include "../include/BIOS.h"
#include "../include/Config.h"
#include "../../../OS/OSRegistry.h"

// Boot record list
constexpr BBP::std::size_t bootrecordCount = 2;
BBP::system::BootRecord *bootrecord[bootrecordCount] = { nullptr, nullptr };

BBP::std::size_t Host::loadBootrecords(BBP::std::PAGE<BBP::system::BootRecord *> &records)
{
	// Set Windowse Record
	bootrecord[0] = &OS::Windowse20Record;

	// Set BBP Record
	bootrecord[1] = &OS::BBPRecord;

	// Set records page
	records = BBP::std::PAGE<BBP::system::BootRecord *>(bootrecordCount, bootrecord);
	return bootrecordCount;
}