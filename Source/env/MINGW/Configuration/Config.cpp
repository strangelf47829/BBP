#include "../../include/EFIConfig.h"
#include "../include/Config.h"
#include "../../../../Source/include/DaemonRegistry.h"
#include "../../include/SystemDriver.h"
#include "../../include/FileSystemDriver.h"
#include "../../include/ScreenDriver.h"
#include "../../include/KeyboardDriver.h"
#include "../include/ScreenData.h"

// Daemons
BBP::system::initd initdD;

BBP::system::DaemonRecord records[2] = { BBP::system::DaemonRecord(&initdD, "initd"), { BBP::system::DaemonRecord(&initdD, "winmand")} };

// Declare static string
BBP::std::static_string<32> deviceName;
BBP::std::static_string<32> hostName;

BBP::std::word yes()
{
	return false;
}

void Host::configure(BBP::BIOS *bios, BBP::system::EFI &efi)
{
	// Configure post actions
	efi.post.basicPost = yes;
	efi.post.biosModeKey = 'f';
	efi.post.biosModeDelay = 5000;

	// Configure capabilities
	efi.systemReport.HostName = hostName.data;
	efi.systemReport.usefulHeap = Host::getSystemHeap();
	efi.systemReport.totalMemory = Host::getSystemMemory();
	efi.systemReport.supportsTTY = true;
	efi.systemReport.supportsGUI = true;

	// Get TTY Resolutions
	efi.systemReport.TTYHorizontalPage = tty_x;
	efi.systemReport.TTYVerticalPage = tty_y;

	// Get GUI Data
	efi.systemReport.xResolution = w;
	efi.systemReport.yResolution = h;

	// Get GOP data
	efi.systemReport.GOP = Host::VBuff;

	// Then get CPU info
	Host::getCPUData(&efi);

	// Load device name
	getHostValue(deviceName, "uname -n");
	getHostValue(hostName, "uname -o");

	// Load Epoch
	//getHostValue(efi.systemReport.epochLower, "date +%s");

	// Configure volume info
	efi.system.volumeLabel = 'v';
	efi.system.volumePath = "V:\\";

	// Configure root password
	efi.system.rootPassword = "root";
	efi.system.deviceName = deviceName.data;

	// Configure drivers
	efi.drivers.loadKeyboard = Host::Drivers::Keyboard::loadKeyboardDriver;
	efi.drivers.loadScreen = Host::Drivers::Screen::loadScreenDriver;
	efi.drivers.loadFileSystem = Host::Drivers::FileSystem::loadFileSystem;
	efi.drivers.loadSystem = Host::Drivers::System::loadSystemDriver;
	efi.drivers.getOtherDrivers = nullptr;
	efi.drivers.loadOtherDrivers = nullptr;

	// Configure daemons
	efi.daemons.records = BBP::std::PAGE<BBP::system::DaemonRecord>(2, records);
	efi.daemons.specialIndicies = BBP::std::PAGE<BBP::std::index_t>(0, nullptr);
	efi.daemons.specialDaemons = 0;

	// Configure boot records
	efi.post.retrieveBootrecords = loadBootrecords;

	// Licensing info
	efi.licenses.BIOSInfo = BBP::system::appInfo(1, 0, 0, 0);
}