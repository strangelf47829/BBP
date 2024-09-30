#include "../include/stdint.h"

// Is little endian?
bool isLittleEndian = true;

// What is the environment endianness policy?
bool BBP::std::isLittleEndianEnvironment()
{
	return isLittleEndian;
}

// Set endianness globally
void BBP::std::setLittleEndianEnvironment()
{
	isLittleEndian = true;
}

void BBP::std::setBigEndianEnvironment()
{
	isLittleEndian = false;
}

BBP::uint16_t BBP::std::stitch(uint8_t MSB, uint8_t LSB) { return stitch(MSB, LSB, isLittleEndian); }
BBP::uint32_t BBP::std::stitch(uint16_t MSB, uint16_t LSB) { return stitch(MSB, LSB, isLittleEndian); }
BBP::uint32_t BBP::std::stitch(uint8_t MSSB, uint8_t LSSB, uint8_t MSLB, uint8_t LSLB) { return stitch(MSSB, LSSB, MSLB, LSLB, isLittleEndian); }

// Above split functions but using global endianness values
void BBP::std::split(uint16_t val, uint8_t &MSB, uint8_t &LSB) { split(val, MSB, LSB, isLittleEndian); }
void BBP::std::split(uint32_t val, uint16_t &MSB, uint16_t &LSB) { split(val, MSB, LSB, isLittleEndian); }
void BBP::std::split(uint32_t val, uint8_t &MSSB, uint8_t &LSSB, uint8_t &MSLB, uint8_t &LSLB) { split(val, MSSB, LSSB, MSLB, LSLB, isLittleEndian); }