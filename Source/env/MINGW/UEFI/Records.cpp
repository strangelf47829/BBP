#include "../../../include/UEFI.h"
#include "../../../include/Kernel.h"
#include "../include/BIOS.h"
#include "../include/Environment.h"
#include "../../../OS/OSRegistry.h"


// Boot record list
constexpr BBP::std::size_t bootrecordCount = 3;
BBP::system::BootRecord *bootrecord[3] = { nullptr, nullptr, nullptr };

BBP::std::size_t Environment::UEFI::loadBootrecords(BBP::std::PAGE<BBP::system::BootRecord *> &records)
{
	// Set BIOS Record
	bootrecord[0] = &Environment::BIOS::bootRecord;

	// Set Windowse Record
	bootrecord[1] = &Environment::BIOS::Windowse20Record;

	// Set BBP Record
	bootrecord[2] = &Environment::BIOS::BBPRecord;

	// Set records page
	records = BBP::std::PAGE<BBP::system::BootRecord *>(bootrecordCount, bootrecord);
	return bootrecordCount;
}