#include "../include/FileSys.h"
#include "../include/stdio.h"

BBP::std::VOLUME::VOLUME(char l, std::conststring path)
	: label(l)
{
	

}

void BBP::std::VOLUME::mount(std::string_element label, std::conststring path)
{
	// Set label
	this->label = label;

	// Set path
	this->volumePath = std::PATH(path);

}

BBP::std::VOLUME::~VOLUME()
{
	
}