#ifndef BBP_STD_ARCHS_H
#define BBP_STD_ARCHS_H

// This file defines differences between architectures (not environments) and sets the 

// Check if architecture is equal to x86
#ifdef ARCH_x86_64

// Used for ::std::size_t
#define LONG_INT unsigned long int

// Assembly instructions

#else // ARCH_x86_64
#ifdef ARCH_ARM

#define ASM_ADD(amount) "add $" #amount ", %0"
#define ASM_SUB(amount) "sub $" #amount ", %0"

// Used for ::std::size_t
#define LONG_INT unsigned int

#else // ARCH_ARM
#error "No architecture specified!"
#define ERR_NOARCH
#endif // ARCH_ARM
#endif // ARCH_x86_64

#ifndef ERR_NOARCH
namespace BBP
{
	namespace std
	{

		// Builtin size_t
		using __cxx_size_t = LONG_INT;

	}
}
#endif // ERR_NOARCH
#endif