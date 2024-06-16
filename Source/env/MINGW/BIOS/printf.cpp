#include "../../../include/stdio.h"
#include "../../../include/Kernel.h"

#include <cstdio>
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