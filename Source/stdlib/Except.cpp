#include "../include/stdio.h"
#include "../include/Except.h"
#include "../include/Kernel.h"
#include "../include/StackTrace.h"
#include "../include/errnoStrings.h"

// Current log level
BBP::std::byte loglevel = 1;

// If busy handling exception
bool handling = false;

// Loglevel getters/setters
BBP::std::byte BBP::std::getExceptionLogLevel() { return loglevel; }
void BBP::std::setExceptionLogLevel(BBP::std::byte level) { loglevel = level; }

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

	// Print where it happened
	stack_trace trace;
	trace.Capture();
	trace.moveUp();

	// Print what happened on loglevel > 0
	if (loglevel > 0)
		std::printf("An exception has occured at 0x%08x<0x%x>: %s: %s.", trace.activeFrame.functionAddress, trace.activeFrame.referenceAddress, errnoMessage.data, msg);

	// Show stack trace on loglevel > 1
	if (loglevel > 1)
		trace.showStackTrace();

	// Done handling
	handling = false;

	// Create object and return
	except exc = {_errno};
	return exc;
}
