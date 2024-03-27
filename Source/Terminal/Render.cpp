#include "../include/TerminalController.h"

void BBP::std::Terminal::TerminalApplication::render(CC character)
{
	// Input a character
	input(character);

	// Present the results
	presentTerminal();

	// If device exists, draw result
	if (device)
		device->renderDisplay();
}

void BBP::std::Terminal::TerminalApplication::render(string &str)
{
	// Input a character
	input(str);

	// Present the results
	presentTerminal();

	// If device exists, draw result
	if (device)
		device->renderDisplay();
}