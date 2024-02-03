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

#include "../include/Kernel.h"

#ifdef BBP_MEMORY_LITTLEENDIAN

int BBP::IO::Memory::ROM::readROMInt(unsigned int offset)
{
    int a = 0xFF & BBP::IO::Memory::ROM::readROM(address * 4 + 0);
    int b = 0xFF & BBP::IO::Memory::ROM::readROM(address * 4 + 1);
    int c = 0xFF & BBP::IO::Memory::ROM::readROM(address * 4 + 2);
    int d = 0xFF & BBP::IO::Memory::ROM::readROM(address * 4 + 3);

    int ret = (d << 24) | (c << 16) | (b << 8) | a;
    return ret;
}

#endif


#ifdef BBP_MEMORY_BIGENDIAN

int BBP::IO::Memory::ROM::readROMInt(unsigned int offset)
{
    int a = 0xFF & BBP::IO::Memory::ROM::readROM(offset * 4 + 0);
    int b = 0xFF & BBP::IO::Memory::ROM::readROM(offset * 4 + 1);
    int c = 0xFF & BBP::IO::Memory::ROM::readROM(offset * 4 + 2);
    int d = 0xFF & BBP::IO::Memory::ROM::readROM(offset * 4 + 3);

    int ret = (a << 24) | (b << 16) | (c << 8) | d;
    return ret;
}

#endif

mem_t *BBP::IO::Memory::ROM::readROMChunk(unsigned int start, unsigned int end)
{
    mem_t *chunk = (mem_t *)BBP::Services::c_alloc(end - start + 1, sizeof(mem_t));
    int index = 0;
    for (int i = start; i < end; i++)
        chunk[index++] = BBP::IO::Memory::ROM::readROM(i);
    return chunk;
}

int *BBP::IO::Memory::ROM::readROMIntChunk(unsigned int start, unsigned int end)
{
    int *arr = (int *)BBP::Services::c_alloc(end - start + 1, sizeof(int));
    int index = 0;
    for (int i = start; i < end; i++)
        arr[index++] = BBP::IO::Memory::ROM::readROMInt(i);
    return arr;
}