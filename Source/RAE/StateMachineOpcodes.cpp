#include "../include/StateMachine.h"
#include "../include/stdio.h"
#include "../include/ValueCategories.h"


// Global operators
BBP::userspace::StateMachine::arithmeticOperation addition   = [](BBP::std::word& a1, BBP::std::word& a2) { return a1 + a2; };
BBP::userspace::StateMachine::arithmeticOperation additionf = [](BBP::std::word& a1, BBP::std::word& a2) { float a1f = *((float*)&a1); float result = a1f + a2; BBP::std::word iresult = *((BBP::std::word*)&result); return iresult; };
BBP::userspace::StateMachine::arithmeticOperation additionff = [](BBP::std::word& a1, BBP::std::word& a2) { float a1f = *((float*)&a1); float a2f = *((float*)&a2); float result = a1f + a2f; BBP::std::word iresult = *((BBP::std::word*)&result); return iresult; };
BBP::userspace::StateMachine::arithmeticOperation additionif = [](BBP::std::word& a1, BBP::std::word& a2) { float a2f = *((float*)&a2); float result = a1 + a2f; BBP::std::word iresult = *((BBP::std::word*)&result); return iresult; };

BBP::userspace::StateMachine::arithmeticOperation subtraction = [](BBP::std::word& a1, BBP::std::word& a2) { return a1 - a2; };
BBP::userspace::StateMachine::arithmeticOperation subtractionf = [](BBP::std::word& a1, BBP::std::word& a2) { float a1f = *((float*)&a1); float result = a1f - a2; BBP::std::word iresult = *((BBP::std::word*)&result); return iresult; };
BBP::userspace::StateMachine::arithmeticOperation subtractionff = [](BBP::std::word& a1, BBP::std::word& a2) { float a1f = *((float*)&a1); float a2f = *((float*)&a2); float result = a1f - a2f; BBP::std::word iresult = *((BBP::std::word*)&result); return iresult; };
BBP::userspace::StateMachine::arithmeticOperation subtractionif = [](BBP::std::word& a1, BBP::std::word& a2) { float a2f = *((float*)&a2); float result = a1 - a2f; BBP::std::word iresult = *((BBP::std::word*)&result); return iresult; };

BBP::userspace::StateMachine::arithmeticOperation division = [](BBP::std::word& a1, BBP::std::word& a2) { return a1 / a2; };
BBP::userspace::StateMachine::arithmeticOperation divisionf = [](BBP::std::word& a1, BBP::std::word& a2) { float a1f = *((float*)&a1); float result = a1f / a2; BBP::std::word iresult = *((BBP::std::word*)&result); return iresult; };
BBP::userspace::StateMachine::arithmeticOperation divisionff = [](BBP::std::word& a1, BBP::std::word& a2) { float a1f = *((float*)&a1); float a2f = *((float*)&a2); float result = a1f / a2f; BBP::std::word iresult = *((BBP::std::word*)&result); return iresult; };
BBP::userspace::StateMachine::arithmeticOperation divisionif = [](BBP::std::word& a1, BBP::std::word& a2) { float a2f = *((float*)&a2); float result = a1 / a2f; BBP::std::word iresult = *((BBP::std::word*)&result); return iresult; };

BBP::userspace::StateMachine::arithmeticOperation multiplication = [](BBP::std::word& a1, BBP::std::word& a2) { return a1 * a2; };
BBP::userspace::StateMachine::arithmeticOperation multiplicationf = [](BBP::std::word& a1, BBP::std::word& a2) { float a1f = *((float*)&a1); float result = a1f * a2; BBP::std::word iresult = *((BBP::std::word*)&result); return iresult; };
BBP::userspace::StateMachine::arithmeticOperation multiplicationff = [](BBP::std::word& a1, BBP::std::word& a2) { float a1f = *((float*)&a1); float a2f = *((float*)&a2); float result = a1f * a2f; BBP::std::word iresult = *((BBP::std::word*)&result); return iresult; };
BBP::userspace::StateMachine::arithmeticOperation multiplicationif = [](BBP::std::word& a1, BBP::std::word& a2) { float a2f = *((float*)&a2); float result = a1 * a2f; BBP::std::word iresult = *((BBP::std::word*)&result); return iresult; };

