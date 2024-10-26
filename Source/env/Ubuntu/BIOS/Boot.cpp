#include "../../include/Bootloader.h"
#include "../include/BIOS.h"

// The bios
Host::BIOS bios;

// Get 'bios'
BBP::BIOS *Host::getBIOS()
{
	return &bios;
}

// Print something
void Host::BIOS::print(BBP::std::conststring str)
{
	// If str is nullptr, do nothing
	if (str == nullptr)
		return;

	// Otherwise print
	for (BBP::std::index_t idx = 0; str[idx]; idx++)
		printCharacter(str[idx]);
}

// Initialization of BIOS
BBP::std::errno_t Host::BIOS::init()
{
	// Start capturing keyboard
	listenForKeyboard();

	// Start clocks
	initClocks();

	// No errors
	return ENONE;
}

// Deinitialization of BIOS
BBP::std::errno_t Host::BIOS::fini()
{
	// Stop capturing keyboard
	stopListeningForKeyboard();

	// No errors
	return ENONE;
}