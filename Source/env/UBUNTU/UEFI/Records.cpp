#include "../../../include/UEFI.h"
#include "../../../include/Kernel.h"
#include "../include/BIOS.h"
#include "../include/Environment.h"



// BBP Record
bool loadIntoBBP() { return true; }
BBP::system::BootRecord BBPRecord(loadIntoBBP, BBP::system::getKernelInstance().getBBPEntryPoint(), "BBP");

// Boot record list
constexpr BBP::std::size_t bootrecordCount = 2;
BBP::system::BootRecord *bootrecord[2] = {&Environment::BIOS::bootRecord, &BBPRecord};

BBP::std::size_t Environment::UEFI::loadBootrecords(BBP::std::PAGE<BBP::system::BootRecord*> &records)
{
	// Set records page
	bootrecord[1] = &BBPRecord;
	records = BBP::std::PAGE<BBP::system::BootRecord*>(bootrecordCount, bootrecord);

	return bootrecordCount;
}