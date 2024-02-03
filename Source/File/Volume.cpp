#include "../include/FileSys.h"
#include "../include/stdio.h"

BBP::std::VOLUME::VOLUME(char l, std::conststring path)
	: volumePath(this, path), label(l)
{


}

BBP::std::VOLUME::~VOLUME()
{
	
}