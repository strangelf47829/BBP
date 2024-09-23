#include "../include/ELSA/ELFSymbol.h"
#include "../include/stdint.h"

// Constructor
BBP::ELF::Symbol::Symbol()
	: name(0), value(0), size(0), info(0), other(0), shndx(0)
{}

// Nullifying means making everything equal to 0
void BBP::ELF::Symbol::Nullify()
{
	name = 0;
	value = 0;
	size = 0;
	info = 0;
	other = 0;
	shndx = 0;
}

// Read data
void BBP::ELF::Symbol::readData(bool endian)
{
	name = std::stitch(data[0], data[1], data[2], data[3], !endian);

	value = std::stitch(data[4], data[5], data[6], data[7], !endian);

	size = std::stitch(data[8], data[9], data[10], data[11], !endian);

	info = data[12];
	other = data[13];

	shndx = std::stitch(data[14], data[15], !endian);
}

// Write data
void BBP::ELF::Symbol::writeData(bool endian)
{
	 std::split(name, data[0], data[1], data[2], data[3], !endian);

	 std::split(value, data[4], data[5], data[6], data[7], !endian);

	 std::split(size, data[8], data[9], data[10], data[11], !endian);

	 data[12] = info;
	 data[13] = info;

	 std::split(shndx, data[14], data[15], !endian);
}