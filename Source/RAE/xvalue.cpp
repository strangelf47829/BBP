#include "../include/ValueCategories.h"
#include "../include/stdio.h"
#include "../include/Opcodes.h"
#include "../include/Threading.h"
#include "../include/StateMachine.h"


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
	__SIGNAL__(SIGSEGV);

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
	__SIGNAL__(SIGSEGV);
}

void BBP::userspace::xvalue::reference(userspace::StateMachine &state, pvalue &value)
{
	if (isLvalue)
		lval.reference(state, value);
	else if (isRvalue)
		rval.reference(state, value);

	// Uninitialized
	__SIGNAL__(SIGSEGV);
}

BBP::std::address_t BBP::userspace::xvalue::getOwnPhysicalAddress()
{
	if (isLvalue)
		return lval.getOwnPhysicalAddress();
	if (isRvalue)
		return rval.getOwnVirtualAddress();

	// Uninitialized value
	__SIGNAL__(SIGSEGV);
}

BBP::std::address_t BBP::userspace::xvalue::getOwnVirtualAddress()
{
	if (isLvalue)
		return lval.getOwnVirtualAddress();
	if (isRvalue)
		return rval.getOwnVirtualAddress();

	// Uninitialized value
	__SIGNAL__(SIGSEGV);
}

BBP::std::word BBP::userspace::xvalue::resolve(userspace::StateMachine &state)
{
	if (isLvalue)
		return lval.resolve(state);
	if (isRvalue)
		return rval.resolve(state);

	// Uninitialized value
	__SIGNAL__(SIGSEGV);
}

void BBP::userspace::xvalue::assign(userspace::StateMachine &state, pvalue &assignee, std::byte bytes)
{
	if (isLvalue)
		lval.assign(state, assignee, bytes);
	else if (isRvalue)
		rval.assign(state, assignee, bytes);

	// Uninitialized
	__SIGNAL__(SIGSEGV);
}