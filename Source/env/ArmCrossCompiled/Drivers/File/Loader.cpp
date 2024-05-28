#include "../../include/FileDriver.h"
#include "../../include/Environment.h"

void Environment::Drivers::Filesystem::loadFileSystem(BBP::system::DeviceDriver &driver)
{
	// Set commands
	driver.hardwareDriver.setHandleData(fileManifest.actions, fileManifest.commandCount, fileManifest.commands);
}