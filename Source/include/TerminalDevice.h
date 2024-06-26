#ifndef BBP_TERMINALDEVICE_H
#define BBP_TERMINALDEVICE_H

// A terminal device is a device that is mostly compliant with ECMA35 and ECMA48 rules.
// This header provides an abstraction to implement own terminal devices (mostly by deciding how data gets input, and how to render the proper output)
// This header also implements the needed blah blah blah

// Also includes Terminal state for some useful structs
#include "TerminalState.h"

namespace BBP
{
	namespace std
	{
		namespace Terminal
		{
			// A virtual class that provides an ECMA35_48 controller an interface to display and render information
			class TerminalDevice
			{

			public:

				// Bell
				virtual void Bell() = 0;

				// Print unkown character
				virtual void Unkown() = 0;

				// Receive private commands
				virtual void Private1() = 0;
				virtual void Private2() = 0;

				// Set device control mode
				virtual void setDCMode(byte) = 0;
				virtual void setICMode(byte) = 0;

				// Receive string commands
				virtual void str_Device(Stack<string_element> &string) = 0;
				virtual void str_Private(Stack<string_element> &string) = 0;
				virtual void str_Application(Stack<string_element> &string) = 0;
				virtual void str_OS(Stack<string_element> &string) = 0;
				virtual void str_Generic(Stack<string_element> &string) = 0;

				// Display a character
				virtual void displayCharacter(wstring_element, TerminalState::TerminalRect &, BBP::std::Terminal::TerminalState &) = 0;

				// Screen operations
				virtual void clearScreen(TerminalState &) = 0;
				virtual void renderDisplay() = 0;

			};

		}
	}
}

#endif