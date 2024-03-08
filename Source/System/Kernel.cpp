#include "../include/Kernel.h"

BBP::system::KernelSubSystems s;

BBP::system::SystemContext::SystemContext()
	: STDOUT(&str_STDOUT, str_STDOUT.dataSize),
	STDIN(&str_STDIN, str_STDIN.dataSize),
	STDERR(&str_STDERR, str_STDERR.dataSize)

{

}

BBP::system::KernelSubSystems::KernelSubSystems()
	: activeContext(&kernelContext)
{
	activeContext = &kernelContext;
}

BBP::system::SystemContext* BBP::system::KernelSubSystems::getKernelSystemContext()
{
	return &kernelContext;
}

BBP::system::KernelSubSystems *BBP::system::kernelSS()
{
	// Check just bc stupid
	if (s.activeContext == nullptr)
		s.activeContext = s.getKernelSystemContext();

	return &s;
}