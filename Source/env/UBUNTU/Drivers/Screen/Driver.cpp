#include "../../include/ScreenDriver.h"
#include "../../include/Environment.h"

BBP::std::Stack<BBP::std::string_element> Environment::Drivers::screen::stdoutStack(nullptr, 0);

#include <cstdio>
#include <thread>
#include <termios.h>
#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <stdarg.h>

BBP::std::static_string<BBP::std::max_page_elements> printfbuff;
BBP::std::static_string<BBP::std::max_page_elements> sprintfbuff;

int BBP::std::printf(std::conststring format, ...)
{
	// Format the string
	va_list args;
	va_start(args, format);
	int res = ::vsprintf(printfbuff.data, format, args);
	va_end(args);

	// Then print to STDOUT
	BBP::system::Kernel::printString(printfbuff);

	return res;
}

int BBP::std::putf(std::Stack<std::string_element> &stream, std::conststring format, ...)
{
	// Format the string
	va_list args;
	va_start(args, format);
	int res = ::vsprintf(sprintfbuff.data, format, args);
	va_end(args);

	// Then print
	stream <<= sprintfbuff;

	return res;
}

int BBP::std::sprintf(std::c_string buffer, std::conststring format, ...)
{
	va_list args;
	va_start(args, format);
	int res = ::vsprintf(buffer, format, args);
	va_end(args);

	return res;
}

void Environment::Drivers::screen::print_stack_to_string(BBP::std::Stack<BBP::std::string_element> *stack)
{
	// Declare string
	BBP::std::c_string str;

	// Get current index before read
	BBP::std::index_t preRead = stack->atElement;

	// Read string
	*stack >>= &str;

	// Get current index after read
	BBP::std::index_t postRead = stack->atElement;

	// Create string
	BBP::std::string strstr = str;

	// Write that data into software driver
	BBP::system::Kernel::printString(strstr);
}