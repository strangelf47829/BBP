/*

Copyright 2023 PCiD

This file is part of BBP.

BBP is free software: you can redistribute it and/or modify it under the terms of the GNU Lesser General
Public License as published by the Free Software Foundation, either version 3 of the License, or (at your
option) any later version.

BBP is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even
the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License along with BBP. If not, see
<https://www.gnu.org/licenses/>.

Author: Rafael de Bie

*/

#include "Kernel.h"

#include <stdio.h>
#include <stdlib.h>
#include <fstream>

#include "SDL.h"

using namespace BBP::IO;
/*
#pragma region GPIO



int GPIO::Count = 0;
int GPIO::Vectortable_c = 0;
GPIO::GPIOfunc *GPIO::Vectortable = nullptr;

int GPIO::AllocationC = 0;
GPIO::GPIO_allocation *GPIO::AllocationV = nullptr;

int GPIO::initializeGPIO()
{
    // Virtual GPIO pins, so lets get ready
    GPIO::Count = 50;
    GPIO::Vectortable_c = 0;
    GPIO::Vectortable = nullptr;
    return 0;
}

void GPIO::terminateGPIO()
{
    for(int i = 0; i < GPIO::AllocationC; i++)
    {
        if(GPIO::AllocationV[i].GPIO_v != nullptr)
            free(GPIO::AllocationV[i].GPIO_v);
        delete &GPIO::AllocationV[i];
    }
    if(GPIO::AllocationV != nullptr)
        free(GPIO::AllocationV);
}


#pragma endregion
*/

#pragma region ROM

int Memory::ROM::ROM_SIZE = 0;

static std::ifstream input;
static mem_t *FILEARR;

int Memory::ROM::initializeROM()
{
    input = std::ifstream("V:\\dev\\CMOS", std::ios::binary);

    if(!input.is_open())
        return 1;

    input.seekg(0, std::ios::end);
    Memory::ROM::ROM_SIZE = input.tellg();
    input.seekg(0, std::ios::beg);

    FILEARR = (mem_t*)calloc(Memory::ROM::ROM_SIZE, sizeof(mem_t));

    char next;
    int index = 0;
    while(input.get(next))
        FILEARR[index++] = next;

    return 0;
}

mem_t Memory::ROM::readROM(unsigned int address)
{
    return FILEARR[address];
}

void Memory::ROM::terminateROM()
{
    input.close();
}

#pragma endregion

#pragma region Interrupts

void BBP::Services::Interrupts::preupdateInterruptsExternal()
{
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		/* handle your event here */
	//User requests quit
		//if( event.type == SDL_QUIT )
            //BBP::Services::Interrupts::interrupts[0]->triggered = true;
			//BBP::IO::Interrupts::KERNEL_QUIT_INTERRUPT = true;
	}
}

#pragma endregion

#pragma region SDMMC

bool BBP::IO::SDMMC::initializeSDMMC()
{
    return false;
}

int BBP::IO::File::getVolumeCount()
{
    return 1;
}

BBP::IO::File::VOLUME_INFO BBP::IO::File::getVolumeInfo(int volume)
{
    return BBP::IO::File::VOLUME_INFO(
        {
            100,
            100,

            BBP::IO::File::VOLUME_INFO::B607,

            'V',

            true,
            true,

            true,
            true,
            
            nullptr
        });
}

BBP::IO::File::FILE_HANDLE BBP::IO::SDMMC::readFile(const char *file)
{
    BBP::IO::File::FILE_HANDLE handle = BBP::IO::File::FILE_HANDLE({nullptr,  0, nullptr, 0});

    if(file == nullptr)
        return handle;

    input = std::ifstream((const char*)file, std::ios::binary);

    if(!input.is_open())
        return handle;

    input.seekg(0, std::ios::end);
    handle.byteCount = input.tellg();
    input.seekg(0, std::ios::beg);

    handle.fileVector = (mem_t*)calloc(handle.byteCount, sizeof(mem_t));

    char next;
    int index = 0;
    while(input.get(next))
        handle.fileVector[index++] = next;

    while(file[handle.fileNameLength++])
    {}

    handle.fileNameLength--;

    handle.fileName = (mem_t*)calloc(handle.fileNameLength, sizeof(mem_t));

    for(int i = 0; i < handle.fileNameLength; i++)
    { handle.fileName[i] = file[i]; }

    return handle;
}

void BBP::Services::Interrupts::postupdateInterruptsExternal()
{

}

#pragma endregion
