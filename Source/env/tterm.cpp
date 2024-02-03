#include "../include/stdio.h"
#include "../include/Graphics.h"
#include "../include/OS.h"
#include "../../build/include/SDL.h"
#include <cstdio>
#include <thread>
#include <iostream>
#include <fcntl.h>

void BBP::std::print_to_terminal(std::Stack<std::string_element> *stack)
{
	// Declare string
	BBP::std::c_string str;

	// Read string
	*stack >>= &str;

	// Read string length
	std::size_t strLength = std::strlen(str);

	// Print the string using R2D

	if (gINIT)
	{
		BBP::std::R2D::print(&BBP::std::kernelDisplay, str);

		BBP::std::Driver::drawWindow(&BBP::std::kernelDisplay);
	}
	else
		::write(STDOUT_FILENO, str, strLength);
	// 'printf'
	//::write(STDOUT_FILENO, str, strLength);
}

SDL_Thread *thr;
volatile bool listenForInput = false;

int inputThread(void *data)
{
	SDL_Event event;
	while (listenForInput) {
		while (SDL_PollEvent(&event)) {
			::std::printf("Hello!");
			if (event.type == SDL_KEYDOWN) {
				SDL_Keycode keyCode = event.key.keysym.sym;
				BBP::std::STDIN << (char)keyCode;
			}
		}
	}
	return 0;
}

void BBP::std::captureInput()
{
	return;
	listenForInput = true;
	thr = SDL_CreateThread(inputThread, "input", nullptr);
}

void BBP::std::stopCapturingInput()
{
	return;
	listenForInput = false;
	SDL_WaitThread(thr, NULL);
}

char BBP::std::getC()
{
	SDL_Event event;
	if (SDL_PollEvent(&event))
	{
		if (event.type == SDL_KEYDOWN) {
			SDL_Keycode keyCode = event.key.keysym.sym;
			BBP::std::STDIN << (char)keyCode;
			return (char)keyCode;
		}
	}
	return 0;
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