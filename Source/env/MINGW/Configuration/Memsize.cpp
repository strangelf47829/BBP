#include "../include/Config.h"
#include <sysinfoapi.h>

// Get total system RAM (in kB)
BBP::std::word Host::getSystemMemory()
{
    // Declare info
    _MEMORYSTATUSEX info;

    // Prepare info for function call
    info.dwLength = sizeof(info);

    // Retrieve information. Return 0 if error
    if (GlobalMemoryStatusEx(&info) == false)
        return 0;

    // return data
    return (info.ullTotalPhys / 1024);
}

// Get free system RAM (in KB)
BBP::std::word Host::getSystemHeap()
{
    // Declare info
    _MEMORYSTATUSEX info;

    // Prepare info for function call
    info.dwLength = sizeof(info);

    // Retrieve information. Return 0 if error
    if (GlobalMemoryStatusEx(&info) == false)
        return 0;

    // Return information
    return (info.ullAvailPhys / 1024);
}