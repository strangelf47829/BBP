#include "../../include/ScreenDriver.h"
#include "../../include/Environment.h"

#define RED "\e[0;31m"
#define NORM "\e[0;37m"
#define PADDING "%s"
#define PROCPADDING "\t"

bool Environment::Drivers::screen::clearScreen(BBP::std::size_t, BBP::std::word *)
{
	// The string that actually clears the string
	BBP::std::string clearScreenString = BBP::std::String("\033[2J\033[1;1H");

	// print
	BBP::std::word dataSent = BBP::std::printf("%s", clearScreenString.data);

	// Return success
	return dataSent > 0;
}

// Display splash screen (UBUNTU only supports text)
bool Environment::Drivers::screen::printSplash(BBP::std::size_t argc, BBP::std::word *argv)
{
	// Padding stuff
	BBP::std::static_string<255> paddingStr;
	BBP::std::c_string padding = paddingStr.data;

	// If argc and argv are not available, set manually
	if (argc == 0 || argv == nullptr)
		BBP::std::strcpy(&paddingStr, "\t\t");


	// If arguments are supplied, print extra info
	if (argc && argv)
	{
		// Get configuration
		BBP::system::EFI *EFI = (BBP::system::EFI *)argv;

		// If TTY is supported, and at least something was found
		if (EFI->systemReport.supportsTTY && EFI->systemReport.TTYHorizontalPage)
		{
			// Calculate padding accordingly.
			BBP::std::size_t leftPadding = ((EFI->systemReport.TTYHorizontalPage - sizeof("+== " ".... BIOS" " ====" "===============================================+")) - 5) / 2;

			// Apply padding
			for (BBP::std::index_t idx = 0; idx < leftPadding; idx++)
				padding[idx] = ' ';

			// Null terminate
			padding[leftPadding] = 0;
		}

		// Otherwise just apply some padding
		else
		{
			BBP::std::strcpy(&paddingStr, "\t\t");
		}

		// Declare two strings
		BBP::std::static_string<64> totalRamStr;
		BBP::std::static_string<64> freeRamStr;

		// Sprintf total ram
		if (EFI->systemReport.totalMemory > 1024 * 1024)
			BBP::std::sprintf(totalRamStr.data, "%dGiB", EFI->systemReport.totalMemory / (1024 * 1024));
		else if (EFI->systemReport.totalMemory > 1024)
			BBP::std::sprintf(totalRamStr.data, "%dMiB", EFI->systemReport.totalMemory / (1024));
		else
			BBP::std::sprintf(totalRamStr.data, "%dKiB", EFI->systemReport.totalMemory);

		// Sprintf free ram
		if (EFI->systemReport.usefulHeap > 1024 * 1024)
			BBP::std::sprintf(freeRamStr.data, "%dGiB", EFI->systemReport.usefulHeap / (1024 * 1024));
		else if (EFI->systemReport.usefulHeap > 1024)
			BBP::std::sprintf(freeRamStr.data, "%dMiB", EFI->systemReport.usefulHeap / (1024));
		else
			BBP::std::sprintf(freeRamStr.data, "%dKiB", EFI->systemReport.usefulHeap);

		// Print device name, etc...
		BBP::std::printf(PADDING RED "+== " NORM "%s BIOS" RED " ====" NORM "===============================================+" NORM "\n", padding, EFI->systemReport.HostName);
		BBP::std::printf(PADDING NORM "Device name: %s" "\n", padding, EFI->system.deviceName);
		BBP::std::printf(PADDING NORM "Primary volume '%c' mounted at '%s'" "\n", padding, EFI->system.volumeLabel, EFI->system.volumePath);
		BBP::std::printf(PADDING NORM "System memory: %s (%s useful heap)" "\n", padding, totalRamStr.data, freeRamStr.data);
		
		// Spacing
		BBP::std::printf(PADDING NORM "\n", padding);

		// Print Video mode
		if (EFI->systemReport.supportsTTY && EFI->systemReport.supportsGUI)
			BBP::std::printf(PADDING NORM "Video mode: mixed" "\n", padding);
		else if (EFI->systemReport.supportsTTY)
			BBP::std::printf(PADDING NORM "Video mode: TTY" "\n", padding);
		else if (EFI->systemReport.supportsGUI)
			BBP::std::printf(PADDING NORM "Video mode: GUI" "\n", padding);
		else
			BBP::std::printf(PADDING NORM "Video mode: " RED "ERROR" NORM "\n", padding);

		// Print video capabilities
		if (EFI->systemReport.supportsTTY && EFI->systemReport.supportsGUI == false)
			BBP::std::printf(PADDING NORM "TTY Resolution %u x %u characters" "\n", padding, EFI->systemReport.TTYHorizontalPage, EFI->systemReport.TTYVerticalPage);

		else if (EFI->systemReport.supportsGUI && EFI->systemReport.supportsTTY == false)
			BBP::std::printf(PADDING NORM "%uB VMEM: GUI %u x %u Pixels (colour depth %u)" "\n", padding, EFI->systemReport.VMEMSize, EFI->systemReport.xResolution, EFI->systemReport.yResolution, EFI->systemReport.colourDepth);

		else if (EFI->systemReport.supportsGUI && EFI->systemReport.supportsTTY)
			BBP::std::printf(PADDING NORM "%uB VMEM: GUI/TTY %u x %u x %u Pixels / %u x %u TTY" "\n", padding, EFI->systemReport.VMEMSize, EFI->systemReport.xResolution, EFI->systemReport.yResolution, EFI->systemReport.colourDepth, EFI->systemReport.TTYHorizontalPage, EFI->systemReport.TTYVerticalPage);
		else
			BBP::std::printf(PADDING NORM "Unkown video mode" "\n", padding);

		// Spacing
		BBP::std::printf(PADDING NORM "\n", padding);

		// Printing
		BBP::std::printf(PADDING NORM "Enter BIOS: <" "\e[0;93m" "%c" NORM ">" " ==" NORM "== Boot Selection <" "\e[0;93m" "tab" NORM ">" "\n", padding, EFI->post.biosModeKey);

		// Spacing
		BBP::std::printf(PADDING NORM "\n", padding);

		// Print Kernel info
		BBP::std::printf(PADDING NORM "%s Kernel Version %u.%u.%u.%u (%s) (C) %u %s" "\n", padding, EFI->licenses.KernelName, EFI->licenses.KernelInfo.build.majorVersion, EFI->licenses.KernelInfo.build.minorVersion, EFI->licenses.KernelInfo.build.buildVersion, EFI->licenses.KernelInfo.build.patchVersion, EFI->licenses.KernelInfo.build.extraInfo, EFI->licenses.KernelInfo.copyright.copyrightYear, EFI->licenses.KernelInfo.copyright.copyrightHolder);

		// Print BIOS info
		BBP::std::printf(PADDING NORM "BIOS Version %u.%u.%u.%u (%s) (C) %u %s" "\n", padding, EFI->licenses.BIOSInfo.build.majorVersion, EFI->licenses.BIOSInfo.build.minorVersion, EFI->licenses.BIOSInfo.build.buildVersion, EFI->licenses.BIOSInfo.build.patchVersion, EFI->licenses.BIOSInfo.build.extraInfo, EFI->licenses.BIOSInfo.copyright.copyrightYear, EFI->licenses.BIOSInfo.copyright.copyrightHolder);

		// Licensing info
		BBP::std::printf(PADDING NORM "Released under %s. More info at %s" "\n", padding, EFI->licenses.KernelInfo.license.licenseName, EFI->licenses.KernelInfo.license.licenseURL);

		// Print bottom
		BBP::std::printf(PADDING RED "+=======================" NORM "===============================================+" NORM "\n" "\n", padding);

		// Print top of BB with info
		BBP::std::printf(PADDING RED "BBBBBBBBBBBBBBBBB   BBBBBBBBBBBBBBBBB" NORM "\n", padding);
		BBP::std::printf(PADDING RED "B::::::::::::::::B  B::::::::::::::::B  " NORM PROCPADDING RED "Processor info" NORM " : " "\n", padding);
		BBP::std::printf(PADDING RED "B::::::BBBBBB:::::B B::::::BBBBBB:::::B " NORM PROCPADDING RED "Speed" NORM " - % uMHz" "\n", padding, EFI->systemReport.processorSpeed);
		BBP::std::printf(PADDING RED "BB:::::B     B:::::BBB:::::B     B:::::B" NORM PROCPADDING RED "Arch" NORM " - %s" "\n", padding, EFI->systemReport.processorArch.data);
		BBP::std::printf(PADDING RED "  B::::B     B:::::B  B::::B     B:::::B" NORM PROCPADDING RED "Name" NORM " - %s" "\n", padding, EFI->systemReport.processorName.data);
	}
	else
	{
		// Otherwise print 'BB' Normally
		BBP::std::printf(PADDING RED "BBBBBBBBBBBBBBBBB   BBBBBBBBBBBBBBBBB" NORM "\n", padding);
		BBP::std::printf(PADDING RED "B::::::::::::::::B  B::::::::::::::::B" NORM "\n", padding);
		BBP::std::printf(PADDING RED "B::::::BBBBBB:::::B B::::::BBBBBB:::::B" NORM "\n", padding);
		BBP::std::printf(PADDING RED "BB:::::B     B:::::BBB:::::B     B:::::B" NORM "\n", padding);
		BBP::std::printf(PADDING RED "  B::::B     B:::::B  B::::B     B:::::B" NORM "\n", padding);
	}

	
	BBP::std::printf(PADDING RED "  B::::B     B:::::B  B::::B     B:::::B" NORM "\n", padding);
	BBP::std::printf(PADDING RED "  B::::BBBBBB:::::B   B::::BBBBBB:::::B" NORM "\n", padding);
	BBP::std::printf(PADDING RED "  B:::::::::::::BB    B:::::::::::::BB" NORM "\n", padding);
	BBP::std::printf(PADDING RED "  B::::BBBBBB:::::B   B::::BBBBBB:::::B" NORM "\n", padding);
	BBP::std::printf(PADDING RED "  B::::B     B:::::B  B::::B     B:::::B" NORM "\n", padding);
	BBP::std::printf(PADDING RED "  B::::B     B:::::B  B::::B     B:::::B" NORM "\n", padding);
	BBP::std::printf(PADDING RED "  B::::B     B:::::B  B::::B     B:::::B" NORM "\n", padding);
	BBP::std::printf(PADDING RED "BB:::::BBBBBB::::::BBB:::::BBBBBB::::::B" NORM "\n", padding);
	BBP::std::printf(PADDING RED "B:::::::::::::::::B B:::::::::::::::::B" NORM "\n", padding);
	BBP::std::printf(PADDING RED "B::::::::::::::::B  B::::::::::::::::B" NORM "\n", padding);
	BBP::std::printf(PADDING RED "BBBBBBBBBBBBBBBBB   BBBBBBBBBBBBBBBBB" NORM "\n", padding);

	// Then 607
	BBP::std::printf(PADDING RED "        66666666    " NORM "         000000000          77777777777777777777" NORM "\n", padding);
	BBP::std::printf(PADDING RED "       6::::::6     " NORM "       00:::::::::00        7::::::::::::::::::7" NORM "\n", padding);
	BBP::std::printf(PADDING RED "      6::::::6      " NORM "     00:::::::::::::00      7::::::::::::::::::7" NORM "\n", padding);
	BBP::std::printf(PADDING RED "     6::::::6       " NORM "    0:::::::000:::::::0     777777777777:::::::7" NORM "\n", padding);
	BBP::std::printf(PADDING RED "    6::::::6        " NORM "    0::::::0   0::::::0                7::::::7 " NORM "\n", padding);
	BBP::std::printf(PADDING RED "   6::::::6         " NORM "    0:::::0     0:::::0               7::::::7  " NORM "\n", padding);
	BBP::std::printf(PADDING RED "  6::::::6          " NORM "    0:::::0     0:::::0              7::::::7   " NORM "\n", padding);
	BBP::std::printf(PADDING RED " 6::::::::66666     " NORM "    0:::::0 000 0:::::0             7::::::7    " NORM "\n", padding);
	BBP::std::printf(PADDING RED "6::::::::::::::66   " NORM "    0:::::0 000 0:::::0            7::::::7     " NORM "\n", padding);
	BBP::std::printf(PADDING RED "6::::::66666:::::6  " NORM "    0:::::0     0:::::0           7::::::7      " NORM "\n", padding);
	BBP::std::printf(PADDING RED "6:::::6     6:::::6 " NORM "    0:::::0     0:::::0          7::::::7       " NORM "\n", padding);
	BBP::std::printf(PADDING RED "6:::::6     6:::::6 " NORM "    0::::::0   0::::::0         7::::::7        " NORM "\n", padding);
	BBP::std::printf(PADDING RED "6::::::66666::::::6 " NORM "    0:::::::000:::::::0        7::::::7         " NORM "\n", padding);
	BBP::std::printf(PADDING RED " 66:::::::::::::66  " NORM "     00:::::::::::::00        7::::::7          " NORM "\n", padding);
	BBP::std::printf(PADDING RED "   66:::::::::66    " NORM "       00:::::::::00         7::::::7           " NORM "\n", padding);
	BBP::std::printf(PADDING RED "     666666666      " NORM "         000000000          77777777            " NORM "\n", padding);

	// Success
	return true;
}

bool Environment::Drivers::screen::connectToScreen(BBP::std::size_t, BBP::std::word *)
{
	// Always works
	return true;
}

bool Environment::Drivers::screen::disconnectFromScreen(BBP::std::size_t, BBP::std::word *)
{
	return true;
}