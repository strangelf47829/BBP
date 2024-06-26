#include "../include/stdint.h"
#include "../include/stddef.h"

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

BBP::uint32_t BBP::std::reverse(uint32_t value)
{
	// Get log10
	word log10 = decimals(value);

	// Declare reversed value
	word reversed = 0;

	// Declare reversed radix
	word reversedRadix = 1;

	// Declare forward radix
	word forwardRadix = 1;
	for (std::index_t idx = 1; idx < log10; idx++)
		forwardRadix *= 10;

	// For each decimal;
	for (std::index_t idx = 0; idx < log10; idx++)
	{
		// Get decimal from value
		byte decimal = (value / forwardRadix) % 10;

		// Then append that value to reversed
		reversed += decimal * reversedRadix;

		// Then modify radicies
		forwardRadix /= 10;
		reversedRadix *= 10;
	}
	
	// Return reversed
	return reversed;
}