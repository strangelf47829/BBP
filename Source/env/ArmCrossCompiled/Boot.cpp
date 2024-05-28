#include "include/Environment.h"

// Since everything is statically linked, no 'bootloading' is necessary.
int main()
{
	int result = BBP::system::Kernel::enterKernelSpace(Environment::UEFI::configureUEFI());
	return result;
}