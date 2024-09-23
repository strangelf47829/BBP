#include "../include/ELSA/ELFSection.h"
#include "../include/stdint.h"

// Initialize everything to 0
BBP::ELF::Section::Section()
	: name(0),
	type(0),
	flags(0),
	address(0),
	offset(0),
	size(0),
	link(0),
	info(0),
	addralign(0),
	entsize(0)
{

}

void BBP::ELF::Section::nullify()
{
	name = 0;
	type = 0;
	flags = 0;
	address = 0;
	offset = 0;
	size = 0;
	link = 0;
	info = 0;
	addralign = 0;
	entsize = 0;
}

void BBP::ELF::Section::readData(bool endian)
{
	// name is index 0..3
	name = std::stitch(data[0], data[1], data[2], data[3], !endian);

	// type is index 4..7
	type = std::stitch(data[4], data[5], data[7], data[8], !endian);

	// flags is index 8..11
	flags = std::stitch(data[8], data[9], data[10], data[11], !endian);

	// Address is index 12..15
	address = std::stitch(data[12], data[13], data[14], data[15], !endian);

	// offset is index 16..19
	offset = std::stitch(data[16], data[17], data[18], data[19], !endian);

	// size is index 20..23
	size = std::stitch(data[20], data[21], data[22], data[23], !endian);

	// link is index 24..27
	link = std::stitch(data[24], data[25], data[26], data[27], !endian);

	// info is index 28..31
	info = std::stitch(data[28], data[29], data[30], data[31], !endian);

	// addralign is index 32..35
	addralign = std::stitch(data[32], data[33], data[34], data[35], !endian);

	// entsize is index 36..39
	entsize = std::stitch(data[36], data[37], data[38], data[39], !endian);
}

void BBP::ELF::Section::writeData(bool endian)
{
	// name is index 0..3
	std::split(name,data[0], data[1], data[2], data[3], !endian);

	// type is index 4..7
	std::split(type,data[4], data[5], data[7], data[8], !endian);

	// flags is index 8..11
	std::split(flags,data[8], data[9], data[10], data[11], !endian);

	// Address is index 12..15
	std::split(address,data[12], data[13], data[14], data[15], !endian);

	// offset is index 16..19
	std::split(offset,data[16], data[17], data[18], data[19], !endian);

	// size is index 20..23
	std::split(size,data[20], data[21], data[22], data[23], !endian);

	// link is index 24..27
	std::split(link,data[24], data[25], data[26], data[27], !endian);

	// info is index 28..31
	std::split(info,data[28], data[29], data[30], data[31], !endian);

	// addralign is index 32..35
	std::split(addralign,data[32], data[33], data[34], data[35], !endian);

	// entsize is index 36..39
	std::split(entsize,data[36], data[37], data[38], data[39], !endian);
}