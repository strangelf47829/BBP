#include "../include/stdio.h"
#include "../include/Except.h"

volatile BBP::std::errno_t BBP::std::__errno = ENONE;


BBP::std::except BBP::std::exception(const char *msg, errno_t _errno)
{
	// set error number
	BBP::std::__errno = _errno;

	// Get length of message
	std::size_t msgLen = std::strlen(msg);

	// Check if STDERR has enough capacity to stream message. If it does, stream message into STDERR
	if (BBP::std::STDERR.max_elements - BBP::std::STDERR.atElement > msgLen)
		BBP::std::STDERR << "\e[0;31mError: " << msg << "\e[0;37m" <<= std::endl;

	except exc = {_errno};

	return exc;
}