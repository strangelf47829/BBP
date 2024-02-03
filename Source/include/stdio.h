#ifndef BBP_STDLIB_STDIO_H
#define BBP_STDLIB_STDIO_H

#include "Strings.h"
#include "Resources.h"

namespace BBP
{
	namespace std
	{
		extern static_string<509> str_STDOUT;
		extern static_string<509> str_STDIN;
		extern static_string<509> str_STDERR;

		extern std::Stack<std::string_element> STDOUT;
		extern std::Stack<std::string_element> STDIN;
		extern std::Stack<std::string_element> STDERR;

		void print_to_terminal(std::Stack<std::string_element> *stack);

		int sprintf(std::c_string buffer, std::conststring format, ...);
		int printf(std::conststring format, ...);

		int putf(std::Stack<std::string_element> &stream, std::conststring format, ...);


		/*
		*  +------------------------------+
		*  |  Functions related to input  |
		*  +------------------------------+
		*/

		void captureInput();
		void stopCapturingInput();
		char getC();


		// This flag tells the shell if the user is expecting a line
		extern volatile bool _waitforline;

		// This flag tells the input capturing process if it should echo things back to the terminal
		extern volatile bool _echoStuffToTerminal;

		void getLine();

	}
}


#endif
