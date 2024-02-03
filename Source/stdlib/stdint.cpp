#include "../include/stdint.h"

BBP::uint32_t BBP::std::decimals(uint32_t value)
{
	uint32_t decim = 1;
	for (uint32_t val = value; val >= 10; val /= 10)
		decim++;
	return decim;
}

BBP::uint8_t BBP::std::getDecimal(uint32_t value, uint32_t position)
{
	uint32_t divisor = 1;

	for (uint32_t p = 0; p < position; p++)
		divisor *= 10;

	return (value / divisor) % 10;
}