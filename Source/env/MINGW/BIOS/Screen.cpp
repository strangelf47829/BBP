#include "../include/BIOS.h"
#include "../include/ScreenData.h"

#include <cstdio>

int atX = 1;
int atY = 1;

// Print a character to the screen
void Host::BIOS::printCharacter(BBP::std::string_element c)
{
	// If not newline, print
	if (c != '\n')
		BBP::std::R2D::PrintCharacterAt(Host::hostDisplay, atX * 8, atY * 16, c);

	// Move up. Wrap around if necessary
	if (++atX > w / 8 || c == '\n')
	{
		atX = 1;
		if (++atY > h / 16)
			atY = 1;
	}
}

// Change FG colour
void Host::BIOS::setOutputFG(BBP::std::byte code)
{
	// Print
	printf("\e[0;%um", 30 + code);
}

// Change BG colour
void Host::BIOS::setOutputBG(BBP::std::byte code)
{
	// Print
	printf("\e[0;%um", 40 + code);
}

// Clear screen
void Host::BIOS::clearScreen()
{
	printf("\033[2J\033[1;1H");
}