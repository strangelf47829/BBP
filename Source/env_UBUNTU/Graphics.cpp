#include "../include/Graphics.h"


bool BBP::std::gINIT = false;

int BBP::std::Driver::initializeGraphics(std::window *window, int w, int h)
{
	window = window;
	w = w;
	h = h;
	gINIT = true;
	return 0;
}

void BBP::std::Driver::drawWindow(BBP::std::window *wind)
{
	wind = wind;
}

void BBP::std::Driver::destructGraphics()
{
	gINIT = false;
}