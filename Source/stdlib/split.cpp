#include "../include/stdint.h"

void BBP::std::split(BBP::uint16_t ref, BBP::uint8_t &MSB, BBP::uint8_t &LSB, bool endian)
{
	// If endian,
	if (endian)
	{
		LSB = (ref & 0xFF);
		MSB = (ref >> 8) & 0xFF;
	}
	else
	{
		MSB = (ref & 0xFF);
		LSB = (ref >> 8) & 0xFF;
	}
}

void BBP::std::split(BBP::uint32_t ref, BBP::uint16_t &MSB, BBP::uint16_t &LSB, bool endian)
{
	// If endian,
	if (endian)
	{
		LSB = (ref & 0xFFFF);
		MSB = (ref >> 16) & 0xFFFF;
	}
	else
	{
		MSB = (ref & 0xFFFF);
		LSB = (ref >> 16) & 0xFFFF;
	}
}

void BBP::std::split(BBP::uint32_t ref, BBP::uint8_t &MSSB, BBP::uint8_t &LSSB, BBP::uint8_t &MSLB, BBP::uint8_t &LSLB, bool endian)
{
	// Create two 16 bits
	BBP::uint16_t MSB = 0;
	BBP::uint16_t LSB = 0;

	// Then split 16 bits
	split(ref, MSB, LSB, endian);

	// Then further split MSB and LSB
	split(MSB, MSSB, LSSB, endian);
	split(LSB, MSLB, LSLB, endian);
}