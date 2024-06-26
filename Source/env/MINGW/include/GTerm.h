#ifndef OS_GTERM_H
#define OS_GTERM_H

#include "../../../include/TerminalController.h"
#include "../../../include/Graphics.h"

namespace OS
{
	namespace Terminal
	{

		class GTerm : public BBP::std::Terminal::TerminalDevice
		{


		public:

			// Window to render on
			BBP::std::window *window;

			// X position and Y position in pixels
			BBP::std::size_t windowX;
			BBP::std::size_t windowY;

			// draw func
			void (*drawFunc)();

			// Bell
			void Bell();

			// Print unkown character
			void Unkown();

			// Receive private commands
			void Private1();
			void Private2();

			// Set device control mode
			void setDCMode(BBP::std::byte);
			void setICMode(BBP::std::byte);

			// Receive string commands
			void str_Device(BBP::std::Stack<BBP::std::string_element> &string);
			void str_Private(BBP::std::Stack<BBP::std::string_element> &string);
			void str_Application(BBP::std::Stack<BBP::std::string_element> &string);
			void str_OS(BBP::std::Stack<BBP::std::string_element> &string);
			void str_Generic(BBP::std::Stack<BBP::std::string_element> &string);

			// Display a character
			void displayCharacter(BBP::std::wstring_element, BBP::std::Terminal::TerminalState::TerminalRect &, BBP::std::Terminal::TerminalState &);

			// Screen operations
			void clearScreen(BBP::std::Terminal::TerminalState &);
			void renderDisplay();

		};


	}
}

#endif