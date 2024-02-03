#include "../include/StateMachine.h"

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
	// Try to get hypervisor to do thing
	for (std::index_t i = 0; i < 7; i++)
	{
		// Get Instruction
		activeHypervisor->FetchActiveThreadInstruction();
		activeThread->LogInstruction();

		// Now load TLS
		activeThread->executable.loadTLS(activeThread->TLS);

		// Do instruction
		execute();

		// Unload TLS
		activeThread->executable.unloadTLS(activeThread->TLS);

	}
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
			__SIGNAL__(SIGILL);
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