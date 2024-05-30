#include "../include/Config.h"
#include <sys/sysinfo.h>
#include <iostream>
#include <string>
#include <vector>
#include <sstream>

BBP::std::errno_t Host::getHostValue(BBP::std::string &str, BBP::std::conststring val)
{
    // Declare a string
    std::string unameOutput;

    // Pipe output of command
    FILE *pipe = popen(val, "r");

    // If opened, read
    if (pipe)
    {
        // Declare a buffer
        char buffer[128];

        // Then, while the file still exists,
        while (!feof(pipe))
        {
            // Read data from the file
            if (fgets(buffer, 128, pipe) != nullptr)
            {
                // And append data to string
                unameOutput += buffer;
            }
        }

        // Close file
        pclose(pipe);

        // Copy string
        for (BBP::std::size_t idx = 0; BBP::std::string_element c : unameOutput)
        {
            // If 'c' is newline, breakout
            if (c == '\n')
                break;

            str[idx] = c;
            str[++idx] = 0;
        }

        // then return success
        return ENONE;
    }

    // Nothing found?
    return EIO;
}

BBP::std::errno_t Host::getHostValue(BBP::std::word &value, BBP::std::conststring val)
{
    // Declare a string
    std::string unameOutput;

    // Pipe output of command
    FILE *pipe = popen(val, "r");

    // If opened, read
    if (pipe)
    {
        // Declare a buffer
        char buffer[128];

        // Then, while the file still exists,
        while (!feof(pipe))
        {
            // Read data from the file
            if (fgets(buffer, 128, pipe) != nullptr)
            {
                // And append data to string
                unameOutput += buffer;
            }
        }

        // Close file
        pclose(pipe);

        // Set
        value = std::stoi(unameOutput);

        // then return success
        return ENONE;
    }

    // Nothing found?
    return EIO;
}