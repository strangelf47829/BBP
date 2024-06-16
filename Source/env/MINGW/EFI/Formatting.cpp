#include "../include/BIOS.h"

void Host::BIOS::print(BBP::std::word num)
{
	// Begin by declaring buffer
	BBP::std::static_string<12> buff;

	// Then get log10
	BBP::std::byte log = BBP::std::decimals(num);
	
	// Get active number
	BBP::std::word val = num;

	// Then, get each decimal
	for (BBP::std::index_t idx = 0; idx < log; idx++)
	{
		// Get decimal for number
		BBP::std::byte decimal = val % 10;

		// Then divide value by 10
		val = val / 10;

		// Store number in log - idx
		buff[log - idx - 1] = decimal + '0';
	}

	// Store terminating 0
	buff[log] = 0;

	// Then print
	print(buff.data);
}

void Host::BIOS::printMem(BBP::std::word memorySize)
{
	// Check for suffix
	if (memorySize < 1024)
	{
		print(memorySize);
		print("KiB");
	}
	else if (memorySize < 1024 * 1024)
	{
		print(memorySize / 1024);
		print("MiB");
	}
	else
	{
		print(memorySize / (1024 * 1024));
		print("GiB");
	}
}