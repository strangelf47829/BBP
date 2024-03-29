
#include "../include/StateMachine.h"
#include "../include/ValueCategories.h"
#include "../include/stdio.h"
#include "../include/Opcodes.h"
#include "../include/Threading.h"

BBP::userspace::rvalue::rvalue()
{

}

BBP::userspace::rvalue::rvalue(std::word wrd)
{
	data = wrd;
	isLiteral = true;
	isReferenceToAddress = false;
}

BBP::userspace::rvalue::rvalue(userspace::StateMachine &state, userspace::Instruction::Arg &arg)
{
	// If the argument is an lvalue, data is just whatever that is but then resolved
	if (isArgumentLValue(arg))
	{
		lvalue val = lvalue(state, arg);
		data = val.resolve(state);
		isLiteral = true;
		isReferenceToAddress = false;
		return;
	}

	// Otherwise, if argument is not rvalue, throw signal
	if (isArgumentRValue(arg) == false)
		std::raise(std::SIGSEGV);

	// If argument is literal, just get data
	if (arg.isLiteralArg)
	{
		data = arg.data;
		isLiteral = true;
		isReferenceToAddress = false;
		return;
	}

	// Argument is reference to something. 
	isLiteral = false;
	isReferenceToAddress = true;

	// Get data and set that to referenced
	referencedAddress = arg.data;
	physicalAddress = state.getActiveThread().executable.virtualToPhysical(referencedAddress);
}

void BBP::userspace::rvalue::dereference(userspace::StateMachine &state, std::address_t lvalueFor)
{
	// Cannot dereference an rvalue
	std::raise(std::SIGSEGV);
}

void BBP::userspace::rvalue::reference(userspace::StateMachine &state, pvalue &value)
{
	// Get virtual address of value
	referencedAddress = value.getOwnVirtualAddress();
	physicalAddress = state.getActiveThread().executable.virtualToPhysical(referencedAddress);

	// Then set flags
	isLiteral = false;
	isReferenceToAddress = true;
}

BBP::std::address_t BBP::userspace::rvalue::getOwnPhysicalAddress()
{
	// If literal, throw SIGSEGV
	if (isLiteral)
		std::raise(std::SIGSEGV);

	return physicalAddress;
}

BBP::std::address_t BBP::userspace::rvalue::getOwnVirtualAddress()
{
	// If literal, throw SIGSEGV
	if (isLiteral)
		std::raise(std::SIGSEGV);

	return referencedAddress;
}

BBP::std::word BBP::userspace::rvalue::resolve(userspace::StateMachine &state)
{
	// If literal data
	if (isLiteral)
		return data;
	return referencedAddress;
}

void BBP::userspace::rvalue::assign(userspace::StateMachine &state, pvalue &assignee, std::byte bytes)
{
	// Cannot assign to rvalue
	std::raise(std::SIGSEGV);
}

void BBP::userspace::rvalue::assign(userspace::StateMachine &state, std::word data, std::byte bytes)
{
	// Cannot assign to rvalue
	std::raise(std::SIGSEGV);
}

bool BBP::userspace::rvalue::canUserExecuteFrom(userspace::StateMachine& state)
{
	// rvalues can never be executed
	return false;
}

bool BBP::userspace::rvalue::canUserReadFrom(userspace::StateMachine& state)
{
	// Rvalues can always be read from
	return true;
}

bool BBP::userspace::rvalue::canUserWriteTo(userspace::StateMachine& state)
{
	// rvalues cannot be written to
	return false;
}