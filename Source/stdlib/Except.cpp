#include "../include/stdio.h"
#include "../include/Except.h"
#include "../include/Kernel.h"
#include "../include/StackTrace.h"
#include "../include/errnoStrings.h"

// If busy handling exception
bool handling = false;

BBP::std::except BBP::std::exception(const char *msg, errno_t _errno)
{
	// If already handling throw kernel signal instead
	if (handling)
		std::panic();

	// Flag handling
	handling = true;

	// set error number
	system::Kernel::setError(_errno);

	// Get length of message
	std::size_t msgLen = std::strlen(msg);

	// String to store errno message
	std::static_string<50> errnoMessage;

	// Get errno
	std::strerror(_errno, errnoMessage);

	// Print what happened
	std::printf("An exception has occured: %s: %s. Showing stack trace:\n", errnoMessage.data, msg);

	// Print where it happened
	stack_trace trace;
	trace.Capture();
	trace.showStackTrace();

	// Done handling
	handling = false;

	// Create object and return
	except exc = {_errno};
	return exc;
}