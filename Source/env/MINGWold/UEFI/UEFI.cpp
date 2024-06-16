#include "../include/Environment.h"
#include "../include/BIOS.h"
#include "../../../../Source/include/Daemon.h"
#include "../../../../Source/include/Strings.h"
#include "../../../../Source/include/DaemonRegistry.h"

// Daemons
BBP::system::initd initdD;

BBP::system::EFI ubuntuEFI;

BBP::system::DaemonRecord records[2] = { BBP::system::DaemonRecord(&initdD, "initd"), { BBP::system::DaemonRecord(&initdD, "winmand")} };

BBP::system::EFI &BBP::system::retrieveEFI()
{
	// Configure post actions
	ubuntuEFI.post.basicPost = Environment::BIOS::POST;
	ubuntuEFI.post.biosModeKey = 'f';
	ubuntuEFI.post.biosModeDelay = 2000;

	// Configure volume info
	ubuntuEFI.system.volumeLabel = 'v';
	ubuntuEFI.system.volumePath = "V:\\";

	// Configure root password
	ubuntuEFI.system.rootPassword = "root";
	ubuntuEFI.system.deviceName = "BBP";

	// Configure drivers
	ubuntuEFI.drivers.loadKeyboard = Environment::Drivers::keyboardManifest.loader;
	ubuntuEFI.drivers.loadScreen = Environment::Drivers::screenManifest.loader;
	ubuntuEFI.drivers.loadFileSystem = Environment::Drivers::fileManifest.loader;

	// Configure daemons
	ubuntuEFI.daemons.records = std::PAGE<system::DaemonRecord>(2, records);
	ubuntuEFI.daemons.specialIndicies = std::PAGE<std::index_t>(0, nullptr);
	ubuntuEFI.daemons.specialDaemons = 0;

	// Configure boot records
	ubuntuEFI.post.retrieveBootrecords = Environment::EFI::loadBootrecords;

	return ubuntuEFI;
}