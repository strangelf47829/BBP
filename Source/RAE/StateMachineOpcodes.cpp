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

BBP::userspace::StateMachine::arithmeticOperation illegalOperation = [](BBP::std::word& a1, BBP::std::word& a2) { BBP::std::raise(BBP::std::SIGILL); return a1 % a2; };

void BBP::userspace::StateMachine::Compare(std::word &argument1, std::word &result)
{
	std::word compareTo = readRegister(activeThread->eax);
	Compare(argument1, compareTo, result);
}

void BBP::userspace::StateMachine::Compare(std::word &argument1, std::word &argument2, std::word &result)
{
	// Check if argument1 is bigger than argument2
	bool greaterThan = argument1 > argument2;

	// Check if argument1 is less than argument2
	bool lessThan = argument1 < argument2;

	// Check if argument1 is equal to argument2
	bool equalTo = argument1 == argument2;

	// Check if result is 0
	bool resultEqualTo = result == 0;

	// Check if signed result is greater or less than 0
	int signedResult = (int)result;

	bool resultGreaterThan = signedResult > 0;
	bool resultLessThan = signedResult < 0;

	// Set flags
	activeThread->flags = greaterThan * activeThread->greaterThan;
	activeThread->flags |= lessThan * activeThread->lessThan;
	activeThread->flags |= equalTo * activeThread->equalTo;
	activeThread->flags |= resultGreaterThan * activeThread->resultGreaterThan;
	activeThread->flags |= resultLessThan * activeThread->resultLessThan;
	activeThread->flags |= resultEqualTo * activeThread->resultEqualTo;

	//std::printf("Compare %u, %u, %u?: %d %d %d %d %d %d\n", argument1, argument2, result, greaterThan, lessThan, equalTo, resultGreaterThan, resultLessThan, resultEqualTo);

}

BBP::std::word BBP::userspace::StateMachine::Operate(arithmeticOperation operation, std::word &arg1, std::word &arg2)
{
	// Operate on the thingy
	std::word result = operation(arg1, arg2);

	// First compare both values
	Compare(arg1, arg2, result);

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
		std::raise(std::SIGILL);

	std::word totalResult = 0;

	// If initial value is set, and its value is below or equal to 8, set this to whatever the initial value is.
	if (initial && initial <= 8)
		totalResult = xvalue(*this, activeThread->instruction.args[initial - 1]).resolve(*this);
	else if (initial)
		totalResult = initial - 9; // Otherwise set it to this.

	std::word firstArgument = 0;
	std::word secondArgument = 0;

	// Perform the required operation
	for (std::byte atArg = 0; atArg < argumentCount; atArg++)
	{
		// Get argument xvalue (as long as it can be read from)
		xvalue argXval = xvalue(*this, activeThread->instruction.args[atArg]);

		// If it cannot be read from, throw sigsegv
		if (argXval.canUserReadFrom(*this) == false)
			std::raise(std::SIGSEGV);

		// Resolve it
		std::word argData = argXval.resolve(*this);

		if (atArg == 0)
			firstArgument = argData;
		else if (atArg == 1)
			secondArgument = argData;

		// if initial value is set, skip at arg
		if (initial && atArg + 1 == initial)
			continue;

		// Now operate on it
		totalResult = operation(totalResult, argData);
	}

	// Now 'compare' that result to set appropriate flags
	if (activeThread->instruction.header.args == 1)
		Compare(firstArgument, totalResult);
	else if (activeThread->instruction.header.args == 2)
		Compare(firstArgument, secondArgument, totalResult);

	// Return result
	return totalResult;
}

