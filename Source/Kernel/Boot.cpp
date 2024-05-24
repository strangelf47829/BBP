#include "../include/Kernel.h"
#include "../include/Daemon.h"
#include "../include/StackTrace.h"

// Kernel license info
BBP::system::appInfo kernelLicense = BBP::system::appInfo(1, 0, 0, 0);

BBP::std::errno_t BBP::system::Kernel::enterKernelSpace(system::UEFI &uefi)
{
	// Set uefi
	singleton.sysconfig = &uefi;

	// Check if uefi post is available
	if (uefi.post.basicPost == nullptr)
		return invalidConfig; // Invalid configuration: no POST available.

	// Perform POST
	std::word basicPOST = uefi.post.basicPost();

	// If post failed, return
	if (basicPOST)
		return failedPOST;

	// Load system driver
	std::errno_t systemDriverStatus = singleton.loadSystemDriver(uefi);

	// If set, return
	if (systemDriverStatus)
		return systemDriverStatus;

	// Load file driver
	std::errno_t fileDriverStatus = singleton.loadFileDriver(uefi);

	// If set, return
	if (fileDriverStatus)
		return fileDriverStatus;

	// Load screen driver
	std::errno_t screenDriverStatus = singleton.loadScreenDriver(uefi);

	// If set, return
	if (screenDriverStatus)
		return screenDriverStatus;

	// Load keyboard driver
	std::errno_t keyboardDriverStatus = singleton.loadKeyboardDriver(uefi);

	// If set, return
	if (keyboardDriverStatus)
		return keyboardDriverStatus;
	
	// If more drivers available, set those
	if (uefi.drivers.getOtherDrivers)
	{
		// Allocate pages
		std::PAGE<UEFILoadDriver> otherDrivers;
		std::size_t driverCount = uefi.drivers.getOtherDrivers(otherDrivers);

		// Allocate new data
		singleton.allocator.page_calloc(singleton.externalDrivers, driverCount);

		// Now loop over
		for (std::index_t idx = 0; idx < driverCount; idx++)
		{
			// Check if valid loader
			if (otherDrivers[idx] == nullptr)
				continue;

			// Create new driver
			DeviceDriver *newDriver = singleton.allocator.add_object(new DeviceDriver(nullptr, nullptr, nullptr, nullptr, 0, nullptr));
			
			// Load driver
			otherDrivers[idx](*newDriver);
		}
	}

	// Keep track of registered daemons.
	std::size_t registeredDaemons = 0;

	// Look through the list of daemons.
	for (std::index_t daemonIndex = 0; daemonIndex < uefi.daemons.records.dataSize; daemonIndex++)
	{
		// If it is not possible to register daemons, exit out here
		if (system::DaemonRecord::records.dataSize <= registeredDaemons)
			return outOfDaemonRecords;

		// Get a pointer to the daemon
		system::DaemonRecord *record = &uefi.daemons.records[daemonIndex];

		// Check if daemon can be loaded
		if (record->owner == nullptr)
			continue;

		// Continue loading daemon. Get status
		std::errno_t status = record->owner->initialize();

		// Add record to static stuff
		system::DaemonRecord::records.data[registeredDaemons++] = *record;
	}

	// Check if configuration supports boot records
	if (uefi.post.retrieveBootrecords == nullptr)
		return invalidConfig;

	// Get list
	std::PAGE<BootRecord *> bootrecords;
	std::size_t bootRecordCount = uefi.post.retrieveBootrecords(bootrecords);

	// If no boot records exist, return
	if (bootRecordCount == 0)
		return noBootRecords;

	BootRecord::bootRecordEntryPoint entryPoint = nullptr;

	// Save BIOS info
	bool canBootIntoBIOS = false;
	BootRecord::bootRecordEntryPoint BIOSEntry = nullptr;

	// Loop until viable boot record is found
	for (std::index_t bootRecordIndex = 0; bootRecordIndex < bootRecordCount; bootRecordIndex++)
	{
		// Check if boot record can be loaded
		bootrecords[bootRecordIndex]->computedValue = bootrecords[bootRecordIndex]->isBootRecordAvailable(&uefi);
		if (bootrecords[bootRecordIndex]->computedValue == false)
			continue;

		if (bootRecordIndex == 0)
		{
			canBootIntoBIOS = true;
			BIOSEntry = bootrecords[bootRecordIndex]->entryPoint;
		}

		// Set boot record entry point
		entryPoint = bootrecords[bootRecordIndex]->entryPoint;

		// Break if not 0
		if (bootRecordIndex != 0)
			break;
	}

	// Check if could boot
	if (entryPoint == nullptr)
		return noBootRecords;

	// Load license info
	uefi.licenses.KernelInfo = kernelLicense;
	uefi.licenses.KernelName = "BBP";

	// Initialize clock
	singleton.initClock();

	// Clear screen
	clearScreen();

	// Show BIOS splash screen
	biosSplashCommand(uefi);

	// Set delay
	std::time_t delay = (canBootIntoBIOS ? uefi.post.biosModeDelay : 0);
	std::time_t calledAt = millis();

	// Key to enter BIOS
	std::string_element enterBios = uefi.post.biosModeKey;
	std::string_element bootSelect = '\t';

	//std::printf("Press <tab> to enter boot selection menu.\n");

	// Print BIOS message. Boot record should be 0.
	//if (canBootIntoBIOS)
		//std::printf("Press <%c> to enter '%s'.\n", enterBios, bootrecords[0]->recordName.data);

	// While this delay is set
	do
	{
		// Get key input
		std::string_element keyInp = getKeyAsync();

		// If not equal to anyting just continue
		if (keyInp != enterBios && keyInp != bootSelect)
			continue;

		// Otherwise, do action
		if (keyInp == enterBios)
		{
			// Set stuff
			std::printf("Entering BIOS mode...\n");
			entryPoint = BIOSEntry;
			break;
		}

		// Otherwise enter boot selection
		entryPoint = singleton.enterBootSelection(bootrecords, entryPoint, BIOSEntry);
		break;

	} while (millis() <= calledAt + delay);

	// Clear screen
	clearScreen();

	// Create task
	singleton.kernelUpdateCycleTask.SetFunctor(kernelUpdateCycle);

	// Add task to taskpool
	singleton.taskpool.Add(singleton.kernelUpdateCycleTask());

	// Load entry point
	bool couldEnter = entryPoint(&uefi, singleton.extTaskPool);

	// Start tracing
	std::trace();

	// If could not enter, return error
	if (couldEnter == false)
		return OSError;

	// Otherwise, continue until cold
	while (singleton.taskpool.Step());

	// Then unload all drivers
	singleton.stopCapture();

	// Return
	return 0;
}