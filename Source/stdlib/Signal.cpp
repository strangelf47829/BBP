#include "../include/Signal.h"
#include "../include/Except.h"

void BBP::std::raise(std::signal_t signal)
{
	BBP::std::__errno = signal + BBP::std::SIGADJ; 
	throw BBP::std::SIGNAL(signal);
}