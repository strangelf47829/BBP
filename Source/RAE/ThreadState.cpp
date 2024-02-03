#include "../include/Threading.h"
#include "../include/Hyperv.h"

BBP::userspace::pid_t BBP::userspace::HyperVisor::allocateThread()
{
	return 0;
}

BBP::userspace::Thread *BBP::userspace::HyperVisor::spawnThread(std::ELF::ELFBuilder &binary, std::ResourceManager *allocator)
{
	// Get next available PID
	pid_t threadPID = allocateThread();

	// Get thread
	userspace::Thread *t = &threads.static_data[threadPID];

	// Set active
	t->active = true;
	activeThread = threadPID;

	// Set page
	t->executable.loadExecutable(binary, allocator);
	t->executable.createTLS(t->TLS);
	t->allocator = allocator;

	// Set eip
	userspace::setRegister(t->eip, binary.entry());

	// Return thread
	return t;
}

void BBP::userspace::HyperVisor::destroyThread(userspace::pid_t pid)
{
	// Get thread
	userspace::Thread *t = &threads.static_data[pid];

	// Unload executable
	t->executable.unloadExecutable();

	// Free up TLS
	t->allocator->free(t->TLS.data);
	t->TLS = std::PAGE<std::string_element>(0, nullptr);

	// Set inactive
	t->active = false;
}