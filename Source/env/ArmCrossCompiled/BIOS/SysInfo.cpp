#include "../include/SysInfo.h"
#include <sys/sysinfo.h>
#include <iostream>
#include <string>
#include <vector>
#include <sstream>

// Get total system RAM
BBP::std::size_t Environment::BIOS::totalSystemMemory()
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
BBP::std::size_t Environment::BIOS::totalFreeMemory()
{
    // Declare info
    struct sysinfo info;

    // Get info
    if (sysinfo(&info) == 0)
        return info.mem_unit * (info.freeram / 1024);

    // Return error
    return 0;
}

// Get Cols
BBP::std::halfword Environment::BIOS::getDisplayCols()
{
    // Declare a string
    std::string tputOutput;

    // Pipe output of 'tput' into 'pipe'
    FILE *pipe = popen("tput cols", "r");

    // If opened,
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
                tputOutput += buffer;
            }
        }

        // Close file
        pclose(pipe);

        // then return number
        return std::stoi(tputOutput);
    }

    // Nothing found?
    return 0;
}

// Get Lines
BBP::std::halfword Environment::BIOS::getDisplayLines()
{
    // Declare a string
    std::string tputOutput;

    // Pipe output of 'tput' into 'pipe'
    FILE *pipe = popen("tput lines", "r");

    // If opened,
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
                tputOutput += buffer;
            }
        }

        // Close file
        pclose(pipe);

        // then return number
        return std::stoi(tputOutput);
    }

    // Nothing found?
    return 0;
}

BBP::std::word Environment::BIOS::getCPUData(BBP::system::EFI *EFI)
{
    // Declare a string
    std::string lscpuOutput;

    // Pipe output of 'lscpu' into 'pipe'
    FILE *pipe = popen("lscpu", "r");

    // If pipe opened successfully,
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
                lscpuOutput += buffer;
            }
        }

        // Close file
        pclose(pipe);

        // Declare lines object
        std::vector<std::string> lines;

        // Declare token
        std::string token;

        // Create istringstream
        std::istringstream tokenStream(lscpuOutput);

        while (std::getline(tokenStream, token, '\n')) {
            lines.push_back(token);
        }

        // Then go over each line
        for (const std::string &line : lines)
        {
            // If the line starts with 'CPU MHZ:' 
            if (line.find("CPU MHz:") != std::string::npos)
            {
                // Move to
                size_t pos = line.find(":");

                // If not end of position
                if (pos != std::string::npos)
                {
                    // Get beginning of number
                    std::size_t offset = 1;

                    // While line doesn't begin with number,
                    while (line.at(pos + offset) == ' ')
                        offset++;

                    // Parse clock speed
                    std::string clockSpeed = line.substr(pos + offset);

                    // Convert to number (double for now)
                    double clockSpeedNum = std::stod(clockSpeed);

                    // Then write into EFI
                    EFI->systemReport.processorSpeed = clockSpeedNum;
                }
            }

            // Otherwise, if line starts with 'Architecture'
            if (line.find("Architecture:") != std::string::npos)
            {
                // Move to
                size_t pos = line.find(":");

                // If not end of position
                if (pos != std::string::npos)
                {
                    // Get beginning of architecture
                    std::size_t offset = 1;

                    // While line doesn't begin,
                    while (line.at(pos + offset) == ' ')
                        offset++;

                    // Parse Architecture
                    std::string architecture = line.substr(pos + offset);

                    // Copy over text
                    for (BBP::std::index_t idx = 0; auto c: architecture)
                    {
                        // If 'C' is newline, stop
                        if (c == '\n')
                            break;

                        // Otherwise copy
                        EFI->systemReport.processorArch[idx++] = c;
                        EFI->systemReport.processorArch[idx] = 0;
                    }
                }

            }

            // Otherwise, if line starts with 'Model name:'
            if (line.find("Model name:") != std::string::npos)
            {
                // Move to
                size_t pos = line.find(":");

                // If not end of position
                if (pos != std::string::npos)
                {
                    // Get beginning of name
                    std::size_t offset = 1;
                    std::size_t wordBeginOffset = 1;

                    // While line doesn't begin,
                    while (line.at(pos + offset) == ' ')
                        offset++;

                    // Set wordBegin
                    wordBeginOffset = offset;

                    // Create a pre-parsed string
                    std::string preParsed = line.substr(pos + offset);

                    // Look for a word that contains a number
                    for (auto c : preParsed)
                    {
                        std::printf("c: %c\n", c);

                        // If space, update offset and wordBeginOffset accordingly
                        if (c == ' ')
                        {
                            offset++;
                            wordBeginOffset = offset;
                            continue;
                        }

                        // If c is numerical, exit out
                        if (c >= '0' && c <= '9')
                            break;

                        // Otherwise increase offset
                        offset++;
                    }

                    // Parse Name
                    std::string Name = line.substr(pos + wordBeginOffset);

                    // Copy over text
                    for (BBP::std::index_t idx = 0; auto c: Name)
                    {
                        // If 'C' is newline, stop
                        if (c == '\n')
                            break;

                        // If 'C' is '@', stop
                        if (c == '@')
                            break;

                        // Otherwise copy
                        EFI->systemReport.processorName[idx++] = c;
                        EFI->systemReport.processorName[idx] = 0;
                    }
                }

            }
        }
    }

    return 0;
}