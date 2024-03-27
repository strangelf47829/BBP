#include "../include/TerminalController.h"

void BBP::std::Terminal::TerminalApplication::NEL_C1()	// Line feed, carriage return
{
	LF_C0();
	CR_C0();
}

void BBP::std::Terminal::TerminalApplication::SSA_C1()	// Set start of selected area
{
	// no-op
}

void BBP::std::Terminal::TerminalApplication::ESA_C1()	// Set end of selected area
{
	// no-op
}

void BBP::std::Terminal::TerminalApplication::PLD_C1()	// Set subscript or restore superscript
{
	// Set normal if superscript, otherwise set subscript
	if (state.textmode == state.Superscript)
		state.textmode = state.normal;
	else
		state.textmode = state.Subscript;
}

void BBP::std::Terminal::TerminalApplication::PLU_C1()	// Set superscript or restore subscript
{
	// Set normal if subscript, otherwise set superscript
	if (state.textmode == state.Subscript)
		state.textmode = state.normal;
	else
		state.textmode = state.Superscript;
}

void BBP::std::Terminal::TerminalApplication::DCS_C1()	// Begin a device command string context
{
	// Set state string mode
	currentState = Str;
	stringState = Device;
}

void BBP::std::Terminal::TerminalApplication::PU1_C1()	// Private use
{

}

void BBP::std::Terminal::TerminalApplication::PU2_C1()	// Private use
{

}

void BBP::std::Terminal::TerminalApplication::CCH_C1()	// DEL
{
	// Delete current character and force redraw
	// ...
}

void BBP::std::Terminal::TerminalApplication::SPA_C1()	// Start of guarded area
{
	// No-op
}

void BBP::std::Terminal::TerminalApplication::EPA_C1()	// End of guarded area
{
	// No-op
}

void BBP::std::Terminal::TerminalApplication::SOS_C1()	// Begin a string context.
{
	currentState = Str;
	stringState = Generic;
}

void BBP::std::Terminal::TerminalApplication::CSI_C1()	// Control sequence indicator.
{
	currentState = ControlSequence;
}

void BBP::std::Terminal::TerminalApplication::ST_C1()	// Terminate a string context
{
	// If got a device,
	if (device)
	{
		// Do action based on ...
		switch (stringState)
		{
		case Device:
			device->str_Device(stringStack);
			break;
		case Private:
			device->str_Private(stringStack);
			break;
		case Application:
			device->str_Application(stringStack);
			break;
		case OS:
			device->str_OS(stringStack);
			break;
		case Generic:
			device->str_Generic(stringStack);
			break;
		}
	}

	currentState = Normal;
	stringState = none;
}

void BBP::std::Terminal::TerminalApplication::OSC_C1()	// Operating system command, and begins a string context
{
	currentState = Str;
	stringState = OS;
}

void BBP::std::Terminal::TerminalApplication::PM_C1()	// Private message
{
	currentState = Str;
	stringState = Private;
}

void BBP::std::Terminal::TerminalApplication::APC_C1()	// Application program command, and begins a string context
{
	currentState = Str;
	stringState = Application;
}


// This file implements actual C1 actions
void BBP::std::Terminal::TerminalApplication::C1Action(C1 action)
{
	switch (action)
	{
	case NEL:
		NEL_C1();
		break;
	case SSA:
		SSA_C1();
		break;
	case ESA:
		ESA_C1();
		break;
	case PLD:
		PLD_C1();
		break;
	case PLU:
		PLU_C1();
		break;
	case DCS:
		DCS_C1();
		break;
	case PU1:
		PU1_C1();
		break;
	case PU2:
		PU2_C1();
		break;
	case CCH:
		CCH_C1();
		break;
	case SPA:
		SPA_C1();
		break;
	case EPA:
		EPA_C1();
		break;
	case SOS:
		SOS_C1();
		break;
	case CSI:
		CSI_C1();
		break;
	case ST:
		CSI_C1();
		break;
	case OSC:
		OSC_C1();
		break;
	case PM:
		PM_C1();
		break;
	case APC:
		APC_C1();
		break;
	}
}

BBP::std::Terminal::TerminalApplication::C1 BBP::std::Terminal::TerminalApplication::getC1Action(CC character)
{
	// Check if within C1 low or C1 high
	bool isUpper = (character >= C1_Low && character <= C1_High);

	// If is Upper, return character minus modifier
	if (isUpper)
		return (C1)(character - this->FE_modifier);

	// Otherwise, return this
	return (C1)(character);
}

// ...

