#include "../include/FAT.h"

// | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 | A | B | C | D | E | F |
// | d | d | d | d | d | m | m | m | m | y | y | y | y | y | y |
BBP::std::halfword BBP::fs::encodeFATDate(std::byte day, std::byte month, std::halfword year)
{
	// Declare result
	std::halfword date = 0;

	// Set year first (6 bits)
	if (1979 < year && year < 2108)
		date |= (year - 1980) & 0b111111;

	// Store year in upper 6 bits
	date = date << 6;

	// Then set month (4 bits)
	if (0 < month && month < 13)
		date |= (month & 0b1111);

	// Then store 4 bits over
	date = date << 4;
	
	// Lastly store date between 1 and 31 inclusive
	if (0 < day && day < 32)
		date |= (day & 0b11111);

	// Return result
	return date;
}

// Decode a FAT date, see 'encodeFATDate' to learn how
void BBP::fs::decodeFATDate(std::halfword data, std::byte &day, std::byte &month, std::halfword &year)
{
	// Date first
	day = data & 0b11111;

	// Shift over 5 bits so month is up front
	data >>= 5;

	// Decode month
	month = data & 0b1111;

	// Shift over 4 bits so year is next
	data >>= 4;

	// Year is 6 bits + 1980
	year = (data & 0b111111) + 1980;
}
