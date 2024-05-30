#include "../include/BIOS.h"
#include "../../../include/stdio.h"

// BIOS Boot record
BBP::system::BootRecord Environment::BIOS::bootRecord = BBP::system::BootRecord(Environment::BIOS::canBootIntoBIOS, Environment::BIOS::EntryPoint, "BIOS");

bool Environment::BIOS::canBootIntoBIOS(BBP::system::EFI *)
{
	return true;
}

bool Environment::BIOS::EntryPoint(BBP::system::EFI *EFI, BBP::std::TaskPool *&)
{
	// Show info
	BBP::std::printf("BIOS version 0.0.0(1) for UBUNTU.\n");


	return true;
}