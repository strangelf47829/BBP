#include "../include/Kernel.h"
#include "../include/UEFI.h"
#include "../include/Daemon.h"
#include "../include/Time.h"
#include "../include/DriverCommands.h"

// Keyboard driver stuff

namespace BBP
{
	namespace system
	{

		std::errno_t loadKeyboard(UEFI &configuration)
		{
			if (getKernelInstance().loadDriver(configuration.drivers.loadKeyboard, getKernelInstance().getKeyboardDriver()))
				return keyboardInitFailed;

			// Now send keyboard commands to driver
			bool keyboardActive = getKernelInstance().getKeyboardDriver().hardwareDriver.executeCommand(HardwareHandle::connectCmd, 0, nullptr);
			keyboardActive &= getKernelInstance().getKeyboardDriver().hardwareDriver.executeCommand(keyboardEnableInput, 0, nullptr);

			// If failed to activate keyboard, return error
			if (keyboardActive == false)
				return keyboardInitFailed;

			return 0;
		}

		std::errno_t loadScreen(UEFI &configuration)
		{
			if (getKernelInstance().loadDriver(configuration.drivers.loadScreen, getKernelInstance().getScreenDriver()))
				return screenInitFailed;

			// Now send commands to driver
			bool screenActive = getKernelInstance().getScreenDriver().hardwareDriver.executeCommand(HardwareHandle::connectCmd, 0, nullptr);

			// If failed to activate screen, return error
			if (screenActive == false)
				return screenInitFailed;

			return 0;
		}

		std::errno_t loadFileSystem(UEFI &configuration)
		{
			if (getKernelInstance().loadDriver(configuration.drivers.loadFileSystem, getKernelInstance().getFileDriver()))
				return fileSystemInitFailed;

			// Now send keyboard commands to driver
			bool fileSystemActive = getKernelInstance().getFileDriver().hardwareDriver.executeCommand(HardwareHandle::connectCmd, 0, nullptr);

			// If failed to activate keyboard, return error
			if (fileSystemActive == false)
				return fileSystemInitFailed;

			return 0;
		}

		std::errno_t loadEssentialDrivers(UEFI &configuration)
		{
			// Attempt to load Screen
			std::errno_t screenStatus = loadScreen(configuration);

			// If error, return
			if (screenStatus)
				return screenStatus;

			// Attempt to load File
			std::errno_t fileSystemStatus = loadFileSystem(configuration);

			// If error, return
			if (fileSystemStatus)
				return fileSystemStatus;

			// Attempt to load keyboard
			std::errno_t keyboardStatus = loadKeyboard(configuration);

			// If error, return
			if (keyboardStatus)
				return keyboardStatus;

			return 0;
		}

		// Boot selection
		void BootSelect(BootRecord::bootRecordEntryPoint &BIOS, BootRecord::bootRecordEntryPoint &entry, std::PAGE<BootRecord *> &bootrecords)
		{
			// Keep track of current selection
			std::index_t selection = 1;

			while (true)
			{

				// Clear the screen
				getKernelInstance().getScreenDriver().hardwareDriver.executeCommand(screenClearScreen, 0, nullptr);

				// Print help
				std::printf("Boot selection menu:\nUse the 'w' and 's' keys to move up and down to select an os to boot into.\n");
				std::printf("The default choice is marked with '\e[0;34m*\e[0;37m'. Options not available are marked with '\e[0;31mx\e[0;37m'.\n");
				std::printf("Your current selection is marked in \e[0;32mgreen\e[0;37m.\n\n");

				// Print out stuff
				for (std::index_t idx = 0; idx < bootrecords.dataSize; idx++)
				{
					//print out index
					std::printf("\t<%u>", idx);

					// If this boot record is the default, mark with *
					if (bootrecords[idx]->entryPoint == entry)
						std::printf("\e[0;34m*\e[0;37m");

					// If this boot record is unavailable, mark with x
					if (bootrecords[idx]->computedValue == false)
						std::printf("\e[0;31mx\e[0;37m");

					// If boot record is selected, make green if available
					if (idx == selection && bootrecords[idx]->computedValue == false)
						std::printf("\e[0;31m");
					else if (idx == selection)
						std::printf("\e[0;32m");

					// Print out name
					std::printf("\t%s\e[0;37m\n", bootrecords[idx]->recordName.data);
				}

				// Print out other tip
				std::printf("\n<enter>: Make selection\n<tab>: Default selection\n");

				// Get character
				std::string_element choice = BBP::std::getChar();
				
				// If tab, exit out
				if (choice == '\t')
					break;

				// If 'w', and selection allows it, move up
				if (choice == 's' && (selection + 1) < bootrecords.dataSize)
					selection++;

				// If 's', and selection allows it, move down
				if (choice == 'w' && selection)
					selection--;

				// If enter, exit out
				if (choice == 0x0a)
				{
					if (bootrecords[selection]->computedValue == false)
						continue;

					if (selection == 0)
						entry = BIOS;
					else
						entry = bootrecords[selection]->entryPoint;

					return;
				}

			}
		}

	}
}

