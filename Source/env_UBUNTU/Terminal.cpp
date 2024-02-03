#include "../include/ECMA35.h"

BBP::std::Terminal &BBP::std::Terminal::Bell()
{
	if (usesGUI)
		throw std::exception("GUI Not supported", EPERM);
	out <<= '\x01';
	return *this;
}

