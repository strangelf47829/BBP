#include "../include/EFIDriverInterface.h"
#include "../../include/EFIConfig.h"

// Active
BBP::system::EFI *active = nullptr;

// Set active (only valid once)
void Host::setActiveInterface(BBP::system::EFI &efi)
{
	// If already has active, do nothing
	if (active)
		return;

	// Else set
	active = &efi;
}

// Get Unix timestamp at startup
void Host::getStartupUnixTimestamp(BBP::std::word &lower, BBP::std::word &upper)
{
	// If no active, set to 0 and do nothing
	if (active == nullptr)
	{
		lower = 0;
		upper = 0;
		return;
	}

	// Otherwise set values, with 'upper' getting set first
	upper = active->systemReport.epochUpper;
	lower = active->systemReport.epochLower;
}