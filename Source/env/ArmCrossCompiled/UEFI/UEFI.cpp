#include "../include/Environment.h"
#include "../include/BIOS.h"
#include "../include/SysInfo.h"
#include "../../../../Source/include/Daemon.h"
#include "../../../../Source/include/Strings.h"
#include "../../../../Source/include/DaemonRegistry.h"

// Daemons
BBP::system::initd initdD;

BBP::system::EFI ubuntuEFI;

BBP::system::DaemonRecord records[2] = { BBP::system::DaemonRecord(&initdD, "initd"), { BBP::system::DaemonRecord(&initdD, "winmand")} };

BBP::system::EFI &Environment::EFI::configureEFI()
{
	// Configure post actions
	ubuntuEFI.post.basicPost = Environment::BIOS::POST;
	ubuntuEFI.post.biosModeKey = 'f';
	ubuntuEFI.post.biosModeDelay = 1000;

	// Configure capabilities
	ubuntuEFI.systemReport.HostName = "WSL-UNIX";
	ubuntuEFI.systemReport.usefulHeap = Environment::BIOS::totalFreeMemory();
	ubuntuEFI.systemReport.totalMemory = Environment::BIOS::totalSystemMemory();
	ubuntuEFI.systemReport.supportsTTY = true;
	ubuntuEFI.systemReport.supportsGUI = false;
	ubuntuEFI.systemReport.TTYHorizontalPage = Environment::BIOS::getDisplayCols();
	ubuntuEFI.systemReport.TTYVerticalPage = Environment::BIOS::getDisplayLines();

	// Then get CPU info
	Environment::BIOS::getCPUData(&ubuntuEFI);

	// Configure volume info
	ubuntuEFI.system.volumeLabel = 'v';
	ubuntuEFI.system.volumePath = "/mnt/v/";

	// Configure root password
	ubuntuEFI.system.rootPassword = "root";
	ubuntuEFI.system.deviceName = "BB607";

	// Configure drivers
	ubuntuEFI.drivers.loadKeyboard = Environment::Drivers::keyboardManifest.loader;
	ubuntuEFI.drivers.loadScreen = Environment::Drivers::screenManifest.loader;
	ubuntuEFI.drivers.loadFileSystem = Environment::Drivers::fileManifest.loader;
	ubuntuEFI.drivers.loadSystem = Environment::Drivers::systemManifest.loader;

	// Configure daemons
	ubuntuEFI.daemons.records = BBP::std::PAGE<BBP::system::DaemonRecord>(2, records);
	ubuntuEFI.daemons.specialIndicies = BBP::std::PAGE<BBP::std::index_t>(0, nullptr);
	ubuntuEFI.daemons.specialDaemons = 0;

	// Configure boot records
	ubuntuEFI.post.retrieveBootrecords = Environment::EFI::loadBootrecords;

	// Licensing info
	ubuntuEFI.licenses.BIOSInfo = BBP::system::appInfo(1, 0, 0, 0);

	return ubuntuEFI;
}