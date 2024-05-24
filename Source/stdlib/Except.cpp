#include "../include/stdio.h"
#include "../include/Except.h"
#include "../include/Kernel.h"



BBP::std::except BBP::std::exception(const char *msg, errno_t _errno)
{
	// set error number
	system::Kernel::setError(_errno);

	// Get length of message
	std::size_t msgLen = std::strlen(msg);

	// Check if STDERR has enough capacity to stream message. If it does, stream message into STDERR
	//if (BBP::system::kernelSS()->activeContext->STDERR.max_elements - BBP::system::kernelSS()->activeContext->STDERR.atElement > msgLen)
	//	BBP::system::kernelSS()->activeContext->STDERR << "\e[0;31mError: " << msg <<= "\e[0;37m";

	std::printf("Error: %s\n", msg);

	except exc = {_errno};

	return exc;
}