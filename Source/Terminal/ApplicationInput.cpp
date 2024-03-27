#include "../include/TerminalController.h"

void BBP::std::Terminal::TerminalApplication::set(size_t w, size_t h)
{
	state.viewportPosition = { h, w };


}

void BBP::std::Terminal::TerminalApplication::input(CC character)
{
	// Add character to input
	addCharacterToPosition(character);

	// If character is null do nothing
	if (character == 0)
		return;

	PassCharacter(character);
}

void BBP::std::Terminal::TerminalApplication::input(std::string &str)
{
	// Get string length
	std::size_t stringLength = std::strlen(str);

	// Pass each character in string (excluding null)
	for (std::index_t idx = 0; idx < stringLength; idx++)
		input(str[idx]);

	// Input null
	input(0);
}