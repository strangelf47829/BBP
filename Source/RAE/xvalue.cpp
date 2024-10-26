#include "../include/StateMachine.h"
#include "../include/ValueCategories.h"
#include "../include/stdio.h"
#include "../include/Opcodes.h"
#include "../include/Threading.h"



BBP::userspace::xvalue::xvalue()
{

}

BBP::userspace::xvalue::xvalue(userspace::StateMachine &state, userspace::Instruction::Arg &arg)
{
	// If argument is of type LValue
	if (isArgumentLValue(arg))
	{
		// Set this to lvalue
		lval = lvalue(state, arg);

		// Set Flags
		isLvalue = true;
		isRvalue = false;

		return;
	}

	// If argument is of type RValue
	if (isArgumentRValue(arg))
	{
		// Set this to lvalue
		rval = rvalue(state, arg);

		// Set Flags
		isRvalue = true;
		isLvalue = false;

		return;
	}

	// Uninitialized
	std::raise(std::SIGSEGV);
}

void BBP::userspace::xvalue::dereference(userspace::StateMachine &state, std::address_t lvalueFor)
{
	if (isLvalue)
	{
		lval.dereference(state, lvalueFor);
		return;
	}
	if (isRvalue)
	{
		rval.dereference(state, lvalueFor);
		return;
	}

	// Uninitialized value
	std::raise(std::SIGSEGV);
}

void BBP::userspace::xvalue::reference(userspace::StateMachine &state, pvalue &value)
{
	if (isLvalue)
		lval.reference(state, value);
	else if (isRvalue)
		rval.reference(state, value);

	// Uninitialized
	std::raise(std::SIGSEGV);
}

BBP::std::address_t BBP::userspace::xvalue::getOwnPhysicalAddress()
{
	if (isLvalue)
		return lval.getOwnPhysicalAddress();
	if (isRvalue)
		return rval.getOwnVirtualAddress();

	// Uninitialized value
	std::raise(std::SIGSEGV);

	// Throw error
	throw 0;
}

BBP::std::address_t BBP::userspace::xvalue::getOwnVirtualAddress()
{
	if (isLvalue)
		return lval.getOwnVirtualAddress();
	if (isRvalue)
		return rval.getOwnVirtualAddress();

	// Uninitialized value
	std::raise(std::SIGSEGV);

	// Throw error
	throw 0;
}

BBP::std::word BBP::userspace::xvalue::resolve(userspace::StateMachine &state)
{
	if (isLvalue)
		return lval.resolve(state);
	if (isRvalue)
		return rval.resolve(state);

	// Uninitialized value
	std::raise(std::SIGSEGV);

	// Throw error
	throw 0;
}

void BBP::userspace::xvalue::assign(userspace::StateMachine &state, pvalue &assignee, std::byte bytes)
{
	if (isLvalue)
		lval.assign(state, assignee, bytes);
	else if (isRvalue)
		rval.assign(state, assignee, bytes);
	else
	{
		// Unassigned
		std::raise(std::SIGSEGV);
	}
}

void BBP::userspace::xvalue::assign(userspace::StateMachine &state, std::word data, std::byte bytes)
{
	if (isLvalue)
		lval.assign(state, data, bytes);
	else if (isRvalue)
		rval.assign(state, data, bytes);
	else
	{
		// Unassigned
		std::raise(std::SIGSEGV);
	}
}

bool BBP::userspace::xvalue::canUserExecuteFrom(userspace::StateMachine& state)
{
	if (isLvalue)
		return lval.canUserExecuteFrom(state);
	else if (isRvalue)
		return rval.canUserExecuteFrom(state);

	// Uninitialized
	std::raise(std::SIGSEGV);

	// Throw error
	throw 0;
}

bool BBP::userspace::xvalue::canUserReadFrom(userspace::StateMachine& state)
{
	if (isLvalue)
		return lval.canUserReadFrom(state);
	else if (isRvalue)
		return rval.canUserReadFrom(state);

	// Uninitialized
	std::raise(std::SIGSEGV);

	// Throw error
	throw 0;
}

bool BBP::userspace::xvalue::canUserWriteTo(userspace::StateMachine& state)
{
	if (isLvalue)
		return lval.canUserWriteTo(state);
	else if (isRvalue)
		return rval.canUserWriteTo(state);

	// Uninitialized
	std::raise(std::SIGSEGV);

	// Throw error
	throw 0;
}
