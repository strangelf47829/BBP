#include "../include/stdio.h"
#include "../include/Graphics.h"
#include "../include/OS.h"
#include "../../build/include/SDL.h"
#include <cstdio>
#include <thread>
#include <iostream>
#include <fcntl.h>
#include <Windows.h>

int BBP::std::printf(std::conststring format, ...)
{
	// Allocate a buffer
	std::string buffer(BBP::std::max_page_elements, (BBP::std::string_element *)BBP::std::activemem->calloc(BBP::std::max_page_elements, sizeof(BBP::std::string_element)));

	// Format the string
	va_list args;
	va_start(args, format);
	::vsprintf(buffer.data, format, args);
	va_end(args);

	// Then print
	std::STDOUT <<= buffer;

	// Then free the buffer
	BBP::std::activemem->free(buffer.data);
}

int BBP::std::putf(std::Stack<std::string_element> &stream, std::conststring format, ...)
{
	// Allocate a buffer
	std::string buffer(BBP::std::max_page_elements, (BBP::std::string_element *)BBP::std::activemem->calloc(BBP::std::max_page_elements, sizeof(BBP::std::string_element)));

	// Format the string
	va_list args;
	va_start(args, format);
	::vsprintf(buffer.data, format, args);
	va_end(args);

	// Then print
	stream <<= buffer;

	// Then free the buffer
	BBP::std::activemem->free(buffer.data);
}

int BBP::std::sprintf(std::c_string buffer, std::conststring format, ...)
{
	va_list args;
	va_start(args, format);
	return ::vsprintf(buffer, format, args);
	va_end(args);
}

// Handle for terminal emulator
HANDLE serialHandle;
volatile bool writeSomething = false;
volatile BBP::std::c_string outputstr;
volatile BBP::std::size_t outputstrlen;
volatile bool written = false;

void BBP::std::print_to_terminal(std::Stack<std::string_element> *stack)
{
	// Declare string
	BBP::std::c_string str;

	// Read string
	*stack >>= &str;

	// Read string length
	std::size_t strLength = std::strlen(str);

	outputstr = str;
	outputstrlen = strLength;

	writeSomething = true;
	while (written == false);
	written = false;

}

SDL_Thread *thr;
volatile bool listenForInput = false;

int inputThread(void *data)
{
	// Create serial object
	HANDLE h_serial;
	HANDLE outputSerial;

	// Create serial object
	h_serial = CreateFile("COM6", GENERIC_READ | GENERIC_WRITE, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	outputSerial = CreateFile("COM8", GENERIC_READ | GENERIC_WRITE, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

	// Configure input
	DCB dcbSerialParam = { 0 };
	dcbSerialParam.DCBlength = sizeof(dcbSerialParam);

	dcbSerialParam.BaudRate = CBR_57600;
	dcbSerialParam.ByteSize = 8;
	dcbSerialParam.StopBits = ONESTOPBIT;
	dcbSerialParam.Parity = NOPARITY;

	// Set configuration
	SetCommState(h_serial, &dcbSerialParam);
	SetCommState(outputSerial, &dcbSerialParam);

	// Set timeouts
	COMMTIMEOUTS timeout = { 0 };
	timeout.ReadIntervalTimeout = 60;
	timeout.ReadTotalTimeoutConstant = 60;
	timeout.ReadTotalTimeoutMultiplier = 15;
	timeout.WriteTotalTimeoutConstant = 60;
	timeout.WriteTotalTimeoutMultiplier = 8;

	// Apply timeouts
	SetCommTimeouts(h_serial, &timeout);
	SetCommTimeouts(outputSerial, &timeout);


	char sBuff[16 + 1] = { 0 };
	DWORD dwRead = 0;

	SDL_Event event;


	while (listenForInput) {
		if (ReadFile(h_serial, sBuff, 16, &dwRead, NULL))
		{
			SDL_PollEvent(&event);
			for (DWORD idx = 0; idx < dwRead; idx++)
			{
				BBP::std::STDIN << sBuff[idx];
			}
		}

		if (writeSomething)
		{
			DWORD amountWritten = 0;
			WriteFile(outputSerial, outputstr, outputstrlen, &amountWritten, NULL);
			writeSomething = false;
			written = true;
		}

	}

	// close serial
	CloseHandle(h_serial);
	CloseHandle(outputSerial);

	return 0;
}

void BBP::std::captureInput()
{
	
	listenForInput = true;
	thr = SDL_CreateThread(inputThread, "input", nullptr);
}

void BBP::std::stopCapturingInput()
{

	listenForInput = false;
	SDL_WaitThread(thr, NULL);
}

char BBP::std::getC()
{
	SDL_Event event;
	if (SDL_PollEvent(&event));

	while (std::STDIN.atElement == 0 && listenForInput)
		SDL_PollEvent(&event);

	if (listenForInput == false)
		std::STDIN << '\0';
	return '\0';
}

/*
#include <cstdio>
#include <thread>
#include <termios.h>
#include <iostream>
#include <unistd.h>
#include <fcntl.h>


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
*/