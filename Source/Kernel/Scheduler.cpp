#include "../include/Kernel.h"

bool BBP::system::Kernel::kernelUpdateCycle(std::async_stack_t<> &stack, std::async_stack_t<std::TaskFlowInterface *> &arg)
{
	// Run singleton IRQ
	singleton.IRQ.updateHandler();

	// Set flag
	bool osStep = false;

	// If ext taskpool no longer exists, kill task
	if (singleton.extTaskPool == nullptr)
		arg.template get<0>()->Kill();

	// Otherwise, step extTaskPool. If none stepped, kill
	else
		osStep = singleton.extTaskPool->Step();

	// Kill
	if (osStep == false)
		arg.template get<0>()->Kill();

	// Return true
	return true;
}
