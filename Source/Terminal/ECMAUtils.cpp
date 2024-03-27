#include "../include/TerminalController.h"

bool BBP::std::Terminal::TerminalApplication::isCCC0(CC character)
{
	// Is character within C0_Low and C0_High?
	return (character >= C0_Low && character <= C0_High);
}

bool BBP::std::Terminal::TerminalApplication::isCCC1Upper(CC character)
{
	// Check if unmodified falls between C1_Low and C1_High
	bool unmodifiedC1 = (character >= C1_Low && character <= C1_High);

	// Return if either
	return unmodifiedC1;
}

bool BBP::std::Terminal::TerminalApplication::isCCC1Lower(CC character)
{
	// Get modified CC
	CC modified = character + this->FE_modifier;

	// Return if either
	return isCCC1Upper(modified);
}

