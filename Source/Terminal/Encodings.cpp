#include "../include/TerminalController.h"

// Byte 0: [character]
// Byte 1: [4:fg] [4:bg]
// Byte 2: [Reserved]
// Byte 3: [Reserved]

BBP::std::wstring_element BBP::std::Terminal::TerminalApplication::encodeState(CC character)
{
	// Declare state
	wstring_element element = 0 | (character & 0x7F);

	// Create 2nd byte
	wstring_element secondByte = (state.simple_fg | (state.simple_bg << 4)) << 8;

	// Combine
	element = element | secondByte;

	// Return element
	return element;
}