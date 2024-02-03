#ifndef BBP_LIB_THREADING
#define BBP_LIB_THREADING

// This header provides RAE with data structures to do threading.

#include "ELF.h"
#include "BinaryExecutable.h"
#include "Memory.h"
#include "Registers.h"
#include "Instructions.h"

namespace BBP
{
	namespace userspace
	{

		typedef std::byte pid_t;

		class HyperVisor;
		struct Thread
		{
			// This is the executable
			BinaryExecutable executable;

			// TLS Data
			std::PAGE<std::string_element> TLS;
			std::ResourceManager *allocator;

			// Flag to if the thread is active or not
			bool active;

			// Registers go here
			static constexpr std::size_t registerCount = 12;
			std::STATIC_PAGE<std::byte, registerCount * 4> registerData;

			// Current instruction
			userspace::Instruction instruction;

			register_t eax;
			register_t  ax;
			register_t  ah;
			register_t  al;

			register_t ecx;
			register_t  cx;
			register_t  ch;
			register_t  cl;

			register_t edx;
			register_t  dx;
			register_t  dh;
			register_t  dl;

			register_t ebx;
			register_t  bx;
			register_t  bh;
			register_t  bl;

			register_t esi;
			register_t six;
			register_t sih;
			register_t sil;

			register_t edi;
			register_t dix;
			register_t dih;
			register_t dil;

			register_t tix;
			register_t  st;
			register_t  ob;
			register_t  tb;
			register_t  ct;
			register_t  pp;
			register_t  tp;

			register_t ssx;
			register_t  hv;
			register_t  ho;
			register_t  ht;
			register_t  ss;
			register_t  cs;
			register_t  sf;

			register_t sui;
			register_t asi;
			register_t gsi;

			register_t eip;
			register_t  ip;
			register_t  ih;
			register_t  il;

			register_t sip;

			// Argument and general Stack
			constexpr static std::word argumentStackSize = 255;
			constexpr static std::word generalStackSize = 255;

			// Argument / General page
			std::STATIC_PAGE<std::word, argumentStackSize> argumentPage;
			std::STATIC_PAGE<std::word, generalStackSize> generalPage;

			// Argument / General Stack
			std::Stack<std::word> argumentStack;
			std::Stack<std::word> generalStack;
			

			// Signal handling stuff goes here

			// Flag handling goes here

			// Fault handling goes here

			Thread()
				: eax(*this, 0, 0, thirtyTwoBit),
				   ax(*this, 0, 0, sixteenBit),
				   ah(*this, 0, 1, eightBit),
				   al(*this, 0, 0, eightBit),

				ecx(*this, 1, 0, thirtyTwoBit),
				 cx(*this, 1, 0, sixteenBit),
				 ch(*this, 1, 1, eightBit),
				 cl(*this, 1, 0, eightBit),

				edx(*this, 2, 0, thirtyTwoBit),
				dx(*this, 2, 0, sixteenBit),
				dh(*this, 2, 1, eightBit),
				dl(*this, 2, 0, eightBit),

				ebx(*this, 3, 0, thirtyTwoBit),
				 bx(*this, 3, 0, sixteenBit),
				 bh(*this, 3, 1, eightBit),
				 bl(*this, 3, 0, eightBit),

				esi(*this, 4, 0, thirtyTwoBit),
				six(*this, 4, 0, sixteenBit),
				sih(*this, 4, 1, eightBit),
				sil(*this, 4, 0, eightBit),

				edi(*this, 5, 0, thirtyTwoBit),
				dix(*this, 5, 0, sixteenBit),
				dih(*this, 5, 1, eightBit),
				dil(*this, 5, 0, eightBit),

				tix(*this, 6, 0, thirtyTwoBit),
				 st(*this, 6, 2, sixteenBit),
				 ob(*this, 6, 2, eightBit),
				 tb(*this, 6, 3, eightBit),
				 ct(*this, 6, 0, sixteenBit),
				 pp(*this, 6, 0, eightBit),
				 tp(*this, 6, 1, eightBit),

				ssx(*this, 8, 0, thirtyTwoBit),
				 hv(*this, 8, 2, sixteenBit),
				 ho(*this, 8, 2, eightBit),
				 ht(*this, 8, 3, eightBit),
				 ss(*this, 8, 0, sixteenBit),
				 cs(*this, 8, 0, eightBit),
				 sf(*this, 8, 1, eightBit),

				sui(*this, 9, 0, thirtyTwoBit),
				asi(*this, 9, 0, sixteenBit),
				gsi(*this, 9, 2, sixteenBit),
			
				eip(*this, 10, 0, thirtyTwoBit),
				 ip(*this, 10, 0, sixteenBit),
				 ih(*this, 10, 1, eightBit),
				 il(*this, 10, 0, eightBit),

				sip(*this, 11, 0, thirtyTwoBit),

				argumentStack(&argumentPage, argumentStackSize),
				generalStack(&generalPage, generalStackSize)
			{

			}

			void LogInstruction();
		};



	}
}

#endif