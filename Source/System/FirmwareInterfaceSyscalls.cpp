#include "../include/Syscalls.h"
#include "../include/SyscallList.h"
#include "../include/SyscallArgs.h"
#include "../include/drvcmd.h"
#include "../include/FirmwareInterface.h"

// Do a system call
BBP::std::errno_t BBP::FirmwareInterface::syscall(BBP::system::syscall_t system, BBP::system::syscall_args_t &args)
{
	// Lil' hack
	std::word *argv = (std::word *)(&args);

	// Get the system driver
	HardwareSystem.hardwareDriver.executeCommand(std::systemCallCmd, system, argv);

	// Return result
	return args.errno;
}
