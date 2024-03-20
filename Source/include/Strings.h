#ifndef BBP_STDLIB_STRINGS_H
#define BBP_STDLIB_STRINGS_H

#include "Memory.h"
#include "Stack.h"
#include "Resources.h"


namespace BBP
{
	namespace std
	{
		/* The type a string is made of (which is just 'char') */
		typedef char string_element;
		typedef unsigned char ustring_element;

		/* Dynamically allocated string */
		typedef PAGE<string_element> string;

		/* Statically allocated string typedef */
		template <int N>
		using static_string = STATIC_PAGE<string_element, N>;

		/* Usual c-string definition */
		typedef string_element *c_string;
		typedef ustring_element *uc_string;

		/* Constant string definition */
		typedef const string_element *conststring;
		typedef const ustring_element *uconststring;

		/* Strings known at compile time for constant expressions */
		typedef const char *const constexprstring;

		/* The absolute longest size of a string */
		const size_t max_string_length = 0xFFFF;

		/* The hashing type of a string */
		typedef uint32_t hash_t;

		/* Copy string literal into 'dst' */
		void strcpy(string *dst, conststring src);
		void operator<=(string &dst, conststring src);
		
		/* Copy conventional string into 'dst' */
		void strcpy(string *dst, c_string src);
		void operator<=(string &dst, c_string src);

		/* Copy conventional string into 'dst' */
		void strcpy(c_string dst, conststring src);

		/* Compare string to string literal */
		bool strcmp(string *sta, conststring stb);

		/* Compare string to conventional string */
		bool strcmp(string *sta, c_string stb);

		/* Calculate size of string literal */
		size_t strlen(conststring str);

		/* Calculate size of conventional string */
		size_t strlen(c_string str);

		/* Calculate size of string */
		size_t strlen(string &str);

		/* Calculate size of string at specified offset */
		size_t strlen(string str, offset_t offset);

		/* Calculate the hash of a given string */
		hash_t strhsh(string &str);
		hash_t strhsh(c_string str);
		hash_t strhsh(conststring str);

		inline std::string String(constexprstring _string)
		{
			return std::string(std::strlen(_string), (c_string)_string);
		}

		constexpr inline std::size_t static_length(constexprstring string)
		{

			// Calculate length
			std::size_t length = 0;
			for (length = 0; string[length]; length++);
			return length;
		}

		constexpr inline std::hash_t static_hash(constexprstring stringToHash)
		{
			// Hashing constants
			const int p = 31;
			const int m = 1e9 + 9;

			// Working variables
			std::hash_t hash = 0;
			uint32_t Power = 1;

			// Get the size of the string
			std::size_t length = static_length(stringToHash);

			// Actual hashing algorithm. Taken from: 
			for (std::index_t index = 0; index < length; index++)
			{
				char c = stringToHash[index];
				hash = (hash + (c - 'a' + 1) * Power) % m;
				Power = (p * Power) % m;
			}

			return hash;
		}

		constexpr inline std::hash_t _const_sym_hash(constexprstring stringToHash)
		{
			return static_hash(stringToHash);
		}

		/* Running hash */
		void strhsh(hash_t &, word &, string_element &);

		/* Calculate size of string literal */
		size_t __UNSAFE__(strlen)(conststring str);

		/* Calculate size of conventional string */
		size_t __UNSAFE__(strlen)(c_string str);

		/* Calculate size of string */
		size_t __UNSAFE__(strlen)(string str);

		/* Calculate size of string at specified offset */
		size_t __UNSAFE__(strlen)(string str, offset_t offset);

		/* Write one whole string into a char buffer */
		std::Stack<std::string_element> &operator<<=(std::Stack<std::string_element> &stack, std::conststring str);
		std::Stack<std::string_element> &operator<<=(std::Stack<std::string_element> &stack, std::c_string str);
		std::Stack<std::string_element> &operator<<=(std::Stack<std::string_element> &stack, std::string &str);
		std::Stack<std::string_element> &operator<<=(std::Stack<std::string_element> &stack, char c);
		std::Stack<std::string_element> &operator<=(std::Stack<std::string_element> &stack, int value);

		/* Write one whole string into a char buffer (no null terminator) */
		std::Stack<std::string_element> &operator<<(std::Stack<std::string_element> &stack, std::conststring str);
		std::Stack<std::string_element> &operator<<(std::Stack<std::string_element> &stack, std::c_string str);
		std::Stack<std::string_element> &operator<<(std::Stack<std::string_element> &stack, std::string &str);
		std::Stack<std::string_element> &operator<(std::Stack<std::string_element> &stack, int value);

		/* Read one whole string from a buffer */
		std::Stack<std::string_element> &operator>>=(std::Stack<std::string_element> &stack, std::c_string *str);
		std::Stack<std::string_element> &operator>>=(std::Stack<std::string_element> &stack, std::string *str);

		/* Read one whole string from a buffer */
		std::Stack<std::string_element> &operator>>(std::Stack<std::string_element> &lh, std::Stack<std::string_element> &rh);
		std::Stack<std::string_element> &operator<<(std::Stack<std::string_element> &lh, std::Stack<std::string_element> &rh);
		std::Stack<std::string_element> &operator>>=(std::Stack<std::string_element> &lh, std::Stack<std::string_element> &rh);
		std::Stack<std::string_element> &operator<<=(std::Stack<std::string_element> &lh, std::Stack<std::string_element> &rh);

		/* Read one whole string from a buffer (unsafe) */
		std::Stack<std::string_element> &operator>(std::Stack<std::string_element> &stack, std::c_string *str);
		std::Stack<std::string_element> &operator>(std::Stack<std::string_element> &stack, std::string *str);

		const std::conststring space = " ";
		const std::conststring endl = "\n";

	}
}


#endif
