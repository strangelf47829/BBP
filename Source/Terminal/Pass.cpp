#include "../include/TerminalController.h"

void BBP::std::Terminal::TerminalApplication::addCharacterToPosition(CC character)
{
	// Get currently active string
	std::string *activeString = state.terminalLines[state.activePresentationPosition.vertical];

	// Get sequence length
	size_t lineLength = seqlen(*activeString);

	// If active position is further than line length, extend
	if (state.activePresentationPosition.horizontal >= lineLength)
		extendLine();

	// Write
	std::string_element c = character;
	write(activeString, c, (std::index_t)state.activePresentationPosition.horizontal);
}

// Input for ...
void BBP::std::Terminal::TerminalApplication::PassCharacter(CC character)
{
	// if character is null, do nothing
	if (character == 0)
		return;

	switch (currentState)
	{
	case Normal:
		actNormal(character);
		return;
	case escaped:
		actEscaped(character);
		return;
	case Str:
		actString(character);
		return;
	case ControlSequence:
		actCSI(character);
		return;
	}
}