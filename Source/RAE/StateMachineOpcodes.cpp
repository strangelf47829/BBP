#include "../include/StateMachine.h"
#include "../include/stdio.h"
#include "../include/ValueCategories.h"


void BBP::userspace::StateMachine::NIL() 
{

}

void BBP::userspace::StateMachine::STACK()
{
	// Get stack
	std::Stack<std::word> *activeStack;

	// If of type push: [STACK]$ lvalue
	xvalue pushFirstArgument;

	// If of type pop: [STACK]$ xvalue
	lvalue popFirstArgument;

	// If argument, we want exactly one argument, Which must always be lvalue
	if (activeThread->instruction.header.args != 1)
	{
		// If not exactly two arguments, throw signal
		__SIGNAL__(SIGILL);
	}

	// Do things based on if stack is argument or not
	if (activeThread->instruction.header.suffix & userspace::argument)
	{
		// If we are pushing a type argument call, throw SIGILL
		if (activeThread->instruction.header.suffix == userspace::push)
		{
			__SIGNAL__(SIGILL);
		}

		activeStack = &activeThread->argumentStack;
	}
	else
	{
		activeStack = &activeThread->generalStack;
	}

	// If active thread
	std::word stackOperandNumerical = 0;
	rvalue stackOperand(0);
	
	switch (activeThread->instruction.header.suffix)
	{

	case userspace::push:
		// Get first argument as xvalue (since it doesn't matter if its lvalue or rvalue)
		pushFirstArgument = xvalue(*this, activeThread->instruction.args[0]);

		// Resolve xvalue to get stack operand
		stackOperandNumerical = pushFirstArgument.resolve(*this);

		// Then push that value into the active thread stack
		*activeStack << stackOperandNumerical;
		break;


	case userspace::pop:
		// Get first argument as lvalue (since we wan't to assign a value into xvalue)
		popFirstArgument = lvalue(*this, activeThread->instruction.args[0]);

		// Stack operand is value popped from active stack
		*activeStack >> stackOperandNumerical;
		stackOperand = rvalue(stackOperandNumerical);

		// Then push that value into the active thread stack
		popFirstArgument.assign(*this, stackOperand, 4);
		break;
	}
	
}
void BBP::userspace::StateMachine::CTRL()
{

}
void BBP::userspace::StateMachine::MEM() 
{
	// The mem opcode handles things like moving values between one another. 
	// Here, mem always expects two arguments. The lvalue to be assigned to, and the rvalue to assign from.

	lvalue assignor;
	rvalue assignee;

	// Two flags dictate the size of the assignment.
	std::byte lvalueSize = 0;
	std::word rvalueSize = 0;

	// The suffix encodes the size of each value using a bitfield.
	switch (activeThread->instruction.header.suffix & 0b11110000)
	{
	case MEMSUFFIXES::sbyte:
		rvalueSize = 0b11111111;
		break;
	case MEMSUFFIXES::shalfword:
		rvalueSize = 0b1111111111111111;
		break;
	case MEMSUFFIXES::saddress:
		rvalueSize = 0b11111111111111111111;
		break;
	case MEMSUFFIXES::sword:
		rvalueSize = 0b11111111111111111111111111111111;
		break;
	}

	switch (activeThread->instruction.header.suffix & 0b1111)
	{
	case MEMSUFFIXES::fbyte:
		lvalueSize = 1;
		break;
	case MEMSUFFIXES::fhalfword:
		lvalueSize = 2;
		break;
	case MEMSUFFIXES::faddress:
		lvalueSize = 3;
		break;
	case MEMSUFFIXES::fword:
		lvalueSize = 4;
		break;
	}

	// Next, get argument one and two. If not exactly two arguments, throw signal
	if (activeThread->instruction.header.args != 2)
	{
		__SIGNAL__(SIGILL);
	}

	// Assign values
	assignor = lvalue(*this, activeThread->instruction.args[0]);
	assignee = rvalue(*this, activeThread->instruction.args[1]);

	// Get assigned value
	std::word assignValue = assignee.resolve(*this);
	assignee = rvalue(assignValue & rvalueSize);
	assignor.assign(*this, assignee, lvalueSize);
}
void BBP::userspace::StateMachine::INC()
{

}
void BBP::userspace::StateMachine::DEC()
{

}
void BBP::userspace::StateMachine::NEG()
{

}
void BBP::userspace::StateMachine::ADD()
{

}
void BBP::userspace::StateMachine::SUB()
{

}
void BBP::userspace::StateMachine::MUL()
{

}
void BBP::userspace::StateMachine::DIV()
{

}
void BBP::userspace::StateMachine::MOD()
{

}
void BBP::userspace::StateMachine::LBS()
{

}
void BBP::userspace::StateMachine::RBS()
{

}
void BBP::userspace::StateMachine::OR()
{

}
void BBP::userspace::StateMachine::XOR()
{

}
void BBP::userspace::StateMachine::AND()
{

}
void BBP::userspace::StateMachine::NOT()
{

}
void BBP::userspace::StateMachine::FLAG()
{

}
void BBP::userspace::StateMachine::JMP()
{

}
void BBP::userspace::StateMachine::JMPZ()
{

}
void BBP::userspace::StateMachine::JMPC()
{

}
void BBP::userspace::StateMachine::PAGE()
{

}
void BBP::userspace::StateMachine::SIGNAL()
{

}
void BBP::userspace::StateMachine::HALT()
{

}