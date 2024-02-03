#ifndef BBP_LIB_REGISTERS
#define BBP_LIB_REGISTERS

#include "stddef.h"
#include "Memory.h"

namespace BBP
{

	namespace userspace
	{

		enum registerSize : std::byte
		{
			eightBit = 1,
			sixteenBit = 2,
			thirtyTwoBit = 4,
			addressSize = 8
		};

		struct Thread;
		struct register_t
		{
			// The size of this register
			registerSize size;
			
			// Data where this register lives
			userspace::Thread *owner;
			std::index_t index;		// In words
			std::offset_t offset;	// In bytes. Total 'index' in bytes is: index * 4 + offset

			register_t() = delete;
			register_t(register_t &) = delete;
			register_t(register_t &&) = delete;

			register_t(Thread &thread, std::index_t index_, std::offset_t offset_, registerSize size_);

		};

		void setRegister(register_t &reg, std::word w);
		std::word readRegister(register_t &reg);

	}

}

#endif