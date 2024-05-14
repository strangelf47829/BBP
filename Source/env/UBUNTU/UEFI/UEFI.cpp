#include "../include/Environment.h"
#include "../include/BIOS.h"
#include "../../../../Source/include/Daemon.h"
#include "../../../../Source/include/Strings.h"
#include "../../../../Source/include/DaemonRegistry.h"

// Daemons
BBP::system::initd initdD;

BBP::system::UEFI ubuntuUEFI;

BBP::system::DaemonRecord records[2] = { BBP::system::DaemonRecord(&initdD, "initd"), { BBP::system::DaemonRecord(&initdD, "winmand")} };

BBP::system::UEFI &BBP::system::retrieveUEFI()
{
	// Configure post actions
	ubuntuUEFI.post.basicPost = Environment::BIOS::POST;
	ubuntuUEFI.post.biosModeKey = 'f';
	ubuntuUEFI.post.biosModeDelay = 2000;

	// Configure volume info
	ubuntuUEFI.system.volumeLabel = 'v';
	ubuntuUEFI.system.volumePath = "/mnt/v/";

	// Configure root password
	ubuntuUEFI.system.rootPassword = "root";
	ubuntuUEFI.system.deviceName = "BBP";

	// Configure drivers
	ubuntuUEFI.drivers.loadKeyboard = Environment::Drivers::keyboardManifest.loader;
	ubuntuUEFI.drivers.loadScreen = Environment::Drivers::screenManifest.loader;
	ubuntuUEFI.drivers.loadFileSystem = Environment::Drivers::fileManifest.loader;
	ubuntuUEFI.drivers.loadSystem = Environment::Drivers::systemManifest.loader;

	// Configure daemons
	ubuntuUEFI.daemons.records = std::PAGE<system::DaemonRecord>(2, records);
	ubuntuUEFI.daemons.specialIndicies = std::PAGE<std::index_t>(0, nullptr);
	ubuntuUEFI.daemons.specialDaemons = 0;

	// Configure boot records
	ubuntuUEFI.post.retrieveBootrecords = Environment::UEFI::loadBootrecords;

	return ubuntuUEFI;
}