// This is the entry point of the system
int BBP::system::bootloaderMain()
{
	// Retrieve the uefi configuration
	UEFI configuration = retrieveUEFI();

	// Get Post configuration
	if (configuration.post.basicPost == nullptr)
		return invalidConfig;

	// Get word
	std::word getPOSTResult = configuration.post.basicPost();

	// If post failed, send corresponding message
	if (getPOSTResult)
		return failedPOST;

	
	/***************************************
	*		STAGE 2: Load device drivers.  *
	****************************************/

	// Load drivers
	std::errno_t driverStatus = loadEssentialDrivers(configuration);
	if (driverStatus)
		return driverStatus;

	// If other drivers are listed,
	if (configuration.drivers.getOtherDrivers)
	{

		// Now get list of other available drivers
		std::PAGE<UEFILoadDriver> otherDrivers;
		std::size_t driverCount = configuration.drivers.getOtherDrivers(otherDrivers);

		// Allocate driver count
		getKernelInstance().allocateExternalDrivers(driverCount);

		// Attempt to load each driver
		for (std::index_t driverIndex = 0; driverIndex < driverCount; driverIndex++)
		{
			// Get function pointer
			UEFILoadDriver loadFunction = otherDrivers[driverIndex];

			// Load driver into kernel.
			std::errno_t kernelErr = getKernelInstance().loadDriver(loadFunction, getKernelInstance().getNextAvailableDriver());

			// Check for error
			if (kernelErr)
				return kernelErr;
		}
	}
	
	/***************************************
	*		STAGE 3: Load daemons.         *
	****************************************/

	// Keep track of registered daemons.
	std::size_t registeredDaemons = 0;

	// Look through the list of daemons.
	for (std::index_t daemonIndex = 0; daemonIndex < configuration.daemons.records.dataSize; daemonIndex++)
	{
		// If it is not possible to register daemons, exit out here
		if (system::DaemonRecord::records.dataSize <= registeredDaemons)
		{
			std::printf("\e[0;34m<bootloader>\e[0;37m Cannot register more daemons: Out of record space.\n");
			break;
		}

		// Get a pointer to the daemon
		system::DaemonRecord *record = &configuration.daemons.records[daemonIndex];

		// Check if daemon can be loaded
		if (record->owner == nullptr)
		{
			// Cannot load daemon
			std::printf("\e[0;34m<bootloader>\e[0;37m FATAL ERROR: Cannot load daemon \e[0;31m%s\e[0;37m\n", record->daemonName.data);
			continue;
		}

		// Continue loading daemon. Get status
		std::errno_t status = record->owner->initialize();

		// Add record to static stuff
		system::DaemonRecord::records.data[registeredDaemons++] = *record;
	}

	//std::printf("\e[0;34m<bootloader>\e[0;37m Loaded \e[0;31m%u\e[0;37m daemons.\n", registeredDaemons);

	/***************************************
	*		STAGE 4: Load boot records.    *
	****************************************/

	// Check if configuration supports boot records
	if (configuration.post.retrieveBootrecords == nullptr)
		return invalidConfig;

	// Get list
	std::PAGE<BootRecord*> bootrecords;
	std::size_t bootRecordCount = configuration.post.retrieveBootrecords(bootrecords);

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
		bootrecords[bootRecordIndex]->computedValue = bootrecords[bootRecordIndex]->isBootRecordAvailable(&configuration);
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

	/***************************************
	*		STAGE 5: Call BIOS.            *
	****************************************/

	// Initialize clock
	std::initClock();

	// Set delay
	std::time_t delay = (canBootIntoBIOS ? configuration.post.biosModeDelay : 0);
	std::time_t calledAt = std::millis();

	// Key to enter BIOS
	std::string_element enterBios = configuration.post.biosModeKey;
	std::string_element bootSelect = '\t';

	// Show boot selection thing
	getKernelInstance().getScreenDriver().hardwareDriver.executeCommand(screenClearScreen, 0, nullptr);
	std::printf("Press <tab> to enter boot selection menu.\n");

	// Print BIOS message. Boot record should be 0.
	if (canBootIntoBIOS)
		std::printf("Press <%c> to enter '%s'.\n", enterBios, bootrecords[0]->recordName.data);

	// Wait for keypress
	do
	{
		// Get keypress asynchronously.
		std::string_element c = std::getCharAsync();

		// If not enterBios, continue
		if (c != enterBios && c != bootSelect)
			continue;
		
		// If we want to enter BIOS, do that
		if (c == enterBios)
		{
			// Set stuff
			std::printf("Entering BIOS mode...\n");

			entryPoint = BIOSEntry;
		}

		// If we want to enter boot selection, do that.s
		if (c == bootSelect)
			BootSelect(BIOSEntry, entryPoint, bootrecords);

	} while (std::millis() <= calledAt + delay);

	// Clear the screen
	getKernelInstance().getScreenDriver().hardwareDriver.executeCommand(screenClearScreen, 0, nullptr);
	std::delay(1000);
	

	/***************************************
	*		STAGE 5: Call kernel.          *
	****************************************/

	// Now we have loaded the drivers and have gotten an entry point, boot the kernel.
	std::errno_t kernelStatus = entryPoint(&configuration);

	/***************************************
	*		STAGE 6: Disable stuff.        *
	****************************************/

	getKernelInstance().getKeyboardDriver().hardwareDriver.executeCommand(keyboardDisableInput, 0, nullptr);

	// Return kernel status
	return kernelStatus;
}