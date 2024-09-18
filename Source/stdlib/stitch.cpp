#include "../include/stdint.h"

BBP::uint16_t BBP::std::stitch(uint8_t MSB, uint8_t LSB, bool littleEndian)
{
	// Create halfword
	BBP::uint16_t hw = (littleEndian ? MSB : LSB);

	// Shift
	hw = hw << 8;

	// Then append last byte
	if (littleEndian)
		return hw | LSB;
	else
		return hw | MSB;
}

BBP::uint32_t BBP::std::stitch(uint16_t MSB, uint16_t LSB, bool littleEndian)
{
	// Create word
	BBP::uint32_t w = (littleEndian ? MSB : LSB);

	// Shift
	w = w << 8;

	// Then append last halfword
	if (littleEndian)
		return w | LSB;
	else
		return w | MSB;
}

// Conjoin 4 bytes into word, using predetermined functions
// Little endian: a-b-c-d
// Big endian 
BBP::uint32_t BBP::std::stitch(uint8_t MSSB, uint8_t LSSB, uint8_t MSLB, uint8_t LSLB, bool littleEndian)
{
	// Create two half words
	BBP::uint16_t MSHW = 0;
	BBP::uint16_t LSHW = 0;
	
	// Then assign value based on endiannes
	if (littleEndian)
	{
		// If little endian, the order is [MSSB,LSSB] - [MSLB,LSLB]
		MSHW = stitch(MSSB, LSSB, true);
		LSHW = stitch(MSLB, LSLB, true);
	}
	else
	{
		// Otherwise, if big endian, the order is [LSLB,MSLB] - [LSSB,MSSB]
		LSHW = stitch(LSSB, MSSB, true);
		MSHW = stitch(LSLB, MSLB, true);
	}

	// Then create word with MSHW, then shift over
	BBP::uint32_t word = MSHW;
	word = word << 16;

	// Then append LSLB
	word = word | LSHW;

	// Now return
	return word;
}