#include "../include/Config.h"

#include <sysinfoapi.h>

void Host::getCPUData(BBP::system::EFI *uefi)
{
    // Set speed
    uefi->systemReport.processorSpeed = 250000;

    // Get architecture and proc name
    BBP::std::strcpy(uefi->systemReport.processorArch.data, "ESA");
    BBP::std::strcpy(uefi->systemReport.processorName.data, "raincandy");

}