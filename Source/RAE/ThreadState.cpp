#include "../include/Threading.h"
#include "../include/Hyperv.h"
#include "../include/stdio.h"

BBP::userspace::pid_t BBP::userspace::HyperVisor::allocateThread()
{
	return currentPIDCount++;
}

BBP::userspace::Thread *BBP::userspace::HyperVisor::spawnThread(std::ELF::ELFBuilder &binary, std::ResourceManager *allocator)
{
	// Check if the ELF file is an executable
	if (binary.type() != std::ELF::ET_EXEC)
		std::exception("Cannot execute binary: not an executable.", ENOEXEC);

	// Get next available PID
	pid_t threadPID = allocateThread();

	// Get thread
	userspace::Thread *t = &threads.static_data[threadPID];
	t->myPid = threadPID;
	std::printf("Spawned thread %u.\n", t->myPid);

	// Set active
	t->active = true;
	activeThread = threadPID;

	// Set page
	t->executable.loadExecutable(binary, allocator);
	t->executable.createTLS(t->TLS);
	t->allocator = allocator;

	// Set EIP point
	userspace::setRegister(t->eip, binary.entry());

	// Expect endbr since entry point will have it
	t->expectsEndbr = true;

	// Reset argument and general stack
	t->argumentStack.atElement = 0;
	t->generalStack.atElement = 0;

	// Add dummy EIP, return object, and argument count.
	t->argumentStack << binary.entry() << binary.entry() << 0;

	// Return thread
	return t;
}

BBP::userspace::Thread::Thread()
	: eax(*this, 0, 0, thirtyTwoBit,false),
	ax(*this, 0, 0, sixteenBit,false),
	ah(*this, 0, 1, eightBit,false),
	al(*this, 0, 0, eightBit,false),

	ecx(*this, 1, 0, thirtyTwoBit, false),
	cx(*this, 1, 0, sixteenBit, false),
	ch(*this, 1, 1, eightBit, false),
	cl(*this, 1, 0, eightBit, false),

	edx(*this, 2, 0, thirtyTwoBit, false),
	dx(*this, 2, 0, sixteenBit, false),
	dh(*this, 2, 1, eightBit, false),
	dl(*this, 2, 0, eightBit, false),

	ebx(*this, 3, 0, thirtyTwoBit, false),
	bx(*this, 3, 0, sixteenBit, false),
		bh(*this, 3, 1, eightBit, false),
		bl(*this, 3, 0, eightBit, false),

		esi(*this, 4, 0, thirtyTwoBit, false),
		six(*this, 4, 0, sixteenBit, false),
		sih(*this, 4, 1, eightBit, false),
		sil(*this, 4, 0, eightBit, false),

		edi(*this, 5, 0, thirtyTwoBit, false),
		dix(*this, 5, 0, sixteenBit, false),
		dih(*this, 5, 1, eightBit, false),
		dil(*this, 5, 0, eightBit, false),

		tix(*this, 6, 0, thirtyTwoBit, true),
		st(*this, 6, 2, sixteenBit, true),
		ob(*this, 6, 2, eightBit, true),
		tb(*this, 6, 3, eightBit, true),
		ct(*this, 6, 0, sixteenBit, true),
		pp(*this, 6, 0, eightBit, true),
		tp(*this, 6, 1, eightBit, true),

		ssx(*this, 8, 0, thirtyTwoBit, true),
		hv(*this, 8, 2, sixteenBit, true),
		ho(*this, 8, 2, eightBit, true),
		ht(*this, 8, 3, eightBit, true),
		ss(*this, 8, 0, sixteenBit, true),
		cs(*this, 8, 0, eightBit, true),
		sf(*this, 8, 1, eightBit, true),

		sui(*this, 9, 0, thirtyTwoBit, true),
		asi(*this, 9, 0, sixteenBit, true),
		gsi(*this, 9, 2, sixteenBit, true),

		eip(*this, 10, 0, thirtyTwoBit, true),
		ip(*this, 10, 0, sixteenBit, true),
		ih(*this, 10, 1, eightBit, true),
		il(*this, 10, 0, eightBit, true),

		sip(*this, 11, 0, thirtyTwoBit, true),

		argumentStack(&argumentPage, argumentStackSize),
		generalStack(&generalPage, generalStackSize),

		routineAddress(&routineAddressPage, maxRoutineDepth)
{

}

void BBP::userspace::HyperVisor::destroyThread(userspace::pid_t pid)
{
	// Get thread
	userspace::Thread *t = &threads.static_data[pid];

	// Unload executable
	t->executable.unloadExecutable();

	// Free up TLS, if it exists
	if (t->TLS.data)
		t->allocator->free(t->TLS.data);

	t->TLS = std::PAGE<std::string_element>(0, nullptr);

	// Set inactive
	t->active = false;
}

bool BBP::userspace::Thread::isThreadCold()
{
	// Return true if the argument stack is at 0
	return argumentStack.atElement == 0;
}