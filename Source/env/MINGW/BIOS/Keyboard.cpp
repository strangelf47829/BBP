#include "../include/BIOS.h"
#include "../include/SDL2/SDL.h"

// Listen for input
bool listenForInput = false;

// Returns true if key pressed, otherwise false
bool Host::BIOS::getKeyboardInput(BBP::std::string_element &ch)
{
	// If not listening for inputs return false
	if (listenForInput == false)
		return false;

	// Pump SDL events
	SDL_PumpEvents();
	
	// Amount of values
	int numkeys = 0;

	// Get keyboard data
	const BBP::std::byte *keyData = SDL_GetKeyboardState(&numkeys);

	// For each key look
	for (BBP::std::index_t idx = 0; idx < numkeys; idx++)
	{
		if (keyData[idx])
			ch = idx;
		input
	}

	return false;
}

// Start capturing keyboard
void Host::BIOS::listenForKeyboard()
{
	listenForInput = true;
}

// Stop capturing keyboard
void Host::BIOS::stopListeningForKeyboard()
{
	listenForInput = false;
}