#ifndef BBP_LIB_STATEMACHINE
#define BBP_LIB_STATEMACHINE

#include "Opcodes.h"
#include "Threading.h"
#include "Hyperv.h"
#include "ValueCategories.h"

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

			// Used to update arithmetic functions
			void Compare(std::word&);
			void Compare(std::word&, std::word&);

		public:

			// Used to define arithmetic operations done on variables
			using arithmeticOperation = std::word(*)(std::word&, std::word&);

			// Call functions
			void callFunction(std::word address, userspace::lvalue &returnInfo, std::byte argCount, std::word args[7]);
			void callFunction(std::word address, std::byte argCount, std::word args[7]);

			// First argument is lvalue to write result to. Second and third argument are arguments for the operation
			std::word Operate(arithmeticOperation, std::word&, std::word&);
			std::word MathOperate(arithmeticOperation ii_, arithmeticOperation fi_, arithmeticOperation if_, arithmeticOperation ff_, std::word);
			void LogicOperate(arithmeticOperation nominal, arithmeticOperation binary);

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