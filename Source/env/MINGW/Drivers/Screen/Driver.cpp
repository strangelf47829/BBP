#include "../../include/ScreenDriver.h"
#include "../../include/Environment.h"
#include "../../include/SDL2/SDL.h"

BBP::std::Stack<BBP::std::string_element> Environment::Drivers::screen::stdoutStack(nullptr, 0);

#include <cstdio>
#include <thread>
#include <iostream>
#include <fcntl.h>

BBP::std::static_string<BBP::std::max_page_elements> printfbuff;
BBP::std::static_string<BBP::std::max_page_elements> sprintfbuff;

int BBP::std::printf(std::conststring format, ...)
{
	// Format the string
	va_list args;
	va_start(args, format);
	int res = ::vsprintf(printfbuff.data, format, args);
	va_end(args);

	// Then print
	system::getKernelInstance().getScreenDriver().softwareDriver << printfbuff.data;

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

	// Read string
	*stack >>= &str;

	// Print string by sending data (through driver)
	BBP::system::getKernelInstance().getScreenDriver().writeData(str);
}