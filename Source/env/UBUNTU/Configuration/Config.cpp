#include "../../include/EFIConfig.h"
#include "../include/Config.h"
#include "../../../../Source/include/DaemonRegistry.h"

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
	efi.systemReport.supportsGUI = false;

	// Get TTY Resolutions
	BBP::std::word xres = 0;
	BBP::std::word yres = 0;
	getHostValue(xres, "tput cols");
	getHostValue(yres, "tput lines");

	efi.systemReport.TTYHorizontalPage = xres;
	efi.systemReport.TTYVerticalPage = yres;

	// Then get CPU info
	Host::getCPUData(&efi);

	// Load device name
	getHostValue(deviceName, "uname -n");
	getHostValue(hostName, "uname -o");

	// Load Epoch
	getHostValue(efi.systemReport.epochLower, "date +%s");

	// Configure volume info
	efi.system.volumeLabel = 'v';
	efi.system.volumePath = "/mnt/v/";

	// Configure root password
	efi.system.rootPassword = "root";
	efi.system.deviceName = deviceName.data;

	// Configure drivers
	//efi.drivers.loadKeyboard = Environment::Drivers::keyboardManifest.loader;
	//efi.drivers.loadScreen = Environment::Drivers::screenManifest.loader;
	//efi.drivers.loadFileSystem = Environment::Drivers::fileManifest.loader;
	//efi.drivers.loadSystem = Environment::Drivers::systemManifest.loader;

	// Configure daemons
	efi.daemons.records = BBP::std::PAGE<BBP::system::DaemonRecord>(2, records);
	efi.daemons.specialIndicies = BBP::std::PAGE<BBP::std::index_t>(0, nullptr);
	efi.daemons.specialDaemons = 0;

	// Configure boot records
	efi.post.retrieveBootrecords = loadBootrecords;

	// Licensing info
	efi.licenses.BIOSInfo = BBP::system::appInfo(1, 0, 0, 0);


}