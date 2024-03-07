#include "../include/StateMachine.h"
#include "../include/stdio.h"

void BBP::userspace::StateMachine::setActiveHypervisor(userspace::HyperVisor *hyperv)
{
	activeHypervisor = hyperv;
}

void BBP::userspace::StateMachine::setActiveThread(userspace::Thread *t)
{
	activeThread = t;
}

void BBP::userspace::StateMachine::cycleThread()
{
	// Get Instruction
	activeHypervisor->FetchActiveThreadInstruction();
	//activeThread->LogInstruction();

	// Now load TLS
	activeThread->executable.loadTLS(activeThread->TLS);

	// Do instruction
	execute();

	// Unload TLS
	activeThread->executable.unloadTLS(activeThread->TLS);

}
void BBP::userspace::StateMachine::execute()
{
	// If no thread active
	if (activeThread == nullptr)
		throw std::exception("No active thread.", ENOEXEC);

	// Cycle instruction
	switch (activeThread->instruction.header.opcode)
	{
		case userspace::NIL:
			NIL();
			break;
		case userspace::STACK:
			STACK();
			break;
		case userspace::CTRL:
			CTRL();
			break;
		case userspace::MEM:
			MEM();
			break;
		case userspace::INC:
			INC();
			break;
		case userspace::DEC:
			DEC();
			break;
		case userspace::NEG:
			NEG();
			break;
		case userspace::ADD:
			ADD();
			break;
		case userspace::SUB:
			SUB();
			break;
		case userspace::MUL:
			MUL();
			break;
		case userspace::DIV: 
			DIV();
			break;
		case userspace::MOD: 
			MOD();
			break;
		case userspace::LBS: 
			LBS();
			break;
		case userspace::RBS: 
			RBS();
			break;
		case userspace::OR:
			OR();
			break;
		case userspace::XOR:
			XOR();
			break;
		case userspace::AND:
			AND();
			break;
		case userspace::NOT: 
			NOT();
			break;
		case userspace::FLAG:
			FLAG();
			break;
		case userspace::JMP:
			JMP();
			break;
		case userspace::JMPZ:
			JMPZ();
			break;
		case userspace::JMPC:
			JMPC();
			break;
		case userspace::PAGE:
			PAGE();
			break;
		case userspace::SIGNAL:
			SIGNAL();
			break;
		case userspace::HALT:
			HALT();
			break;
		default:
			std::raise(std::SIGILL);
	}


}

BBP::userspace::Thread &BBP::userspace::StateMachine::getActiveThread()
{
	// If no thread active
	if (activeThread == nullptr)
		throw std::exception("No active thread.", ENOEXEC);
	
	// Return thread
	return *activeThread;
}


// Shorthand for calling functions without promise
void BBP::userspace::StateMachine::callFunction(std::word address, std::byte argCount, std::word args[7])
{
	// Create lvalue handle for eip
	Instruction::Arg eipArg = { userspace::REGISTERPAGE | 10, false, true, 0, 1, {1, 0, 0, 0}, false, false };
	lvalue eipHandle(*this, eipArg);

	// Now call function with EIP handle.
	callFunction(address, eipHandle, argCount, args);
}

// Calling a function from within
void BBP::userspace::StateMachine::callFunction(std::word address, lvalue &returnObject, std::byte argCount, std::word args[7])
{

	// Check if argcount is valid
	if (argCount >= 7)
		std::raise(std::SIGSEGV);

	// Get current EIP
	std::word jumpBackTo = readRegister(activeThread->eip);

	// Now store that in argument stack
	activeThread->argumentStack << jumpBackTo;

	// Now get lvalue virtual address, and store that into argument stack
	activeThread->argumentStack << returnObject.getOwnVirtualAddress();

	// Now add each argument into the stack
	for (std::byte arg = 0; arg < argCount; arg++)
		activeThread->argumentStack << args[arg];

	// Now add the amount of arguments added
	activeThread->argumentStack << argCount;

	// Now actually set eip to address
	setRegister(activeThread->eip, address);

}