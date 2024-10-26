#include "../include/BIOS.h"

// Headers to interact with terminal
#include <cstdio>
#include <thread>
#include <termios.h>
#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <stdarg.h>

// Threading data
::std::thread inpThread;
struct termios originalTermios;
struct termios rawTermios;

// Pointers to volatile bools
volatile bool *capture; // Should capture?
volatile bool captured; // Has captured
volatile BBP::std::string_element inp;

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
	// If capture is nullptr, do nothing
	while (capture == nullptr);

	// While waiting for input,
	while (*capture)
	{
		// Declare value and read
		char c = 0;
		read(STDIN_FILENO, &c, 1);

		// Set values
		inp = c;

		// If no longer needing to capture something, break
		if (!(*capture))
			break;

		// Otherwise, if nothing captured, do nothing
		if (inp == 0)
			continue;

		// Captured something, set flag
		captured = true;
	}
}

// Returns true if key pressed, otherwise false
bool Host::BIOS::getKeyboardInput(BBP::std::string_element &ch)
{
	// If not listening for input, return false
	if (listenForInput == false)
		return false;

	// Otherwise if nothing captured return false
	if (captured == false)
		return false;

	// Set 'ch' equal to inp, then reset flag
	ch = inp;
	captured = false;

	// Return true because something returned
	return true;
}

// Start capturing keyboard
void Host::BIOS::listenForKeyboard()
{
	// Save terminal state
	saveTerminalState();

	// Capture user input
	listenForInput = true;

	// Then set pointer
	capture = &listenForInput;

	// Create threads
	inpThread = ::std::thread(userInput_thread);
}

// Stop capturing keyboard
void Host::BIOS::stopListeningForKeyboard()
{
	// Stop capturing user input
	listenForInput = false;

	// Join the thread
	inpThread.join();

	// Restore terminal state
	restoreTerminalState();
}