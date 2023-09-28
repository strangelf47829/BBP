
#include "../include/kernel.h"

#include <cstdio>
#include <stdio.h>

#include <chrono>

int r = 0;
int g = 0;
int b = 0;

time_t startupTime = 0;

time_t getNow()
{
	return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now().time_since_epoch()).count();
}

int BBP::Graphics::Driver::initializeGraphics(BBP::Graphics::window *wind, int width, int height)
{
	startupTime = getNow();

	wind->backBuffer = nullptr;
	R2D::newWindow(wind, width, height, 0,0,0);

	return 0;
}

void BBP::Graphics::Driver::drawWindow(BBP::Graphics::window *wind)
{
}

void BBP::Graphics::Driver::destructGraphics()
{

}

unsigned int BBP::Services::millis()
{
	return getNow() - startupTime;
}
