#include "../include/TerminalController.h"

void BBP::std::Terminal::TerminalApplication::presentTerminal()
{
	// If no device, do nothing
	if (device == nullptr)
		return;

	// Clear device
	device->clearScreen(state);

	// Go over each character from the beginning.... yeah...
	Terminal::TerminalState::TerminalRect prevPos = state.activePresentationPosition;
	state.activePresentationPosition = { 0, 0 };

	// Turn off implicit scroll
	state.implicitScroll = false;

	// Just display characters as-is for now, just for testing.
	for (std::index_t strIndex = 0; strIndex < state.lineCount; strIndex++)
	{
		std::w_string *str = state.terminalLines[strIndex];

		while (str)
		{
			// Go over each element in str
			for (std::index_t ccIndex = 0; ccIndex < str->dataSize; ccIndex++)
				PassCharacter(str->data[ccIndex]);

			str = str->nextPage;
		}
	}

	// Turn on implicit scroll
	state.implicitScroll = true;

	state.activePresentationPosition = prevPos;

}

