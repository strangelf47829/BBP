#include "../include/GTerm.h"

// Array for FG colours
BBP::std::colour colours[16] = {
	BBP::std::colour(0, 0, 0),
	BBP::std::colour(170, 0, 0),
	BBP::std::colour(0,170,0),
	BBP::std::colour(170, 85, 0),
	BBP::std::colour(0, 0, 170),
	BBP::std::colour(170, 0, 170),
	BBP::std::colour(0, 160, 170),
	BBP::std::colour(170, 170, 170),
	BBP::std::colour(85, 85, 85),
	BBP::std::colour(255, 85, 85),
	BBP::std::colour(85,255,85),
	BBP::std::colour(255, 255, 85),
	BBP::std::colour(85, 85, 255),
	BBP::std::colour(255, 85, 255),
	BBP::std::colour(85, 255, 255),
	BBP::std::colour(255, 255, 255)};

// Bell
void OS::Terminal::GTerm::Bell()
{

}

void OS::Terminal::GTerm::Unkown()
{

}

void OS::Terminal::GTerm::Private1()
{

}

void OS::Terminal::GTerm::Private2()
{

}

void OS::Terminal::GTerm::setDCMode(BBP::std::byte)
{

}

void OS::Terminal::GTerm::setICMode(BBP::std::byte)
{

}

void OS::Terminal::GTerm::str_Device(BBP::std::Stack<BBP::std::string_element> &string)
{

}

void OS::Terminal::GTerm::str_Private(BBP::std::Stack<BBP::std::string_element> &string)
{

}

void OS::Terminal::GTerm::str_Application(BBP::std::Stack<BBP::std::string_element> &string)
{

}

void OS::Terminal::GTerm::str_OS(BBP::std::Stack<BBP::std::string_element> &string)
{

}

void OS::Terminal::GTerm::str_Generic(BBP::std::Stack<BBP::std::string_element> &string)
{

}

void OS::Terminal::GTerm::displayCharacter(BBP::std::wstring_element cc, BBP::std::Terminal::TerminalState::TerminalRect &pos, BBP::std::Terminal::TerminalState &state)
{
	// X and Y modifiers
	BBP::std::word xMod = window->fontSpacing * window->fontSize;
	BBP::std::word yMod = window->fontSpacing * window->fontSize * 2;

	BBP::std::word xPos = pos.horizontal + state.viewportPosition.horizontal * state.viewportSize.horizontal;
	BBP::std::word yPos = pos.vertical - state.viewportPosition.vertical;

	// If character is not in view, do nothing
	if (pos.vertical < state.viewportPosition.vertical || pos.vertical >= state.viewportPosition.vertical + state.viewportSize.vertical)
		return;

	BBP::std::offset_t offset = 10;

	// Get colour
	BBP::std::nibble fg = (cc >> 8) & 0xF;
	BBP::std::nibble bg = (cc >> 12) & 0xF;

	// Print character
	BBP::std::R2D::PrintCharacterAt(*window, xPos * xMod + windowX + offset, yPos * yMod + windowY + offset, cc, window->fontSize, colours[fg], colours[bg]);
}

void OS::Terminal::GTerm::clearScreen(BBP::std::Terminal::TerminalState &state)
{
	// Draw empty background
	BBP::std::R2D::background(*window, 0xFF000000);

	// Calculate widths
	BBP::std::word xSize = window->fontSpacing * window->fontSize;
	BBP::std::word ySize = window->fontSpacing * window->fontSize * 2;

	xSize *= state.viewportSize.horizontal + 1;
	ySize *= state.viewportSize.vertical + 1;

	// Frame
	BBP::std::R2D::stroke(*window, 0xFFFFFFFF);
	window->strokeWeight = 2;

	BBP::std::R2D::Frame(*window, windowX, windowY, xSize, ySize);
}

void OS::Terminal::GTerm::renderDisplay()
{
	if (drawFunc)
		drawFunc();
}