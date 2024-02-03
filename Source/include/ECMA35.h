#ifndef BBP_STDLIB_ECMA35_H
#define BBP_STDLIB_ECMA35_H

#include "Resources.h"
#include "Strings.h"
#include "Graphics.h"

namespace BBP
{
	namespace std
	{
		class Terminal
		{
		public:

			// Create a terminal of size ...
			Terminal();
			Terminal(window *);

			// Put a single character into the terminal
			Terminal &put(string_element);

			// Put a string into the buffer.
			Terminal &puts(string &);
			Terminal &puts(conststring);
			Terminal &puts(c_string);

			// Private:

			// Stores the streams used by this terminal
			std::Stack<std::string_element> out;
			std::Stack<std::string_element> in;
			std::Stack<std::string_element> err;

			// If used in GUI mode
			bool usesGUI;
			window *GUI;

			// C0 control codes
			Terminal &Bell();
			Terminal &Backspace();
			Terminal &Tab();
			Terminal &LineFeed();
			Terminal &FormFeed();
			Terminal &CarriageReturn();
			Terminal &Escape();

			// Fe escape sequences
			Terminal &SS2(); // Single shift two
			Terminal &SS3(); // Single shift three

		};

	}
}

#endif