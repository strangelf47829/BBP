#ifndef BBP_STDLIB_STDINT_H
#define BBP_STDLIB_STDINT_H

namespace BBP
{

	typedef signed char        int8_t;
	typedef short              int16_t;
	typedef int                int32_t;
#ifndef USE_32_BITS
	typedef long long          int64_t;
#endif
	typedef unsigned char      uint8_t;
	typedef unsigned short     uint16_t;
	typedef unsigned int       uint32_t;
#ifndef USE_32_BITS
	typedef unsigned long long uint64_t;
#endif

	typedef signed char        int_least8_t;
	typedef short              int_least16_t;
	typedef int                int_least32_t;
#ifndef USE_32_BITS
	typedef long long          int_least64_t;
#endif
	typedef unsigned char      uint_least8_t;
	typedef unsigned short     uint_least16_t;
	typedef unsigned int       uint_least32_t;
#ifndef USE_32_BITS
	typedef unsigned long long uint_least64_t;
#endif

	typedef signed char        int_fast8_t;
	typedef int                int_fast16_t;
	typedef int                int_fast32_t;
#ifndef USE_32_BITS
	typedef long long          int_fast64_t;
#endif
	typedef unsigned char      uint_fast8_t;
	typedef unsigned int       uint_fast16_t;
	typedef unsigned int       uint_fast32_t;
#ifndef USE_32_BITS
	typedef unsigned long long uint_fast64_t;
#endif
}

namespace BBP
{
	namespace std
	{

		// Get the amount of decimals (so log10)
		uint32_t decimals(uint32_t value);

		// Get a specific decimal
		uint8_t getDecimal(uint32_t value, uint32_t position);

		// Reverse the order of a value (base 10)
		uint32_t reverse(uint32_t);

		// Convert 2 bytes into half word, taking endianness into account
		uint16_t stitch(uint8_t, uint8_t, bool);

		// Convert 2 halfwords into a word, taking endianness into account
		uint32_t stitch(uint16_t, uint16_t, bool);

		// Convert 4 bytes into a word, taking endianness into account
		uint32_t stitch(uint8_t, uint8_t, uint8_t, uint8_t, bool);

		// Convert a 16 bit value into two 8 byte values
		void split(uint16_t, uint8_t &MSB, uint8_t &LSB, bool);

		// Convert a 32 bit value into two 16 byte values
		void split(uint32_t, uint16_t &MSB, uint16_t &LSB, bool);

		// Convert a 32 bit value into four 8 byte values
		void split(uint32_t, uint8_t &MSSB, uint8_t &LSSB, uint8_t &MSLB, uint8_t &LSLB, bool);

		// What is the environment endianness policy?
		bool isLittleEndianEnvironment();

		// Set endianness globally
		void setLittleEndianEnvironment();
		void setBigEndianEnvironment();

		// Above conversion functions but using global endianness values
		uint16_t stitch(uint8_t, uint8_t);
		uint32_t stitch(uint16_t, uint16_t);
		uint32_t stitch(uint8_t, uint8_t, uint8_t, uint8_t);

		// Above split functions but using global endianness values
		void split(uint16_t, uint8_t &MSB, uint8_t &LSB);
		void split(uint32_t, uint16_t &MSB, uint16_t &LSB);
		void split(uint32_t, uint8_t &MSSB, uint8_t &LSSB, uint8_t &MSLB, uint8_t &LSLB);

	}
}

#endif



