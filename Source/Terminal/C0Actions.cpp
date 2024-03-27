#include "../include/TerminalController.h"

// This file implements actual C0 actions

void BBP::std::Terminal::TerminalApplication::BEL_C0()
{
	// If active device, call bell
	if (device)
		device->Bell();
}

void BBP::std::Terminal::TerminalApplication::BS_C0()
{
	// Move back one position
	MoveBack(1);
}

void BBP::std::Terminal::TerminalApplication::HT_C0()
{
	// If tabulation is 0, do nothing
	if (state.tabularSettings.horizontal == 0)
		return;

	// Otherwise, get presentation and modulo that with state
	std::size_t toIncrease = state.activePresentationPosition.horizontal % state.tabularSettings.horizontal;

	// If this is 0, set to horizontal tab setting
	if (toIncrease == 0)
		toIncrease = state.tabularSettings.horizontal;

	// Move forward that amount
	MoveForward(toIncrease);
}

void BBP::std::Terminal::TerminalApplication::LF_C0()
{
	// Move down one position
	MoveDown(1);

	// If implicit CR, do this
	if (state.implicitCR)
		CR_C0();
}

void BBP::std::Terminal::TerminalApplication::VT_C0()
{
	// Do nothing for now
}

void BBP::std::Terminal::TerminalApplication::FF_C0()
{
	// If viewport size is 0, do nothing
	if (state.viewportSize.vertical == 0)
		return;

	// Get amount of lines already set within this viewport
	std::size_t toIncrease = state.activePresentationPosition.vertical % state.viewportSize.vertical;

	// Get the amount needed to finish this viewport
	toIncrease = state.viewportSize.vertical - toIncrease;

	// Scroll down that amount
	MoveDown(toIncrease);
}

void BBP::std::Terminal::TerminalApplication::CR_C0()
{
	// Check horizontal limit
	std::size_t limit = 0;

	// If SIMD is 1, set limit to limits.
	if (state.SIMD == 1)
		limit = state.Limits.horizontal;

	// If horizontal position is greater than that of 'limit', do nothing
	if (state.activePresentationPosition.horizontal <= limit)
		return;

	// Get difference between the two
	std::size_t horizontalDiff = state.activePresentationPosition.horizontal - limit;

	// Do action based on DCSM
	switch (state.DCSM)
	{
	case Terminal::TerminalState::DCSMSetting::Presentation:
		MoveBack(horizontalDiff);
		break;
	case Terminal::TerminalState::DCSMSetting::Data:
		MoveBack(horizontalDiff);
		break;
	}
}

void BBP::std::Terminal::TerminalApplication::DC1_C0()
{
	// Set mode
	if (device)
		device->setDCMode(1);
}

void BBP::std::Terminal::TerminalApplication::DC2_C0()
{
	// Set mode
	if (device)
		device->setDCMode(2);
}

void BBP::std::Terminal::TerminalApplication::DC3_C0()
{
	// Set mode
	if (device)
		device->setDCMode(3);
}

void BBP::std::Terminal::TerminalApplication::DC4_C0()
{
	// Set mode
	if (device)
		device->setDCMode(4);
}

void BBP::std::Terminal::TerminalApplication::SUB_C0()
{
	// Tell device to print unkown character
	if (device)
		device->Unkown();
}

void BBP::std::Terminal::TerminalApplication::ESC_C0()
{
	// Set mode to 'escape'
	currentState = escaped;
}

void BBP::std::Terminal::TerminalApplication::IS1_C0()
{
	// Set mode
	if (device)
		device->setICMode(1);
}

void BBP::std::Terminal::TerminalApplication::IS2_C0()
{
	// Set mode
	if (device)
		device->setICMode(2);
}

void BBP::std::Terminal::TerminalApplication::IS3_C0()
{
	// Set mode
	if (device)
		device->setICMode(3);
}

void BBP::std::Terminal::TerminalApplication::IS4_C0()
{
	// Set mode
	if (device)
		device->setICMode(4);
}


// Do action based on action
void BBP::std::Terminal::TerminalApplication::C0Action(C0 action)
{
	// If not in normal mode, do nothing
	if (currentState != Normal)
		return;
	
	// Do action based on ...
	switch (action)
	{
	case BEL:
		BEL_C0();
		break;
	case BS:
		BS_C0();
		break;
	case HT:
		HT_C0();
		break;
	case LF:
		LF_C0();
		break;
	case VT:
		VT_C0();
		break;
	case FF:
		FF_C0();
		break;
	case CR:
		CR_C0();
		break;
	case DC1:
		DC1_C0();
		break;
	case DC2:
		DC2_C0();
		break;
	case DC3:
		DC3_C0();
		break;
	case DC4:
		DC4_C0();
		break;
	case SUB:
		SUB_C0();
		break;
	case ESC:
		ESC_C0();
		break;
	case IS1:
		IS1_C0();
		break;
	case IS2:
		IS2_C0();
		break;
	case IS3:
		IS3_C0();
		break;
	case IS4:
		IS4_C0();
		break;
	}
}

BBP::std::Terminal::TerminalApplication::C0 BBP::std::Terminal::TerminalApplication::getC0Action(CC character)
{
	return (C0)character;
}

// ...

