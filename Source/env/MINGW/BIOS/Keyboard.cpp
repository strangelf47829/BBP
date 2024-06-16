#include "../include/BIOS.h"
#include "../include/SDL2/SDL.h"

// Listen for input
bool listenForInput = false;

// Mask
BBP::std::STATIC_PAGE<bool, 512> oldMask;

// Shifted values for 0-9
BBP::std::conststring numericalShifts = ")!@#$%^&*(";
BBP::std::conststring manualNormal = "-=;',/.[]\\";
BBP::std::conststring manualShifted = "_+:\"<>?{}|";

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

	// Set ch to 0
	ch = 0;

	// For each key look
	for (BBP::std::index_t idx = 0; idx < numkeys; idx++)
	{
		// If key is mod, continue
		if (idx == SDL_SCANCODE_LSHIFT || idx == SDL_SCANCODE_RSHIFT)
			continue;

		if (idx == SDL_SCANCODE_LCTRL || idx == SDL_SCANCODE_RCTRL)
			continue;

		// Get onDown state
		bool justPressed = (keyData[idx] == true) && (oldMask[idx] == false);

		// Update old mask
		oldMask[idx] = keyData[idx];

		// If no input detected, continue
		if (justPressed == false)
			continue;
		
		else
			ch |= SDL_GetKeyFromScancode((SDL_Scancode)idx);
	}

	// If nothing was pressed in return false
	if (ch == 0)
		return false;

	// Get keyboard mod state
	SDL_Keymod mod = SDL_GetModState();

	// If mod is none, return true
	if (mod == KMOD_NONE)
		return true;

	// If 'ch' is alpha, and shift is pressed in, make uppercase
	if (ch >= 'a' && ch <= 'z' && (mod & KMOD_SHIFT))
		ch -= ('a' - 'A');

	// If this is a numerical character and shift is pressed in, use array
	if (ch >= '0' && ch <= '9' && (mod & KMOD_SHIFT))
		ch = numericalShifts[ch - '0'];

	// Otherwise shift manually
	for (BBP::std::index_t idx = 0; idx < 10; idx++)
		if (ch == manualNormal[idx])
			ch = manualShifted[idx];

	// If ch is non-zero it means something was found.
	return !!(ch);
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