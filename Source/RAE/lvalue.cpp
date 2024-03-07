#include "../include/StateMachine.h"
#include "../include/ValueCategories.h"
#include "../include/Opcodes.h"
#include "../include/Threading.h"



BBP::userspace::lvalue::lvalue()
{

}

BBP::userspace::lvalue::lvalue(userspace::StateMachine &state, std::word address)
{
	dereference(state, address);
}

BBP::userspace::lvalue::lvalue(userspace::StateMachine &state, userspace::Instruction::Arg &arg)
{

	// If argument is not an L value, signal error
	if (isArgumentLValue(arg) == false)
		std::raise(std::SIGILL);

	// Expect one trailer
	if (arg.trailerCount < 1)
		std::raise(std::SIGILL);

	// So it is referenced. Check where
	std::address_t lvalueFor = arg.data;
	std::word dereferenceCount = arg.trailers[0];

	// Dereference at least once
	dereference(state, lvalueFor);

	// For dereference count
	for (std::index_t dereferenceCounter = 1; dereferenceCounter < dereferenceCount; dereferenceCounter++)
	{
		// Resolve lvalueFor, then dereference that
		lvalueFor = resolve(state);
		dereference(state, lvalueFor);
	}

}

void BBP::userspace::lvalue::dereference(userspace::StateMachine &state, std::address_t lvalueFor)
{
	// Check if lvalue is register
	if (isAddressRegisterAddress(lvalueFor))
	{
		_register = getAddressForRegister(state, lvalueFor);

		// Set flags
		isRegister = true;
		isAddress = false;
		PhysicalAddress = 0;
		VirtualMemory = lvalueFor;
		
		return;

	}

	// Not a register. Set flags
	isRegister = false;
	isAddress = true;

	// Set vmem
	VirtualMemory = lvalueFor;
	PhysicalAddress = state.getActiveThread().executable.virtualToPhysical(VirtualMemory);

}

void BBP::userspace::lvalue::reference(userspace::StateMachine &state, pvalue &value)
{
	// Get virtual memory address of pvalue
	std::address_t valueAddress = value.getOwnVirtualAddress();

	// Check if is register
	isRegister = isAddressRegisterAddress(valueAddress);
	isAddress = !isRegister;


	// If not a register, set physical address too
	if (isAddress)
		PhysicalAddress = state.getActiveThread().executable.virtualToPhysical(valueAddress);
	else
		PhysicalAddress = 0;
}

BBP::std::address_t BBP::userspace::lvalue::getOwnPhysicalAddress()
{
	// Only return physical memory if not a register
	if (isAddress)
		return PhysicalAddress;

	// Cannot get physical address of register.
	std::raise(std::SIGSEGV);
}

BBP::std::address_t BBP::userspace::lvalue::getOwnVirtualAddress()
{
	// Just return
	return VirtualMemory;
}

BBP::std::word BBP::userspace::lvalue::resolve(userspace::StateMachine &state)
{
	// If register, read thread
	if (isRegister)
	{
		// If invalid register, throw sigsegv
		if (_register == nullptr)
			std::raise(std::SIGSEGV);

		// Return data
		return readRegister(*_register);
	}

	// If Address, return data read by thread
	if (isAddress)
		return std::read(&state.getActiveThread().executable.BinaryData, PhysicalAddress);

	// If not anything, SIGSEGV
	std::raise(std::SIGSEGV);
}

void BBP::userspace::lvalue::assign(userspace::StateMachine &state, pvalue &assignee, std::byte bytes)
{
	// Get data to write
	std::word data = assignee.resolve(state);

	// if register, write to register
	if (isRegister)
	{
		setRegister(*_register, data);
		return;
	}
	
	// Otherwise, if is address, write directly into physical address. But only the specified amount of bytes
	for (std::byte atByte = 0; atByte < bytes; atByte++)
	{
		std::string_element byteToWrite = data >> (atByte * 8);
		std::write(&state.getActiveThread().executable.BinaryData, byteToWrite, PhysicalAddress + atByte);
	}
}

void BBP::userspace::lvalue::assign(userspace::StateMachine &state, std::word data, std::byte bytes)
{
	rvalue val(data);
	assign(state, val, bytes);
}

bool BBP::userspace::lvalue::canUserExecuteFrom(userspace::StateMachine& state)
{
	// If value is register, return false
	if (isRegister)
		return false;

	// If not address, throw signal for uninitialized data
	if (!isAddress)
		std::raise(std::SIGSEGV);

	// Get page permissions
	std::index_t page = state.getActiveThread().executable.findIndexOfVirtualMemory(VirtualMemory);

	// Check for error
	if (page >= state.getActiveThread().executable.mapping.dataSize)
		std::raise(std::SIGSEGV);

	return state.getActiveThread().executable.mapping.static_data[page].executable;
}

bool BBP::userspace::lvalue::canUserReadFrom(userspace::StateMachine& state)
{
	// If value is register, return true
	if (isRegister)
		return true;

	// If not address, throw signal for uninitialized data
	if (!isAddress)
		std::raise(std::SIGSEGV);

	// Get page permissions
	std::index_t page = state.getActiveThread().executable.findIndexOfVirtualMemory(VirtualMemory);

	// Check for error
	if (page >= state.getActiveThread().executable.mapping.dataSize)
		std::raise(std::SIGSEGV);

	return state.getActiveThread().executable.mapping.static_data[page].readable;
}

bool BBP::userspace::lvalue::canUserWriteTo(userspace::StateMachine& state)
{
	// If value is register, return register readonly
	if (isRegister)
		return !_register->readonly;

	// If not address, throw signal for uninitialized data
	if (!isAddress)
		std::raise(std::SIGSEGV);

	// Get page permissions
	std::index_t page = state.getActiveThread().executable.findIndexOfVirtualMemory(VirtualMemory);

	// Check for error
	if (page >= state.getActiveThread().executable.mapping.dataSize)
		std::raise(std::SIGSEGV);

	return state.getActiveThread().executable.mapping.static_data[page].writeable;
}