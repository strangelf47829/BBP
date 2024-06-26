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

	}
}

#endif



