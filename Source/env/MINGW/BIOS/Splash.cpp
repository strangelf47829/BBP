#include "../include/BIOS.h"
#include "../include/ScreenData.h"

// Splash string
BBP::std::conststring splashStr = \
"_____ _____ _____ " "\n"\
"| () )| () )| ()_)" "\n"\
"|_()_)|_()_)|_|   " "\n";

void Host::BIOS::splash(BBP::system::EFI &efi)
{
	// Clear screen
	clearScreen();

	// Print splash, then several newlines
	print(splashStr);
	printCharacter('\n');

	// Print system info
	print("BBP BIOS(C) ");

	// Print copyright year and holder
	print(efi.licenses.BIOSInfo.copyright.copyrightYear);
	printCharacter(' ');
	print(efi.licenses.BIOSInfo.copyright.copyrightHolder);
	printCharacter('\n');

	// Print license info
	print("License ");
	print(efi.licenses.BIOSInfo.license.licenseName);
	print(": ");
	print(efi.licenses.BIOSInfo.license.licenseInfo);
	print(" <");
	print(efi.licenses.BIOSInfo.license.licenseURL);
	print(">\n\n");

	// Print BIOS information
	print("BIOS ");
	print(efi.licenses.BIOSInfo.build.extraInfo);
	print(" version ");
	print(efi.licenses.BIOSInfo.build.majorVersion);
	printCharacter('.');
	print(efi.licenses.BIOSInfo.build.minorVersion);
	printCharacter('.');
	print(efi.licenses.BIOSInfo.build.patchVersion);
	printCharacter('(');
	print(efi.licenses.BIOSInfo.build.buildVersion);
	print(") ");

	// Print BIOS compile date
	print(efi.licenses.BIOSInfo.build.buildDate.data);

	// Print BIOS architecture
	print(" (Compiled for " ARCH ")\n");

	// Print device name
	print("Device: ");
	print(efi.system.deviceName);
	printCharacter('\n');

	// Print system capabilities
	print("CPU: ");
	print(efi.systemReport.processorName.data);
	print(" @ ");
	print(efi.systemReport.processorSpeed);
	print("MHz\n\n");

	// Print memory capabilities
	print("Total memory: ");
	printMem(efi.systemReport.totalMemory);

	print(" (Available heap: ");
	printMem(efi.systemReport.usefulHeap);
	print(")\n");

	// Print graphics modes
	print("Supported video modes: ");

	if (efi.systemReport.supportsTTY)
	{
		// Print TTY info
		print("TTY (");

		// Print resolution
		print(efi.systemReport.TTYHorizontalPage);
		printCharacter('x');
		print(efi.systemReport.TTYVerticalPage);

		// Closing parentheses
		print(") ");
	}
	if (efi.systemReport.supportsGUI)
	{
		// Print TTY info
		print("GUI (");

		// Print resolution
		print(efi.systemReport.xResolution);
		printCharacter('x');
		print(efi.systemReport.yResolution);

		// Closing parentheses
		print(") ");
	}

	// Draw display
	drawDisplay();
}