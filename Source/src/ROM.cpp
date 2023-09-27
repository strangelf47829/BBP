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