#include "../../include/KeyboardDriver.h"
#include "../../include/Environment.h"
#include "../../include/SDL2/SDL.h"
#include "../../../../include/stdio.h"

BBP::std::Stack<BBP::std::string_element> Environment::Drivers::keyboard::keyboardStack(nullptr, 0);

SDL_Thread *thr;
volatile bool Environment::Drivers::keyboard::captureUserInput = false;

int inputThread(void *data)
{
	SDL_Event event;
	while (Environment::Drivers::keyboard::captureUserInput) {
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_KEYDOWN) {
				SDL_Keycode keyCode = event.key.keysym.sym;
				Environment::Drivers::keyboard::keyboardStack << (char)keyCode;
			}
		}
	}
	return 0;
}

void Environment::Drivers::keyboard::captureInput()
{
	return;
	Environment::Drivers::keyboard::captureUserInput = true;
	thr = SDL_CreateThread(inputThread, "input", nullptr);
}

void Environment::Drivers::keyboard::stopCapturingInput()
{
	return;
	Environment::Drivers::keyboard::captureUserInput = false;
	SDL_WaitThread(thr, NULL);
}