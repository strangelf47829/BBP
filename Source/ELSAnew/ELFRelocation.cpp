#include "../include/ELSA/ELFRelocation.h"
#include "../include/stdint.h"

// Constructor
BBP::ELF::Relocation::Relocation()
	: offset(0), type(0), value(0)
{}

// Nullifying means making everything equal to 0
void BBP::ELF::Relocation::Nullify()
{
	offset = 0;
	type = 0;
	value = 0;
}

// Read data
void BBP::ELF::Relocation::readData(bool endian)
{
	offset = std::stitch(data[0], data[1], data[2], data[3], !endian);

	// Because of the 24 bit structure, the stitch function won't work on its own.
	std::word info = std::stitch(data[4], data[5], data[6], data[7], !endian);

	// type is first byte
	type = (info & 0xFF);

	// value is remaining three
	value = (info >> 8) & 0xFFFFFF;
}

// Write data
void BBP::ELF::Relocation::writeData(bool endian)
{
	std::split(offset, data[0], data[1], data[2], data[3], !endian);

	// Create word
	std::word info = ((value << 8) & 0xFFFFFF00) | type;

	// Because of the 24 bit structure, the stitch function won't work on its own.
	std::split(info, data[4], data[5], data[6], data[7], !endian);
}