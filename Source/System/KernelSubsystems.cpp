#include "../include/Kernel.h"

BBP::userspace::HyperVisor visors[2];

BBP::system::ProcessContext::ProcessContext(SystemContext &context, std::c_string wd, std::c_string ad)
	: workingVolume(context.primaryVolume),
	activeDirectory(context.primaryVolume, ad),
	workingDirectory(context.primaryVolume, wd),
	activeMemory(context.activemem)
{

}

BBP::system::ProcessContext::ProcessContext(SystemContext &context)
	: workingVolume(context.primaryVolume),
	activeDirectory(context.primaryVolume, "/"),
	workingDirectory(context.primaryVolume, "/"),
	activeMemory(context.activemem)
{

}

BBP::system::SystemContext::SystemContext()
	: STDOUT(&str_STDOUT, str_STDOUT.dataSize),
	STDIN(&str_STDIN, str_STDIN.dataSize),
	STDERR(&str_STDERR, str_STDERR.dataSize)
{

}

BBP::system::KernelSubSystems::KernelSubSystems()
	: activeContext(&kernelContext),
	hypervisors(2, visors)
{
	activeContext = &kernelContext;
}

BBP::system::SystemContext *BBP::system::KernelSubSystems::getKernelSystemContext()
{
	return &kernelContext;
}

BBP::system::KernelSubSystems *BBP::system::kernelSS()
{
	// Check just bc stupid
	if (getKernelInstance().SubSystems().activeContext == nullptr)
		getKernelInstance().setKernelContext();

	return &getKernelInstance().SubSystems();
}