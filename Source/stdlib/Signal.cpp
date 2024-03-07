#include "../include/Signal.h"
#include "../include/Except.h"
#include "../include/Kernel.h"

void BBP::std::raise(std::signal_t signal)
{
	BBP::system::kernelSS()->activeContext->__errno = signal + BBP::std::SIGADJ;
	throw BBP::std::SIGNAL(signal);
}