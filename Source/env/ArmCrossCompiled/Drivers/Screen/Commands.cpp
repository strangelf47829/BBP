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
		BBP::system::UEFI *uefi = (BBP::system::UEFI *)argv;

		// If TTY is supported, and at least something was found
		if (uefi->systemReport.supportsTTY && uefi->systemReport.TTYHorizontalPage)
		{
			// Calculate padding accordingly.
			BBP::std::size_t leftPadding = ((uefi->systemReport.TTYHorizontalPage - sizeof("+== " ".... BIOS" " ====" "===============================================+")) - 5) / 2;

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
		if (uefi->systemReport.totalMemory > 1024 * 1024)
			BBP::std::sprintf(totalRamStr.data, "%dGiB", uefi->systemReport.totalMemory / (1024 * 1024));
		else if (uefi->systemReport.totalMemory > 1024)
			BBP::std::sprintf(totalRamStr.data, "%dMiB", uefi->systemReport.totalMemory / (1024));
		else
			BBP::std::sprintf(totalRamStr.data, "%dKiB", uefi->systemReport.totalMemory);

		// Sprintf free ram
		if (uefi->systemReport.usefulHeap > 1024 * 1024)
			BBP::std::sprintf(freeRamStr.data, "%dGiB", uefi->systemReport.usefulHeap / (1024 * 1024));
		else if (uefi->systemReport.usefulHeap > 1024)
			BBP::std::sprintf(freeRamStr.data, "%dMiB", uefi->systemReport.usefulHeap / (1024));
		else
			BBP::std::sprintf(freeRamStr.data, "%dKiB", uefi->systemReport.usefulHeap);

		// Print device name, etc...
		BBP::std::printf(PADDING RED "+== " NORM "%s BIOS" RED " ====" NORM "===============================================+" NORM "\n", padding, uefi->systemReport.HostName);
		BBP::std::printf(PADDING NORM "Device name: %s" "\n", padding, uefi->system.deviceName);
		BBP::std::printf(PADDING NORM "Primary volume '%c' mounted at '%s'" "\n", padding, uefi->system.volumeLabel, uefi->system.volumePath);
		BBP::std::printf(PADDING NORM "System memory: %s (%s useful heap)" "\n", padding, totalRamStr.data, freeRamStr.data);
		
		// Spacing
		BBP::std::printf(PADDING NORM "\n", padding);

		// Print Video mode
		if (uefi->systemReport.supportsTTY && uefi->systemReport.supportsGUI)
			BBP::std::printf(PADDING NORM "Video mode: mixed" "\n", padding);
		else if (uefi->systemReport.supportsTTY)
			BBP::std::printf(PADDING NORM "Video mode: TTY" "\n", padding);
		else if (uefi->systemReport.supportsGUI)
			BBP::std::printf(PADDING NORM "Video mode: GUI" "\n", padding);
		else
			BBP::std::printf(PADDING NORM "Video mode: " RED "ERROR" NORM "\n", padding);

		// Print video capabilities
		if (uefi->systemReport.supportsTTY && uefi->systemReport.supportsGUI == false)
			BBP::std::printf(PADDING NORM "TTY Resolution %u x %u characters" "\n", padding, uefi->systemReport.TTYHorizontalPage, uefi->systemReport.TTYVerticalPage);

		else if (uefi->systemReport.supportsGUI && uefi->systemReport.supportsTTY == false)
			BBP::std::printf(PADDING NORM "%uB VMEM: GUI %u x %u Pixels (colour depth %u)" "\n", padding, uefi->systemReport.VMEMSize, uefi->systemReport.xResolution, uefi->systemReport.yResolution, uefi->systemReport.colourDepth);

		else if (uefi->systemReport.supportsGUI && uefi->systemReport.supportsTTY)
			BBP::std::printf(PADDING NORM "%uB VMEM: GUI/TTY %u x %u x %u Pixels / %u x %u TTY" "\n", padding, uefi->systemReport.VMEMSize, uefi->systemReport.xResolution, uefi->systemReport.yResolution, uefi->systemReport.colourDepth, uefi->systemReport.TTYHorizontalPage, uefi->systemReport.TTYVerticalPage);
		else
			BBP::std::printf(PADDING NORM "Unkown video mode" "\n", padding);

		// Spacing
		BBP::std::printf(PADDING NORM "\n", padding);

		// Printing
		BBP::std::printf(PADDING NORM "Enter BIOS: <" "\e[0;93m" "%c" NORM ">" " ==" NORM "== Boot Selection <" "\e[0;93m" "tab" NORM ">" "\n", padding, uefi->post.biosModeKey);

		// Spacing
		BBP::std::printf(PADDING NORM "\n", padding);

		// Print Kernel info
		BBP::std::printf(PADDING NORM "%s Kernel Version %u.%u.%u.%u (%s) (C) %u %s" "\n", padding, uefi->licenses.KernelName, uefi->licenses.KernelInfo.build.majorVersion, uefi->licenses.KernelInfo.build.minorVersion, uefi->licenses.KernelInfo.build.buildVersion, uefi->licenses.KernelInfo.build.patchVersion, uefi->licenses.KernelInfo.build.extraInfo, uefi->licenses.KernelInfo.copyright.copyrightYear, uefi->licenses.KernelInfo.copyright.copyrightHolder);

		// Print BIOS info
		BBP::std::printf(PADDING NORM "BIOS Version %u.%u.%u.%u (%s) (C) %u %s" "\n", padding, uefi->licenses.BIOSInfo.build.majorVersion, uefi->licenses.BIOSInfo.build.minorVersion, uefi->licenses.BIOSInfo.build.buildVersion, uefi->licenses.BIOSInfo.build.patchVersion, uefi->licenses.BIOSInfo.build.extraInfo, uefi->licenses.BIOSInfo.copyright.copyrightYear, uefi->licenses.BIOSInfo.copyright.copyrightHolder);

		// Licensing info
		BBP::std::printf(PADDING NORM "Released under %s. More info at %s" "\n", padding, uefi->licenses.KernelInfo.license.licenseName, uefi->licenses.KernelInfo.license.licenseURL);

		// Print bottom
		BBP::std::printf(PADDING RED "+=======================" NORM "===============================================+" NORM "\n" "\n", padding);

		// Print top of BB with info
		BBP::std::printf(PADDING RED "BBBBBBBBBBBBBBBBB   BBBBBBBBBBBBBBBBB" NORM "\n", padding);
		BBP::std::printf(PADDING RED "B::::::::::::::::B  B::::::::::::::::B  " NORM PROCPADDING RED "Processor info" NORM " : " "\n", padding);
		BBP::std::printf(PADDING RED "B::::::BBBBBB:::::B B::::::BBBBBB:::::B " NORM PROCPADDING RED "Speed" NORM " - % uMHz" "\n", padding, uefi->systemReport.processorSpeed);
		BBP::std::printf(PADDING RED "BB:::::B     B:::::BBB:::::B     B:::::B" NORM PROCPADDING RED "Arch" NORM " - %s" "\n", padding, uefi->systemReport.processorArch.data);
		BBP::std::printf(PADDING RED "  B::::B     B:::::B  B::::B     B:::::B" NORM PROCPADDING RED "Name" NORM " - %s" "\n", padding, uefi->systemReport.processorName.data);
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