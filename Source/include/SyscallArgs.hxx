
#include "Except.h"

#ifndef BBP_SYSCALLS_ARGS_H
#include "SyscallArgs.h"
#endif

#ifndef BBP_SYSCALLS_ARGS_HXX
#define BBP_SYSCALLS_ARGS_HXX

template<typename T>
T &BBP::system::getInValue(syscall_args_t &args, std::index_t idx)
{
	// Check argument count
	if (idx >= std::seqlen(*(args.argumentValues)))
		throw std::exception("Invalid argument index for syscall", ENODATA);
	 
	// Get pointer to argument
	T *argumentPointer = (T *)(*args.argumentValues)[idx];

	// If null pointer, throw error
	if (argumentPointer == nullptr)
		throw std::exception("Argument supplied is nullptr", EBADR);

	// Return dereferenced item
	return *argumentPointer;
}

template<typename T>
T &BBP::system::getOutValue(syscall_args_t &args, std::index_t idx)
{
	// Get out value count
	if (idx >= std::seqlen(*(args.returnValues)))
		throw std::exception("Invalid outval index for syscall", ENODATA);

	// Get pointer to out value
	T *outValuePointer = (T *)(*args.returnValues)[idx];

	// If null pointer, throw error
	if (outValuePointer == nullptr)
		throw std::exception("Out value supplied is nullptr", EBADR);

	// Return dereferenced item
	return *outValuePointer;
}

#endif