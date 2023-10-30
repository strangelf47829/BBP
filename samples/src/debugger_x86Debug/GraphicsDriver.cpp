/*

Copyright 2023 PCiD

This file is part of BBP.

BBP is free software: you can redistribute it and/or modify it under the terms of the GNU Lesser General
Public License as published by the Free Software Foundation, either version 3 of the License, or (at your
option) any later version.

BBP is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even
the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License along with BBP. If not, see
<https://www.gnu.org/licenses/>.

Author: Rafael de Bie

*/

#include "Kernel.h"

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
