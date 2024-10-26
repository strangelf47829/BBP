#include "../include/Config.h"

#include <sys/sysinfo.h>

// Get total system RAM
BBP::std::word Host::getSystemMemory()
{
    // Declare info
    struct sysinfo info;

    // Get info
    if (sysinfo(&info) == 0)
        return info.mem_unit * (info.totalram / 1024);

    // Return error
    return 0;
}

// Get free system RAM
BBP::std::word Host::getSystemHeap()
{
    // Declare info
    struct sysinfo info;

    // Get info
    if (sysinfo(&info) == 0)
        return info.mem_unit * (info.freeram / 1024);

    // Return error
    return 0;
}