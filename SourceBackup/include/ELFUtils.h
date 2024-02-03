#ifndef CLELF_ELFUTILS_H
#define CLELF_ELFUTILS_H

//#include "../include/Kernel.h"
#include <cstdlib>
#include <cstddef>
#include <cstdio>

#define setHALFWORD(arr, addr, value) \
	arr [addr+1] = (value >> 8); \
	arr [addr+0] = (value & 0xFF)

#define setWORD(arr, addr, value) \
	arr [addr+3] = (value >> 24); \
	arr [addr+2] = (value >> 16) & 0xFF; \
	arr [addr+1] = (value >> 8) & 0xFF; \
	arr [addr+0] = (value >> 0) & 0xFF

#include "ELF64.h"
#include "ELFBuilder.h"
#include "ELFEmitter.h"
#include "ELFReader.h"
#include "ELFRepair.h"
#include "ELFLoader.h"

#endif
