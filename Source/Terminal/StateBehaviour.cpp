#include "../include/TerminalController.h"



void BBP::std::Terminal::TerminalApplication::actCSI(CC character)
{
	// Check if character is of P or I type
	bool isP = (character >= this->CSI_P_Low && character <= this->CSI_P_High);
	bool isI = (character >= this->CSI_I_Low && character <= this->CSI_I_High);

	// If is of P type, check if it is seperator
	if (isP)
	{
		// If not accepting P type, return
		if (startingIntermediateBytes)
			return;

		// Check if is separator
		if (character == 0x3B)
		{
			// Push argument,
			pushArgument(currentArgument);

			// Reset argument
			currentArgument = 0;
			currentArgumentRadix = 1;

			// done.
			return;
		}

		// If is within 0x30 and 0x3A (inclusive), add to argument
		if (character >= 0x30 && character <= 0x3A)
		{
			// Multiply by radix
			word multipliedArgument = (character - 0x30) * currentArgumentRadix;

			// Multiply radix by 10
			currentArgumentRadix *= 10;

			// Add that to argument
			currentArgument += multipliedArgument;

			// Done
			return;
		}

		// combinations 0x3C through 0x3F are reserved
		return;
	}

	// If is I type, add that to I bytes and disable P args
	if (isI)
	{
		// Enable flag
		startingIntermediateBytes = true;

		// Push byte
		pushIntermediateByte(character);

		// Return
	}

	// Otherwise, check if final
	bool isFinal = (character >= this->CSI_F_Low && character <= this->CSI_F_High);
	bool isFinalPrivate = (character >= this->CSI_F_Priv_Low && character <= this->CSI_F_High);

	// If neither, return since nothing is to be done.
	if ((isFinal || isFinalPrivate) == false)
		return;

	// Do CSI action based on if intermediate bytes are present or not
	if (startingIntermediateBytes)
		return; //TODO: implement
	else
		CSIActionNoInters(character);

}

void BBP::std::Terminal::TerminalApplication::actNormal(CC character)
{
	// Check if character is a C0 sequence
	bool isC0 = isCCC0(character);

	// If so, do corresponding C0 action
	if (isC0)
	{
		// Get corresponding action
		C0 c0Action = getC0Action(character);

		// Do action
		C0Action(c0Action);

		// Then return.
		return;
	}

	// Check if character is C1 sequence
	bool isC1 = isCCC1Upper(character);

	// If so, do corresponding C1 action
	if (isC1)
	{
		// Get corresponding action
		C1 c1Action = getC1Action(character);

		// Do action
		C1Action(c1Action);

		// Then return.
		return;
	}

	// Display character, if position is not before viewport
	if (device && state.activePresentationPosition.vertical >= state.viewportPosition.vertical)
		device->displayCharacter(character, state.activePresentationPosition, state);

	// Then move active position
	MoveForward(1);

}

void BBP::std::Terminal::TerminalApplication::actEscaped(CC character)
{
	// ESC must be followed by a lower C1 code.
	// If not, then do unkown, then set back to normal mode.
	bool isC1 = isCCC1Lower(character);

	if (isC1 == false)
	{
		C0Action(C0::SUB);
		currentState = Normal;
		return;
	}

	// Otherwise, get action
	C1 action = getC1Action(character);

	// Do that action
	C1Action(action);

	// If current state is escaped, set back to normal
	if (currentState == escaped)
		currentState = Normal;
}

void BBP::std::Terminal::TerminalApplication::actString(CC character)
{
	// Check if character is string terminate
	if (character == C1::ST)
	{
		ST_C1();
		return;
	}

	// Otherwise add character to stack
	stringStack << character;
}