void BBP::userspace::StateMachine::LogicOperate(arithmeticOperation nominal, arithmeticOperation binary)
{

	// Check if binary mode is enabled
	arithmeticOperation operation = (activeThread->instruction.header.suffix & userspace::bitwise) ? binary : nominal;

	// Check if arguments are exactly two
	if (activeThread->instruction.header.args != 2)
		std::raise(std::SIGILL);

	// Get two xvalues
	xvalue argument1 = xvalue(*this, activeThread->instruction.args[0]);
	xvalue argument2 = xvalue(*this, activeThread->instruction.args[1]);

	// If either of them cannot be written to, raise error
	if ((argument1.canUserReadFrom(*this) && argument2.canUserReadFrom(*this)) == false)
		std::raise(std::SIGSEGV);

	// Resolve arguments
	std::word argument1Data = argument1.resolve(*this);
	std::word argument2Data = argument2.resolve(*this);

	// Perform operation, then compare it
	std::word result = operation(argument1Data, argument2Data);
	Compare(argument1Data, argument2Data, result);

	// Create rvalue based on value
	rvalue rvalueResult = rvalue(result);

	// Now figure out if it is in place, if so, lvalue is first argument. Otherwise, it is eax
	if (activeThread->instruction.header.suffix & userspace::inplace)
	{
		// Get lvalue for first argument
		lvalue inplace = lvalue(*this, activeThread->instruction.args[0]);

		// If not writeable, throw signal
		if (inplace.canUserWriteTo(*this) == false)
			std::raise(std::SIGSEGV);

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

	// Check for push or pop
	if (activeThread->instruction.header.suffix & userspace::pop)
	{
		// Set some variables
		std::Stack<std::word> *popFrom = nullptr;
		std::byte expectedArguments = 0;

		// Check for argument flag
		if (activeThread->instruction.header.suffix & userspace::argument)
		{
			popFrom = &activeThread->argumentStack;
			expectedArguments = 2;
		}
		else
		{
			popFrom = &activeThread->generalStack;
			expectedArguments = 1;
		}

		// Check for expected arguments
		if (activeThread->instruction.header.args != expectedArguments)
			std::raise(std::SIGILL);

		// Get first lvalue
		lvalue firstArg = lvalue(*this, activeThread->instruction.args[0]);

		// Now, if not argument, just get a value from stack and return
		if ((activeThread->instruction.header.suffix & userspace::argument) == false)
		{
			// Pop from stack
			std::word popped = 0;
			(*popFrom) >> popped;

			// Assign to lvalue
			firstArg.assign(*this, popped, 4);

			// Done. Now return.
			return;
		}

		// Get second argument xvalue
		xvalue secondArg = xvalue(*this, activeThread->instruction.args[1]);

		// Now resolve the argument index
		std::word argumentIndex = secondArg.resolve(*this);

		// Peek argument count
		std::word argumentCount = (*popFrom)[popFrom->atElement - 1];

		// Check if argument is within bounds.
		if (argumentIndex >= argumentCount)
			std::raise(std::SIGILL);

		// Peek value
		std::word popped = (*popFrom)[popFrom->atElement - (argumentCount - argumentIndex) - 1];

		// Assign to first argument, then return
		firstArg.assign(*this, popped, 4);
		return;
	}

	// We are pushing. Check if the argument flag is set.
	if (activeThread->instruction.header.suffix & userspace::argument)
		std::raise(std::SIGILL);

	// Check for exactly one argument
	if (activeThread->instruction.header.args != 1)
		std::raise(std::SIGILL);

	// Get xvalue
	xvalue firstArg = xvalue(*this, activeThread->instruction.args[0]);

	// Get value
	std::word pushing = firstArg.resolve(*this);

	// Now push that 
	activeThread->generalStack << pushing;
	
}
void BBP::userspace::StateMachine::CTRL()
{

	// Check if instruction is a routine
	if (activeThread->instruction.header.suffix & userspace::routine)
	{
		// Check whether routine is absolute or not
		bool absolute = !!(activeThread->instruction.header.suffix & userspace::absolute);

		// If not absolute signal SIGILL, TODO: Implement non-trivial routines
		if (absolute == false)
			std::raise(std::SIGILL);
		
		// Get target address
		xvalue tgtAddressxv = xvalue(*this, activeThread->instruction.args[0]);
		std::word tgtAddress = tgtAddressxv.resolve(*this) + 0x20000;

		// Add address to stack
		activeThread->routineAddress << tgtAddress;
		return;
	}


	// Check if the instruction is a syscall
	if (activeThread->instruction.header.suffix & userspace::syscall)
	{
		// If no arguments given, throw signal
		if (activeThread->instruction.header.args < 1)
			std::raise(std::SIGILL);

		// Resolve first argument.
		std::index_t callNum = xvalue(*this, activeThread->instruction.args[0]).resolve(*this);

		// If callNum is above the amount of system calls available, throw sigill
		if (callNum >= activeHypervisor->systemCallCount)
			std::raise(std::SIGILL);

		// Get function pointer
		HyperVisor::syscall_t call = activeHypervisor->systemcalls[callNum];

		// If it is null, throw signal
		if (call == nullptr)
			std::raise(std::SIGINV);

		// Call the system call.
		std::word returned = call(this, activeHypervisor, activeThread, activeThread->instruction);

		// Then write result into ebx
		setRegister(activeThread->ebx, returned);

		return;
	}

	// Check if this is a return instruction
	if (activeThread->instruction.header.suffix & userspace::ret)
	{
		// Get returning argument count
		std::word argumentCount = 0;
		activeThread->argumentStack >> argumentCount;

		// Now go back those amount of times
		for (std::byte arg = 0; arg < argumentCount; arg++)
		{
			std::word bin = 0;
			activeThread->argumentStack >> bin;			
		}

		// Get virtual address to write into
		std::word returnObjectVirtualAddress = 0;
		activeThread->argumentStack >> returnObjectVirtualAddress;

		// Now create lvalue for that virtual address
		lvalue returnObject = lvalue(*this, returnObjectVirtualAddress);
		
		// Check if there is at least one argument
		if (activeThread->instruction.header.args != 1)
			std::raise(std::SIGILL);

		// Get object to be returned.
		std::word returnValue = rvalue(*this, activeThread->instruction.args[0]).resolve(*this);

		// Store that return value in earlier lvalue
		returnObject.assign(*this, returnValue, 4);

		// Get EIP from argument stack
		std::word newEIP = 0;
		activeThread->argumentStack >> newEIP;

		// Restore EIP
		setRegister(activeThread->eip, newEIP);

		return;
	}


	// Calling something, just try this
	xvalue callingAddress = xvalue(*this, activeThread->instruction.args[0]);

	// Create argument list
	std::word args[7] = { 0, 0, 0, 0, 0, 0, 0 };

	// Now go through each argument to add to the arg Count
	std::byte startAt = 1 + !!(activeThread->instruction.header.suffix & userspace::callp);

	// Check minimum arguments
	if (activeThread->instruction.header.args < startAt)
		std::raise(std::SIGILL);

	// Now add arguments
	for (std::byte arg = startAt; arg < activeThread->instruction.header.args; arg++)
		args[arg - startAt] = rvalue(*this, activeThread->instruction.args[arg]).resolve(*this);

	// Call the function
	if (activeThread->instruction.header.suffix & userspace::callp)
	{
		lvalue returnObject = lvalue(*this, activeThread->instruction.args[1]);
		callFunction(callingAddress.resolve(*this), returnObject, activeThread->instruction.header.args - startAt, args);
	}
	{
		callFunction(callingAddress.resolve(*this), activeThread->instruction.header.args - startAt, args);
	}

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
		std::raise(std::SIGILL);

	// Assign values
	assignor = lvalue(*this, activeThread->instruction.args[0]);
	assignee = rvalue(*this, activeThread->instruction.args[1]);

	// Get assigned value
	std::word assignValue = assignee.resolve(*this);
	assignee = rvalue(assignValue & rvalueSize);

	// Check if allowed to assign
	if (assignor.canUserWriteTo(*this) == false)
		std::raise(std::SIGSEGV);

	assignor.assign(*this, assignee, lvalueSize);
}
void BBP::userspace::StateMachine::INC()
{
	// Verify that there is exactly one argument
	if (activeThread->instruction.header.args != 1)
		std::raise(std::SIGILL);

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
		std::raise(std::SIGSEGV);

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
		std::raise(std::SIGILL);

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
		std::raise(std::SIGSEGV);

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
	std::word _word = MathOperate(subtraction, subtractionf, subtractionif, subtractionff, 1);

	// Write result into eax
	setRegister(activeThread->eax, _word);
}
void BBP::userspace::StateMachine::MUL()
{
	// Perform multiplication
	std::word _word = MathOperate(multiplication, multiplicationf, multiplicationif, multiplicationff, 10);

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
		std::raise(std::SIGILL);

	// Loop over each argument
	for (std::byte atArg = 0; atArg < activeThread->instruction.header.args; atArg++)
	{
		// Get xvalue
		xvalue val = xvalue(*this, activeThread->instruction.args[atArg]);

		// Resolve bytes
		std::word valBytes = val.resolve(*this);

		// Check if it zero
		if ((valBytes == integerZero || valBytes == floatingPointZero) && atArg != 0)
			std::raise(std::SIGFPE);
	}

	// Perform division, since nothing is 0
	std::word _word = MathOperate(division, divisionf, divisionif, divisionff, 1);

	//std::printf("Division float result: %f\n", _word);

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
		std::raise(std::SIGILL);

	// Loop over each argument
	for (std::byte atArg = 0; atArg < activeThread->instruction.header.args; atArg++)
	{
		// Get xvalue
		xvalue val = xvalue(*this, activeThread->instruction.args[atArg]);

		// Resolve bytes
		std::word valBytes = val.resolve(*this);

		// Check if it zero (except for first argument0
		if ((valBytes == integerZero || valBytes == floatingPointZero) && atArg != 0)
			std::raise(std::SIGFPE);
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
	// Check if instruction not a comparison
	if (activeThread->instruction.header.suffix == 1)
	{
		// Not implemented
		std::raise(std::SIGILL);
	}

	// Declare two arguments already
	std::word argument1 = 0;
	std::word argument2 = 0;

	// Declare result
	std::word result = 0;

	// Get argument count
	std::byte argumentCount = activeThread->instruction.header.args;

	// if arguments is 1:
	if (argumentCount >= 1)
		argument1 = xvalue(*this, activeThread->instruction.args[0]).resolve(*this);
	
	if (argumentCount >= 2)
		argument2 = xvalue(*this, activeThread->instruction.args[1]).resolve(*this);

	if (argumentCount >= 3)
		result = xvalue(*this, activeThread->instruction.args[2]).resolve(*this);

	result = argument1 - argument2;
	//std::printf("Result: %u - %u = %u\n", argument1, argument2, result);

	if (argumentCount > 3)
		std::raise(std::SIGILL);

	// Now do a comparison based on argument count
	switch (argumentCount)
	{
	case 1:
		Compare(argument1, result);
		break;
	case 2:
		// Implicit result
		
		Compare(argument1, argument2, result);
		break;
	case 3:
		Compare(argument1, argument2, result);
		break;
	}

}
void BBP::userspace::StateMachine::JMP()
{

	// Check if jump is routinal
	if (activeThread->instruction.header.suffix & userspace::jmproutine)
	{
		// Check if there is a current routine
		if (activeThread->routineAddress.atElement == 0)
			std::raise(std::SIGINV); // Invalid instruction because missing routine, not because ill-formed syntax

		// Get target address
		std::word tgt = activeThread->routineAddress[activeThread->routineAddress.atElement - 1];

		// Expect endrt and jump to that address
		activeThread->expectsEndrt = true;
		setRegister(activeThread->eip, tgt);
		return;
	}

	// Check if at least one argument is present
	if (activeThread->instruction.header.args != 1)
		std::raise(std::SIGILL);

	// Get first value
	xvalue val = xvalue(*this, activeThread->instruction.args[0]);

	// Get address to jump to.
	std::address_t jumpTo = val.resolve(*this);

	// If jump is not absolute, add 0x20004 to address
	if (!!(activeThread->instruction.header.suffix & userspace::absoluteJmp))
		jumpTo += 0x20004;

	// Jump to address
	setRegister(activeThread->eip, jumpTo);
}
void BBP::userspace::StateMachine::JMPZ()
{

	// Create a boolean value to check if the conditions to jump have been met
	bool shouldExecute = false;

	// Check for less than (zero) flag
	if (activeThread->instruction.header.suffix & userspace::lessthan)
		shouldExecute |= activeThread->flags & activeThread->resultLessThan;

	// Check for greater than (zero) flag
	if (activeThread->instruction.header.suffix & userspace::greaterthan)
		shouldExecute |= activeThread->flags & activeThread->resultGreaterThan;

	// Check for equal to (zero) flag
	if (activeThread->instruction.header.suffix & userspace::equalto)
		shouldExecute |= activeThread->flags & activeThread->resultEqualTo;

	// Negate evaluated value if negation flag is set
	if (activeThread->instruction.header.suffix & userspace::negated)
		shouldExecute = !shouldExecute;

	// If should not execute, do not execute
	if (shouldExecute == false)
		return;

	// Otherwise jump
	JMP();


}
void BBP::userspace::StateMachine::JMPC()
{

	// Create a boolean value to check if the conditions to jump have been met
	bool shouldExecute = false;

	// Check for less than (zero) flag
	if (activeThread->instruction.header.suffix & userspace::lessthan)
		shouldExecute |= activeThread->flags & activeThread->lessThan;

	// Check for greater than (zero) flag
	if (activeThread->instruction.header.suffix & userspace::greaterthan)
		shouldExecute |= activeThread->flags & activeThread->greaterThan;

	// Check for equal to (zero) flag
	if (activeThread->instruction.header.suffix & userspace::equalto)
		shouldExecute |= activeThread->flags & activeThread->equalTo;

	// Negate evaluated value if negation flag is set
	if (activeThread->instruction.header.suffix & userspace::negated)
		shouldExecute = !shouldExecute;

	// If should not execute, do not execute
	if (shouldExecute == false)
		return;

	// Otherwise jump
	JMP();
}
void BBP::userspace::StateMachine::PAGE()
{

	// Get possible operations as constexpressions
	constexpr std::byte page = userspace::assign | userspace::read | userspace::write;
	constexpr std::byte pageaw = userspace::assign | userspace::write;
	constexpr std::byte pagear = userspace::assign | userspace::read;
	constexpr std::byte pagew = userspace::write;
	constexpr std::byte pager = userspace::read;
	constexpr std::byte palloc = userspace::assign | userspace::memoryOp | userspace::sizeOp;
	constexpr std::byte pfree = userspace::memoryOp | userspace::sizeOp;
	constexpr std::byte pages = userspace::sizeOp | userspace::read;
	constexpr std::byte pagebc = pages | userspace::bytes;
	constexpr std::byte passign = userspace::assign | userspace::read | userspace::write | userspace::memoryOp;
	constexpr std::byte palign = userspace::assign | userspace::memoryOp;

	// Get first three arguments
	std::byte argCount = activeThread->instruction.header.args;
	xvalue firstArg;
	xvalue secondArg;
	xvalue thirdArg;

	if (argCount > 0)
		firstArg = xvalue(*this, activeThread->instruction.args[0]);

	if (argCount > 1)
		secondArg = xvalue(*this, activeThread->instruction.args[1]);

	if (argCount > 2)
		thirdArg = xvalue(*this, activeThread->instruction.args[2]);

	// Stupid juMp To CasE LabeL
	std::byte activePage = 0; 
	std::index_t index = 0;
	std::word value = 0;
	std::address_t address = 0;

	// Use a switch statement to get operation.
	switch (activeThread->instruction.header.suffix)
	{
		// If none found, throw sigill
	default:
		std::raise(std::SIGILL);
		return;

	case page:
	case pageaw:
	case pagear:
		// Check exactly one arg
		if (argCount != 1)
			std::raise(std::SIGILL);

		// Set R/W page based on case
		activePage = firstArg.resolve(*this);
		if (activeThread->instruction.header.suffix == page || activeThread->instruction.header.suffix == pageaw)
		{
			std::printf("Setting active write page to %u.\n", activePage);
			
			// Check if page is within bounds
			if (activePage >= activeThread->maxPages || activePage < 0)
				std::raise(std::SIGSEGV);
			
			activeThread->activeWPage = activePage;
		}
		if (activeThread->instruction.header.suffix == page || activeThread->instruction.header.suffix == pagear)
		{
			std::printf("Setting active read page to %u.\n", activePage);

			// Check if page is within bounds
			if (activePage >= activeThread->maxPages || activePage < 0)
				std::raise(std::SIGSEGV);

			activeThread->activeRPage = activePage;
		}

		activeThread->activeRPage = activePage;
		break;
	case pagew:

		// Get active write page
		activePage = activeThread->activeWPage;

		// Check for either 1 or 2 args.
		if (argCount == 1)
		{
			// If only one argument is supplied, retrieve index and set value to ebx
			index = firstArg.resolve(*this);
			value = readRegister(activeThread->ebx);
		}
		else if (argCount == 2)
		{
			// Otherwise, if two arguments are supplied, value is from second argument
			index = firstArg.resolve(*this);
			value = secondArg.resolve(*this);
		}
		else
		{
			// Otherwise, throw SIGILL
			std::raise(std::SIGILL);
		}

		// Check if user can write
		if (activeThread->pages[activePage].canUserWrite == false)
			std::raise(std::SIGSEGV);

		// Write value into page
		activeThread->pages[activePage].write(index, value);

		break;
	case pager:

		// Get active read page
		activePage = activeThread->activeRPage;

		// If there is not one or two arguments, throw SIGILL
		if (argCount < 1 || argCount > 2)
			std::raise(std::SIGILL);

		// Retrieve index, and read page
		index = firstArg.resolve(*this);
		value = activeThread->pages[activePage][index];

		// If argument count is 1, write into ebx
		if (argCount == 1)
			setRegister(activeThread->ebx, value);
		else
			secondArg.assign(*this, value, 4);

		break;

	case palloc:

		// There must be exactly two arguments.
		if (argCount != 2)
			std::raise(std::SIGILL);

		// Retrieve active page and requested size from arguments
		activePage = firstArg.resolve(*this);
		value = secondArg.resolve(*this);

		// Check if page is within bounds
		if (activePage >= activeThread->maxPages || activePage < 0)
			std::raise(std::SIGSEGV);

		// Allocate 'value' amount of words to page
		activeThread->pages[activePage] = std::DYN_PAGE(value, (std::word *)activeThread->allocator->calloc(value, sizeof(std::word)), std::DYN_PAGE::Alignment::HalfWord, true);

		break;
	case pfree:

		// There must be exactly one argument, namely the page to free
		if (argCount != 1)
			std::raise(std::SIGILL);

		// Retrieve the active page
		activePage = firstArg.resolve(*this);

		// Attempt to free that pages data
		activeThread->allocator->free(activeThread->pages[activePage].data);

		// Success, now invalidate that page
		activeThread->pages[activePage] = std::DYN_PAGE(0, nullptr, std::DYN_PAGE::Alignment::Unallocated, false);

		break;

	case pages:
	case pagebc:

		// Expect one argument 
		if (argCount != 1)
			std::raise(std::SIGILL);

		// Get active page from argument
		activePage = firstArg.resolve(*this);

		// Check if page is within bounds
		if (activePage >= activeThread->maxPages || activePage < 0)
			std::raise(std::SIGSEGV);

		// Get size based on pagebc
		if (activeThread->instruction.header.suffix == pages)
			value = activeThread->pages[activePage].dataSize;
		else if (activeThread->instruction.header.suffix == pagebc)
			value = activeThread->pages[activePage].bytes;

		// Write into eax
		setRegister(activeThread->eax, value);

		break;

	case passign:

		// Three arguments
		if (argCount != 3)
			std::raise(std::SIGILL);

		// Get active page, address and size.
		activePage = firstArg.resolve(*this);
		address = secondArg.getOwnPhysicalAddress();
		value = thirdArg.resolve(*this);

		// Check if page is within bounds
		if (activePage >= activeThread->maxPages || activePage < 0)
			std::raise(std::SIGSEGV);

		// Check if address is readable
		if (secondArg.canUserReadFrom(*this) == false)
			std::raise(std::SIGSEGV);

		// Set active page to that
		std::printf("Page %u assigned physical address 0x%04x\n", activePage, address);
		activeThread->pages[activePage] = std::DYN_PAGE(value, (std::word *)(&std::read(&activeThread->executable.BinaryData, address)), std::DYN_PAGE::Alignment::HalfWord, secondArg.canUserWriteTo(*this));

		break;

	case palign:

		// Check for exactly two arguments
		if (argCount != 2)
			std::raise(std::SIGILL);

		// Read arguments
		activePage = firstArg.resolve(*this);
		value = secondArg.resolve(*this);

		// Check if page is within bounds
		if (activePage >= activeThread->maxPages || activePage < 0)
			std::raise(std::SIGSEGV);

		// Set size
		activeThread->pages[activePage].setAlignment((std::DYN_PAGE::Alignment)value);

		break;

	}


}
void BBP::userspace::StateMachine::SIGNAL()
{



}
void BBP::userspace::StateMachine::HALT()
{

}

void BBP::userspace::StateMachine::SECR()
{
	// If endbranch instruction, check for matching argument count
	if (activeThread->instruction.header.suffix == userspace::endbr)
	{
		// If processor is not expecting endbr, raise security fault
		if (activeThread->expectsEndbr == false)
			std::raise(std::SIGSEC);

		// Get argument 0 data (always literal, since compiler emits instruction)
		std::word argumentCount = activeThread->instruction.args[0].data;

		// Get argument count from stack
		std::word presentedArgumentCount = activeThread->argumentStack[activeThread->argumentStack.atElement - 1];

		// If they are not the same, SIGSEC
		if (argumentCount != presentedArgumentCount)
			std::raise(std::SIGSEC);

		// Reset expecting endbr flag
		activeThread->expectsEndbr = false;

		return;

	}

	// If endroutine instruction;
	if (activeThread->instruction.header.suffix == userspace::endrt)
	{
		// Remove address from stack and continue.
		std::word bin = 0;
		activeThread->routineAddress >> bin;

		// Reset flag
		activeThread->expectsEndrt = false;
		return;

	}

}