BBP::userspace::StateMachine::arithmeticOperation modulo = [](BBP::std::word& a1, BBP::std::word& a2) { return a1 % a2; };

BBP::userspace::StateMachine::arithmeticOperation illegalOperation = [](BBP::std::word& a1, BBP::std::word& a2) { __SIGNAL__(SIGILL); return a1 % a2; };

void BBP::userspace::StateMachine::Compare(std::word &argument1)
{

}

void BBP::userspace::StateMachine::Compare(std::word &argument1, std::word &argument2)
{

}

BBP::std::word BBP::userspace::StateMachine::Operate(arithmeticOperation operation, std::word &arg1, std::word &arg2)
{

	// First compare both values
	Compare(arg1, arg2);

	// Basically just this
	std::word result = operation(arg1, arg2);

	return result;
}


BBP::std::word BBP::userspace::StateMachine::MathOperate(arithmeticOperation ii_, arithmeticOperation fi_, arithmeticOperation if_, arithmeticOperation ff_, std::word initial)
{

	std::byte minimumArguments = 0;
	std::byte maximumArguments = 0;
	arithmeticOperation operation;

	switch (activeThread->instruction.header.suffix)
	{
	default:
		minimumArguments = 2;
		maximumArguments = 8;
		operation = ii_;
		break;
	case userspace::ffloat:
		minimumArguments = 2;
		maximumArguments = 2;
		operation = fi_;
		break;
	case userspace::sfloat:
		minimumArguments = 2;
		maximumArguments = 2;
		operation = if_;
		break;
	case userspace::sfloat | userspace::ffloat:
		minimumArguments = 2;
		maximumArguments = 2;
		operation = ff_;
		break;
	}

	// Check if argument count fits what we specified
	std::byte argumentCount = activeThread->instruction.header.args;

	if (argumentCount < minimumArguments || argumentCount > maximumArguments)
	{
		__SIGNAL__(SIGILL);
	}

	std::word totalResult = initial;

	// Perform the required operation
	for (std::byte atArg = 0; atArg < argumentCount; atArg++)
	{
		// Get argument xvalue (as long as it can be read from)
		xvalue argXval = xvalue(*this, activeThread->instruction.args[atArg]);

		// If it cannot be read from, throw sigsegv
		if (argXval.canUserReadFrom(*this) == false)
		{
			__SIGNAL__(SIGSEGV);
		}

		// Resolve it
		std::word argData = argXval.resolve(*this);

		// Now operate on it
		totalResult = operation(totalResult, argData);
	}

	// Now 'compare' that result to set appropriate flags
	Compare(totalResult);

	std::printf("Calculated %u\n", totalResult);

	// Return result
	return totalResult;
}

