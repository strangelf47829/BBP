#include "../include/Kernel.h"

BBP::system::Kernel BBP::system::Kernel::singleton;

BBP::system::Kernel::Kernel()
	: taskpool(taskpoolData),
	systemDriver(nullptr, nullptr, "System", nullptr, 0, nullptr),
	fileDriver(nullptr, nullptr, "FileSystem", nullptr, 0, nullptr),
	screenDriver(nullptr, nullptr, "Screen", nullptr, 0, nullptr),
	keyboardDriver(nullptr, nullptr, "Keyboard", nullptr, 0, nullptr)
{
	fileDriver.Associate();
}