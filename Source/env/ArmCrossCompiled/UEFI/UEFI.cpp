#include "../include/Environment.h"
#include "../include/BIOS.h"
#include "../include/SysInfo.h"
#include "../../../../Source/include/Daemon.h"
#include "../../../../Source/include/Strings.h"
#include "../../../../Source/include/DaemonRegistry.h"

// Daemons
BBP::system::initd initdD;

BBP::system::UEFI ubuntuUEFI;

BBP::system::DaemonRecord records[2] = { BBP::system::DaemonRecord(&initdD, "initd"), { BBP::system::DaemonRecord(&initdD, "winmand")} };

BBP::system::UEFI &Environment::UEFI::configureUEFI()
{
	// Configure post actions
	ubuntuUEFI.post.basicPost = Environment::BIOS::POST;
	ubuntuUEFI.post.biosModeKey = 'f';
	ubuntuUEFI.post.biosModeDelay = 1000;

	// Configure capabilities
	ubuntuUEFI.systemReport.HostName = "WSL-UNIX";
	ubuntuUEFI.systemReport.usefulHeap = Environment::BIOS::totalFreeMemory();
	ubuntuUEFI.systemReport.totalMemory = Environment::BIOS::totalSystemMemory();
	ubuntuUEFI.systemReport.supportsTTY = true;
	ubuntuUEFI.systemReport.supportsGUI = false;
	ubuntuUEFI.systemReport.TTYHorizontalPage = Environment::BIOS::getDisplayCols();
	ubuntuUEFI.systemReport.TTYVerticalPage = Environment::BIOS::getDisplayLines();

	// Then get CPU info
	Environment::BIOS::getCPUData(&ubuntuUEFI);

	// Configure volume info
	ubuntuUEFI.system.volumeLabel = 'v';
	ubuntuUEFI.system.volumePath = "/mnt/v/";

	// Configure root password
	ubuntuUEFI.system.rootPassword = "root";
	ubuntuUEFI.system.deviceName = "BB607";

	// Configure drivers
	ubuntuUEFI.drivers.loadKeyboard = Environment::Drivers::keyboardManifest.loader;
	ubuntuUEFI.drivers.loadScreen = Environment::Drivers::screenManifest.loader;
	ubuntuUEFI.drivers.loadFileSystem = Environment::Drivers::fileManifest.loader;
	ubuntuUEFI.drivers.loadSystem = Environment::Drivers::systemManifest.loader;

	// Configure daemons
	ubuntuUEFI.daemons.records = BBP::std::PAGE<BBP::system::DaemonRecord>(2, records);
	ubuntuUEFI.daemons.specialIndicies = BBP::std::PAGE<BBP::std::index_t>(0, nullptr);
	ubuntuUEFI.daemons.specialDaemons = 0;

	// Configure boot records
	ubuntuUEFI.post.retrieveBootrecords = Environment::UEFI::loadBootrecords;

	// Licensing info
	ubuntuUEFI.licenses.BIOSInfo = BBP::system::appInfo(1, 0, 0, 0);

	return ubuntuUEFI;
}