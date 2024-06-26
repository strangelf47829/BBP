#include "../include/TerminalController.h"

void BBP::std::Terminal::TerminalApplication::presentTerminal()
{
	// If no device, do nothing
	if (device == nullptr)
		return;

	// Clear device
	device->clearScreen(state);

	// Get display position
	Terminal::TerminalState::TerminalRect dispPos = state.viewportPosition;

	// Get string index
	for (std::index_t strIndex = 0; strIndex < state.viewportSize.vertical; strIndex++)
	{
		// Get active string
		std::w_string *str = state.terminalLines[strIndex + state.viewportPosition.vertical];

		// Present lines
		for (std::index_t ccIndex = 0; ccIndex < state.viewportSize.horizontal; ccIndex++)
		{
			// get CC
			std::wstring_element cc = (*str)[ccIndex + state.viewportPosition.horizontal];

			// If '0' break
			if ((cc & 0x7F) == 0)
				break;

			// Update dispPos
			dispPos = { strIndex + state.viewportPosition.vertical, ccIndex + state.viewportPosition.horizontal };

			// Display character (if it exists)
			device->displayCharacter(cc, dispPos, state);
		}
	}
}

