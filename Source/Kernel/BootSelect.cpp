#include "../include/Kernel.h"

BBP::system::BootRecord::bootRecordEntryPoint BBP::system::Kernel::enterBootSelection(std::PAGE<BootRecord *> &bootrecords, system::BootRecord::bootRecordEntryPoint entry, system::BootRecord::bootRecordEntryPoint BIOS)
{
	// Keep track of current selection
	std::index_t selection = 1;

	while (true)
	{

		// Clear the screen
		clearScreen();

		// Print help
		std::printf("Boot selection menu:\nUse the 'w' and 's' keys to move up and down to select an os to boot into.\n");
		std::printf("The default choice is marked with '\e[0;34m*\e[0;37m'. Options not available are marked with '\e[0;31mx\e[0;37m'.\n");
		std::printf("Your current selection is marked in \e[0;32mgreen\e[0;37m.\n\n");

		// Print out stuff
		for (std::index_t idx = 0; idx < bootrecords.dataSize; idx++)
		{
			//print out index
			std::printf("\t<%u>", idx);

			// If this boot record is the default, mark with *
			if (bootrecords[idx]->entryPoint == entry)
				std::printf("\e[0;34m*\e[0;37m");

			// If this boot record is unavailable, mark with x
			if (bootrecords[idx]->computedValue == false)
				std::printf("\e[0;31mx\e[0;37m");

			// If boot record is selected, make green if available
			if (idx == selection && bootrecords[idx]->computedValue == false)
				std::printf("\e[0;31m");
			else if (idx == selection)
				std::printf("\e[0;32m");

			// Print out name
			std::printf("\t%s\e[0;37m\n", bootrecords[idx]->recordName.data);
		}

		// Print out other tip
		std::printf("\n<enter>: Make selection\n<tab>: Default selection\n");

		// Get character
		std::string_element choice = system::Kernel::getKey();

		// If tab, exit out
		if (choice == '\t')
			break;

		// If 'w', and selection allows it, move up
		if (choice == 's' && (selection + 1) < bootrecords.dataSize)
			selection++;

		// If 's', and selection allows it, move down
		if (choice == 'w' && selection)
			selection--;

		// If enter, exit out
		if (choice == 0x0a)
		{
			if (bootrecords[selection]->computedValue == false)
				continue;

			if (selection == 0)
				return BIOS;
			else
				return bootrecords[selection]->entryPoint;
		}

	}

	// None found, throww error
	throw std::exception("Could not load boot records", ENODATA);
}
