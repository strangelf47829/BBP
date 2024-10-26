#ifndef BBP_ELSA_NUMERICAL_H
#define BBP_ELSA_NUMERICAL_H

#include "../stddef.h"
#include "../Hashing.h"

namespace BBP
{
	namespace elsa
	{

		// Numerical structure
		struct Numerical_t
		{

			// assert float 32 bits
			static_assert(sizeof(float) == sizeof(std::word), "Float must be 32-bits long in a numerical");

			// The 32 bits
			std::word bits;

			// Interpret them as:
			float *floatPtr;
			std::word *wordPtr;
			int32_t *signedPtr;

			// What the current type is
			enum
			{
				UINT8,
				UINT16,
				UINT32,
				INT8,
				INT16,
				INT32,
				FLOAT
			} type;

			// Write the 32 bits into a word
			void getBytes(std::word &);

			// Interpret current value as signed value
			void interpretSigned();

			// Interpret current value as unsigned
			void interpretUnsigned();

			// Interpret as float
			void interpretFloat();

			// Interpret as byte
			void interpretByte();

			// Interpret as halfword
			void interpretHalfword();

			// Interpret as word.
			void interpretWord();

			// Is float?
			bool isFloat();

			// is Signed?
			bool isSigned();

			// is Unsigned?
			bool isUnsigned();

			// Create as float
			Numerical_t(float);

			// Create as unsigned
			Numerical_t(std::word);

			// Create as signed
			Numerical_t(int32_t);

			// Default constructor.
			Numerical_t();

			// Static operator stuff
			Numerical_t operator+(Numerical_t);
			Numerical_t operator-(Numerical_t);

			// Static operator stuff
			Numerical_t operator*(Numerical_t);
			Numerical_t operator/(Numerical_t);
			Numerical_t operator%(Numerical_t);

			// Equality comparators
			bool operator==(Numerical_t);
			bool operator!=(Numerical_t);

			// Greater than, less than, comparators
			bool operator<(Numerical_t);
			bool operator>(Numerical_t);

			// Compound comparators
			bool operator<=(Numerical_t);
			bool operator>=(Numerical_t);	

			// Compound operators
			void operator+=(Numerical_t);
			void operator-=(Numerical_t);
			void operator/=(Numerical_t);
			void operator*=(Numerical_t);
			void operator%=(Numerical_t);

			// Compound unary operators
			Numerical_t operator++();
			Numerical_t operator--();

			// Negate 
			Numerical_t operator-();

			// Not
			bool operator!();

			// Bitwise operations
			std::word operator|(Numerical_t);
			std::word operator&(Numerical_t);
			std::word operator^(Numerical_t);
			std::word operator~();

			// Hashing function
			explicit operator std::hash_t () const;

		};

	}
}

#endif
