#include "../include/BIOS.h"

#include <cstdio>

// Print a character to the screen
void Host::BIOS::printCharacter(BBP::std::string_element c)
{
	putchar(c);
	fflush(stdout);
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