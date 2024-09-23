#include "../include/ELSA/ELFSegment.h"
#include "../include/stdint.h"

BBP::ELF::Segment::Segment()
	: type(0), offset(0), vaddr(0), paddr(0), filesz(0), memsz(0), flags(0), align(0)
{}

void BBP::ELF::Segment::nullify()
{
	type = 0;
	offset = 0;
	vaddr = 0;
	paddr = 0;
	filesz = 0;
	memsz = 0;
	flags = 0;
	align = 0;
}

void BBP::ELF::Segment::readData(bool endian)
{
	type =		std::stitch(data[0], data[1], data[2], data[3], !endian);
	offset =	std::stitch(data[4], data[5], data[6], data[7], !endian);
	paddr =		std::stitch(data[8], data[9], data[10], data[11], !endian);
	vaddr =		std::stitch(data[12], data[13], data[14], data[15], !endian);
	filesz =	std::stitch(data[16], data[17], data[18], data[19], !endian);
	memsz =		std::stitch(data[20], data[21], data[22], data[23], !endian);
	flags =		std::stitch(data[24], data[25], data[26], data[27], !endian);
	align =		std::stitch(data[28], data[29], data[30], data[31], !endian);
}

void BBP::ELF::Segment::writeData(bool endian)
{
	std::split(type, data[0], data[1], data[2], data[3], !endian);
	std::split(offset, data[4], data[5], data[6], data[7], !endian);
	std::split(paddr, data[8], data[9], data[10], data[11], !endian);
	std::split(vaddr, data[12], data[13], data[14], data[15], !endian);
	std::split(filesz, data[16], data[17], data[18], data[19], !endian);
	std::split(memsz, data[20], data[21], data[22], data[23], !endian);
	std::split(flags, data[24], data[25], data[26], data[27], !endian);
	std::split(align, data[28], data[29], data[30], data[31], !endian);
}