void BBP::userspace::StateMachine::LogicOperate(arithmeticOperation nominal, arithmeticOperation binary)
{

	// Check if binary mode is enabled
	arithmeticOperation operation = (activeThread->instruction.header.suffix & userspace::bitwise) ? binary : nominal;

	// Check if arguments are exactly two
	if (activeThread->instruction.header.args != 2)
	{
		__SIGNAL__(SIGILL);
	}

	// Get two xvalues
	xvalue argument1 = xvalue(*this, activeThread->instruction.args[0]);
	xvalue argument2 = xvalue(*this, activeThread->instruction.args[1]);

	// If either of them cannot be written to, raise error
	if ((argument1.canUserReadFrom(*this) && argument2.canUserReadFrom(*this)) == false)
	{
		__SIGNAL__(SIGSEGV);
	}

	// Resolve arguments
	std::word argument1Data = argument1.resolve(*this);
	std::word argument2Data = argument2.resolve(*this);

	// Perform operation, then compare it
	std::word result = operation(argument1Data, argument2Data);
	Compare(result);

	std::printf("Calculated logic %u\n", result);

	// Create rvalue based on value
	rvalue rvalueResult = rvalue(result);

	// Now figure out if it is in place, if so, lvalue is first argument. Otherwise, it is eax
	if (activeThread->instruction.header.suffix & userspace::inplace)
	{
		// Get lvalue for first argument
		lvalue inplace = lvalue(*this, activeThread->instruction.args[0]);

		// If not writeable, throw signal
		if (inplace.canUserWriteTo(*this) == false)
		{
			__SIGNAL__(SIGSEGV);
		}

		// Assign rvalue and return
		inplace.assign(*this, rvalueResult, 4);
		return;
	}

	// Write result into eax
	setRegister(activeThread->eax, result);
}




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

	// Check if allowed to assign
	if (assignor.canUserWriteTo(*this) == false)
	{
		__SIGNAL__(SIGSEGV);
	}

	assignor.assign(*this, assignee, lvalueSize);
}
void BBP::userspace::StateMachine::INC()
{
	// Verify that there is exactly one argument
	if (activeThread->instruction.header.args != 1)
	{
		__SIGNAL__(SIGILL);
	}

	// Create operation, and select based on suffix
	arithmeticOperation operation;

	switch (activeThread->instruction.header.suffix)
	{
	case userspace::ffloat:
		operation = additionf;
		break;
	default:
		operation = addition;
		break;
	}

	// Get lvalue first operand
	lvalue incrementee = lvalue(*this, activeThread->instruction.args[0]);

	// If lvalue cannot be written nor read from, raise signal
	if ((incrementee.canUserReadFrom(*this) && incrementee.canUserWriteTo(*this)) == false)
	{
		__SIGNAL__(SIGSEGV);
	}

	// Next, get values as words
	std::word incrementeeValue = incrementee.resolve(*this);
	std::word incrementBy = 1;

	// Perform operation
	rvalue result = rvalue(Operate(operation, incrementeeValue, incrementBy));

	// Then assign value
	incrementee.assign(*this, result, 4);
}
void BBP::userspace::StateMachine::DEC()
{
	// Verify that there is exactly one argument
	if (activeThread->instruction.header.args != 1)
	{
		__SIGNAL__(SIGILL);
	}

	// Create operation, and select based on suffix
	arithmeticOperation operation;

	switch (activeThread->instruction.header.suffix)
	{
	case userspace::ffloat:
		operation = subtractionf;
		break;
	default:
		operation = subtraction;
		break;
	}

	// Get lvalue first operand
	lvalue subtractee = lvalue(*this, activeThread->instruction.args[0]);

	// If lvalue cannot be written nor read from, raise signal
	if ((subtractee.canUserReadFrom(*this) && subtractee.canUserWriteTo(*this)) == false)
	{
		__SIGNAL__(SIGSEGV);
	}

	// Next, get values as words
	std::word subtracteeValue = subtractee.resolve(*this);
	std::word subtractBy = 1;

	// Perform operation
	rvalue result = rvalue(Operate(operation, subtracteeValue, subtractBy));

	// Then assign value
	subtractee.assign(*this, result, 4);
}
void BBP::userspace::StateMachine::NEG()
{

}
void BBP::userspace::StateMachine::ADD()
{
	// Perform addition
	std::word _word = MathOperate(addition, additionf, additionif, additionff, 0);

	// Write result into eax
	setRegister(activeThread->eax, _word);
}
void BBP::userspace::StateMachine::SUB()
{
	// Perform subtraction
	std::word _word = MathOperate(subtraction, subtractionf, subtractionif, subtractionff, 0);

	// Write result into eax
	setRegister(activeThread->eax, _word);
}
void BBP::userspace::StateMachine::MUL()
{
	// Perform multiplication
	std::word _word = MathOperate(multiplication, multiplicationf, multiplicationif, multiplicationff, 1);

	// Write result into eax
	setRegister(activeThread->eax, _word);
}
void BBP::userspace::StateMachine::DIV()
{
	// Check if any argument is 0 (including floating point)
	float floatingPointZeroFloat = 0;
	std::word floatingPointZero = *((std::word*)&floatingPointZeroFloat);
	std::word integerZero = 0;

	// There must be exactly two arguments anyway
	if (activeThread->instruction.header.args != 2)
	{
		__SIGNAL__(SIGILL);
	}

	// Loop over each argument
	for (std::byte atArg = 0; atArg < activeThread->instruction.header.args; atArg++)
	{
		// Get xvalue
		xvalue val = xvalue(*this, activeThread->instruction.args[atArg]);

		// Resolve bytes
		std::word valBytes = val.resolve(*this);

		// Check if it zero
		if (valBytes == integerZero || valBytes == floatingPointZero)
		{
			__SIGNAL__(SIGFPE);
		}
	}

	// Perform division, since nothing is 0
	std::word _word = MathOperate(division, divisionf, divisionif, divisionff, 1);

	// Write result into eax
	setRegister(activeThread->eax, _word);

}
void BBP::userspace::StateMachine::MOD()
{
	// Check if any argument is 0 (including floating point)
	float floatingPointZeroFloat = 0;
	std::word floatingPointZero = *((std::word*)&floatingPointZeroFloat);
	std::word integerZero = 0;

	// There must be exactly two arguments anyway
	if (activeThread->instruction.header.args != 2)
	{
		__SIGNAL__(SIGILL);
	}

	// Loop over each argument
	for (std::byte atArg = 0; atArg < activeThread->instruction.header.args; atArg++)
	{
		// Get xvalue
		xvalue val = xvalue(*this, activeThread->instruction.args[atArg]);

		// Resolve bytes
		std::word valBytes = val.resolve(*this);

		// Check if it zero
		if (valBytes == integerZero || valBytes == floatingPointZero)
		{
			__SIGNAL__(SIGFPE);
		}
	}

	// Perform modulo, since nothing is 0
	std::word _word = MathOperate(modulo, illegalOperation, illegalOperation, illegalOperation, 1);

	// Write result into eax
	setRegister(activeThread->eax, _word);
}
void BBP::userspace::StateMachine::LBS()
{
	// Define operation
	arithmeticOperation operation = [](std::word& arg1, std::word& arg2) { return arg1 << arg2; };
	LogicOperate(operation, illegalOperation);
}
void BBP::userspace::StateMachine::RBS()
{
	arithmeticOperation operation = [](std::word& arg1, std::word& arg2) { return arg1 >> arg2; };
	LogicOperate(operation, illegalOperation);
}
void BBP::userspace::StateMachine::OR()
{
	arithmeticOperation nominal = [](std::word& arg1, std::word& arg2) { return (std::word)(arg1 || arg2); };
	arithmeticOperation bitwise = [](std::word& arg1, std::word& arg2) { return arg1 | arg2; };
	LogicOperate(nominal, bitwise);
}
void BBP::userspace::StateMachine::XOR()
{
	arithmeticOperation operation = [](std::word& arg1, std::word& arg2) { return arg1 ^ arg2; };
	LogicOperate(operation, illegalOperation);
}
void BBP::userspace::StateMachine::AND()
{
	arithmeticOperation nominal = [](std::word& arg1, std::word& arg2) { return (std::word)(arg1 && arg2); };
	arithmeticOperation bitwise = [](std::word& arg1, std::word& arg2) { return arg1 & arg2; };
	LogicOperate(nominal, bitwise);
}
void BBP::userspace::StateMachine::NOT()
{
	arithmeticOperation nominal = [](std::word& arg1, std::word& arg2) { return (std::word)!arg1; };
	arithmeticOperation bitwise = [](std::word& arg1, std::word& arg2) { return ~arg1; };
	LogicOperate(nominal, bitwise);
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