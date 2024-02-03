#ifndef BBP_LIB_STATEMACHINE
#define BBP_LIB_STATEMACHINE

#include "Opcodes.h"
#include "Threading.h"
#include "Hyperv.h"

namespace BBP
{
	namespace userspace
	{
		class StateMachine
		{
			userspace::HyperVisor *activeHypervisor;
			userspace::Thread *activeThread;

			// Stuff
			void NIL();
			void STACK();
			void CTRL();
			void MEM();
			void INC();
			void DEC();
			void NEG();
			void ADD();
			void SUB();
			void MUL();
			void DIV();
			void MOD();
			void LBS();
			void RBS();
			void OR();
			void XOR();
			void AND();
			void NOT();
			void FLAG();
			void JMP();
			void JMPZ();
			void JMPC();
			void PAGE();
			void SIGNAL();
			void HALT();

		public:

			// Set active threads
			void setActiveHypervisor(userspace::HyperVisor *);
			void setActiveThread(userspace::Thread *);

			// Thread stuff
			void cycleThread();

			// Executing
			void execute();

			// Getter and setter
			Thread &getActiveThread();


		};
	}
}

#endif