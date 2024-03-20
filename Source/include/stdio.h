#ifndef BBP_STDLIB_STDIO_H
#define BBP_STDLIB_STDIO_H

#include "Strings.h"
#include "Resources.h"

namespace BBP
{
	namespace std
	{
		// Printing stuff
		int sprintf(std::c_string buffer, std::conststring format, ...);
		int printf(std::conststring format, ...);

		// Putting stuff
		int putf(std::Stack<std::string_element> &stream, std::conststring format, ...);

		// Get amount of characters buffered
		std::size_t inputBufferCount();

		// Get a single character from the input buffer
		std::string_element getChar();
		std::string_element getCharAsync();

		// stupid namespace is needed. Thanks C++
		namespace InputEncoding
		{

			// Key structure
			enum KeyCode : std::word
			{
				// Control characters
				NULL = 0,


				// Standard codes
				SPACE = 0x20,
				EXCLAMATION,
				DOUBLE_QUOTE,
				POUND,
				PERCENT,
				AMPERSAND,
				SINGLE_QUOTE,
				LEFT_PARENTHESES,
				RIGHT_PARENTHESES,
				ASTERISK,
				PLUS,
				COMMA,
				DASH,
				PERIOD,
				STROKE,
				ZERO,
				ONE,
				TWO,
				THREE,
				FOUR,
				FIVE,
				SIX,
				SEVEN,
				EIGHT,
				NINE,
				COLON,
				SEMICOLON,
				LESSTHAN,
				EQUAL,
				GREATERTHAN,
				QUESTIONMARK,
				AT,
				A,
				B,
				C,
				D,
				E,
				F,
				G,
				H,
				I,
				J,
				K,
				L,
				M,
				N,
				O,
				P,
				Q,
				R,
				S,
				T,
				U,
				V,
				W,
				X,
				Y,
				Z,
				LEFTSQUAREBRACKET,
				BACKSTROKE,
				RIGHTSQUAREBRACKET,
				CARET,
				UNDERSCORE,
				GRAVE,
				a,
				b,
				c,
				d,
				e,
				f,
				g,
				h,
				i,
				j,
				k,
				l,
				m,
				n,
				o,
				p,
				q,
				r,
				s,
				t,
				u,
				v,
				w,
				x,
				y,
				z,
				LEFTCURLYBRACKET,
				PIPE,
				RIGHTCURLYBRACKET,
				TILDE,
				DELETE,

				// Special characters
				UP = 0x100,
				DOWN,
				RIGHT,
				LEFT
			};

		}

		// Input-getting but with keycodes
		InputEncoding::KeyCode getKeyCode();
		InputEncoding::KeyCode getKeyCodeAsync();

	}
}


#endif
