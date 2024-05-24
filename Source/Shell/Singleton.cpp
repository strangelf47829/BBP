#include "../include/Shell.h"

BBP::system::Shell BBP::system::Shell::singleton;

BBP::system::Shell::Shell()
	: memoryStack(&memories)
{

}