#include "../../../include/Syscalls.h"
#include "../../../include/SystemDriver.h"
#include "../../../../include/Syscalls.h"
#include "../../../../include/SyscallArgs.h"
#include "../../../../include/SyscallList.h"
#include "../../include/EFIDriverInterface.h"
#include "../../include/BIOS.h"

#include <cstdint>

bool Host::Drivers::System::connectSystem(BBP::std::size_t, BBP::std::word *)
{
	// TODO: implement user logging in
	return true;
}

bool Host::Drivers::System::disconnectSystem(BBP::std::size_t, BBP::std::word *)
{
	// Always works
	return true;
}

bool Host::Drivers::System::handleSystemCall(BBP::std::size_t systemcall_, BBP::std::word *args_ptr)
{
	// Convert stuff
	BBP::system::syscall_t call = systemcall_;
	BBP::system::syscall_args_t *args = (BBP::system::syscall_args_t *)args_ptr;

	// Then handle system call
	switch (systemcall_)
	{
	case BBP::systemcalls::system_malloc:

		// Simple malloc
		BBP::system::getOutValue<void *>(*args, 0) = Host::syscalls::sys_malloc(BBP::system::getInValue<BBP::std::size_t>(*args, 0));

		return true;
	case BBP::systemcalls::system_calloc:

		// Simple calloc
		BBP::system::getOutValue<void *>(*args, 0) = Host::syscalls::sys_calloc(BBP::system::getInValue<BBP::std::size_t>(*args, 0), BBP::system::getInValue<BBP::std::size_t>(*args, 1));

		return true;
	case BBP::systemcalls::system_free:

		// Simple free
		Host::syscalls::sys_free(BBP::system::getInValue<void *>(*args, 0));

	default:

		return false;
	}
}

bool Host::Drivers::System::initClock(BBP::std::size_t argc, BBP::std::word *argv)
{
	// Does nothing -- Bios handled
	return false;
}

bool Host::Drivers::System::getClockMicros(BBP::std::size_t argc, BBP::std::word *argv)
{
	// Check if arguments exist
	if (argc == 0 || argv == nullptr)
		return false;

	// Get bios clock
	argv[0] = Host::getBIOS()->microseconds();

	// Return success
	return true;
}

bool Host::Drivers::System::getClockMillis(BBP::std::size_t argc, BBP::std::word *argv)
{
	// Check if arguments exist
	if (argc == 0 || argv == nullptr)
		return false;

	// Get bios clock
	argv[0] = Host::getBIOS()->milliseconds();

	// Return success
	return true;
}

bool Host::Drivers::System::getUnixTimestamp(BBP::std::size_t argc, BBP::std::word *argv)
{
	// If no args, do nothing
	if (argc == 0 || argv == nullptr)
		return false;

	// Get milliseconds and divide by 1000
	BBP::std::time_t secondsSinceStartup = Host::getBIOS()->milliseconds() / 1000;

	// Get two words for epoch
	BBP::std::word epochLower;
	BBP::std::word epochHigher;

	// Read timestamp at startup
	Host::getStartupUnixTimestamp(epochLower, epochHigher);

	// If only 1 argument asked, return epochLower + seconds
	if (argc == 1)
	{
		argv[0] = epochLower + secondsSinceStartup;
		return true;
	}

	// Else get 64 bit number for operations
	uint64_t epoch = ((uint64_t)(epochHigher)) << 32 | epochLower;

	// Add numbers
	epoch += secondsSinceStartup;

	// Unpack lower word
	epochLower = (BBP::std::word)(epoch & (0xFFFFFFFF));
	
	// Shift over
	epoch = epoch >> 32;

	// Unpack upper word
	epochHigher = (BBP::std::word)(epoch & (0xFFFFFFFF));

	// Assign
	argv[0] = epochLower;
	argv[1] = epochHigher;

	// Return true
	return true;
}

bool Host::Drivers::System::getStartupTimestamp(BBP::std::size_t argc, BBP::std::word *argv)
{
	// If no args, do nothing
	if (argc == 0 || argv == nullptr)
		return false;

	// Get two words for epoch
	BBP::std::word epochLower;
	BBP::std::word epochHigher;

	// Read timestamp at startup
	Host::getStartupUnixTimestamp(epochLower, epochHigher);

	// If only 1 argument asked, return epochLower + seconds
	if (argc == 1)
	{
		argv[0] = epochLower;
		return true;
	}

	// Assign
	argv[0] = epochLower;
	argv[1] = epochHigher;

	// Return true
	return true;
}