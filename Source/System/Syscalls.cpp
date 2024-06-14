#include "../include/Syscalls.h"
#include "../include/SyscallList.h"
#include "../include/SyscallArgs.h"
#include "../include/drvcmd.h"
#include "../include/Kernel.h"

// Do a system call
BBP::std::errno_t BBP::system::Kernel::syscall(BBP::system::syscall_t system, BBP::system::syscall_args_t &args)
{
	// Lil' hack
	std::word *argv = (std::word *)(&args);

	// Get the system driver
	singleton.Core().firmware->HardwareSystem.hardwareDriver.executeCommand(std::systemCallCmd, system, argv);

	// Return result
	return args.errno;
}

void *BBP::systemcalls::malloc(std::size_t size)
{
	system::syscall_arg_data<void *> ReturnData = nullptr;
	system::syscall_arg_data<std::size_t> ArgumentData = size;

	// Create argument
	BBP::system::syscall_args_t arg(ReturnData, ArgumentData);

	// System call
	system::Kernel::syscall(systemcalls::system_malloc, arg);

	// Return result
	return ReturnData.template get<0>();
}

void *BBP::systemcalls::calloc(std::size_t count, std::size_t size)
{
	system::syscall_arg_data<void *> ReturnData = nullptr;
	system::syscall_arg_data<std::size_t, std::size_t> ArgumentData(count, size);

	// Create argument
	BBP::system::syscall_args_t arg(ReturnData, ArgumentData);

	// System call
	system::Kernel::syscall(systemcalls::system_calloc, arg);

	// Return result
	return ReturnData.template get<0>();
}

void BBP::systemcalls::free(void *ptr)
{
	system::syscall_arg_data<> ReturnData;
	system::syscall_arg_data<void *> ArgumentData = ptr;

	// Create argument
	BBP::system::syscall_args_t arg(ReturnData, ArgumentData);

	// System call
	system::Kernel::syscall(systemcalls::system_free, arg);
}

/*
BBP::std::word BBP::systemcalls::tripleNum(BBP::std::word num)
{
	// Create two numbers
	BBP::std::word result = 0;
	BBP::std::word argument = num;

	// Create buffer with addresses
	BBP::std::word *data[2] = { &argument, &result };

	// Create two pages
	BBP::std::PAGE<void *> arg_page(1, (void **)data);
	BBP::std::PAGE<void *> res_page(1, (void **)data + 1);

	// Create argument
	BBP::system::syscall_args_t arg = { ENONE, &res_page, &arg_page };

	// System call
	system::syscall(systemcalls::tripleNumber, arg);

	// Return result
	return result;
}
*/