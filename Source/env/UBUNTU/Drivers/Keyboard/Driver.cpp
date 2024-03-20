#include "../../include/KeyboardDriver.h"
#include "../../include/Environment.h"

#include <cstdio>
#include <thread>
#include <termios.h>
#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <stdarg.h>

BBP::std::Stack<BBP::std::string_element> Environment::Drivers::keyboard::keyboardStack(nullptr, 0);

::std::thread inpThread;
volatile bool Environment::Drivers::keyboard::captureUserInput = false;
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
	while (Environment::Drivers::keyboard::captureUserInput)
	{
		c = 0;
		read(STDIN_FILENO, &c, 1);

		if (!Environment::Drivers::keyboard::captureUserInput)
			break;

		if (c == 0)
			continue;

		if (c == 0x10)
		{
			restoreTerminalState();
			std::exit(-999);
		}

		BBP::system::getKernelInstance().getKeyboardDriver().softwareDriver < c;
	}
}

void Environment::Drivers::keyboard::captureInput()
{
	// Save terminal state
	saveTerminalState();

	// Create threads
	inpThread = ::std::thread(userInput_thread);

	// Capture user input
	captureUserInput = true;
}

void Environment::Drivers::keyboard::stopCapturingInput()
{
	// Stop capturing user input
	captureUserInput = false;

	// Join the thread
	inpThread.join();

	// Restore terminal state
	restoreTerminalState();
}