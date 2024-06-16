#ifndef HOST_SCREEN_H
#define HOST_SCREEN_H

#include "../include/BIOS.h"
#include "../../../include/FirmwareInterface.h"
#include "../../../include/Graphics.h"
#include "../../../include/TerminalController.h"
#include "GTerm.h"
#include "SDL2/SDL.h"

// Get references to commonly used functions

namespace Host
{
	
	// Terminal device
	extern OS::Terminal::GTerm gterm;

	// Terminal itself
	extern BBP::std::Terminal::TerminalApplication terminal;

	// Window dimensions
	constexpr BBP::std::pos_t w = 1200;
	constexpr BBP::std::pos_t h = 800;

	// Window
	extern BBP::std::window hostDisplay;

	// Start the window
	void startDisplay(Host::BIOS &);

	// Close the window
	void closeDisplay();

	// Draw a display
	void drawWindow(BBP::std::window *, SDL_Renderer *);

	// Draw the display
	void drawDisplay();


}

#endif