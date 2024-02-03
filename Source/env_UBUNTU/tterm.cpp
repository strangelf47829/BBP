#include "../include/stdio.h"
#include "../include/Graphics.h"
#include "../include/OS.h"

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

	// Then print
	std::STDOUT <<= printfbuff;
	
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

void BBP::std::print_to_terminal(std::Stack<std::string_element> *stack)
{
	// Declare string
	BBP::std::c_string str;

	// Read string
	*stack >>= &str;
	
	// Read string length
	std::size_t strLength = std::strlen(str);

	// 'printf'
	::write(STDOUT_FILENO, str, strLength);
}

::std::thread inpThread;
volatile bool captureUserInput = false;
struct termios originalTermios;
struct termios rawTermios;

void saveTerminalState()
{
	
	// Save the current terminal settings
	tcgetattr(STDIN_FILENO, &originalTermios);

	// Set the terminal to raw mode
	rawTermios = originalTermios;
	rawTermios.c_lflag &= ~(ICANON | ECHO);
	tcsetattr(STDIN_FILENO, TCSANOW, &rawTermios);

	// Set non-blocking mode for stdin
	fcntl(STDIN_FILENO, F_SETFL, fcntl(STDIN_FILENO, F_GETFL) | O_NONBLOCK);
}

void restoreTerminalState()
{
	// Reset the terminal to its original state
	tcsetattr(STDIN_FILENO, TCSANOW, &originalTermios);
}

void userInput_thread()
{
	char c = 0;
	while (captureUserInput)
	{
		c = 0;
		read(STDIN_FILENO, &c, 1);

		if (!captureUserInput)
			break;

		if (c == 0)
			continue;

		if (c == 0x10)
		{
			restoreTerminalState();
			std::exit(-999);
		}

		BBP::std::STDIN << c;
	}
}

char BBP::std::getC()
{
	while (std::STDIN.atElement == 0);
	return '\0';
}

void BBP::std::captureInput()
{
	// Save terminal state
	saveTerminalState();

	// Create threads
	inpThread = ::std::thread(userInput_thread);

	// Capture user input
	captureUserInput = true;
}

void BBP::std::stopCapturingInput()
{
	// Stop capturing user input
	captureUserInput = false;

	// Join the thread
	inpThread.join();

	// Restore terminal state
	restoreTerminalState();
}
