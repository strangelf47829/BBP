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

#include "kernel.h"
#include "SDL.h"

#include <cstdio>
#include <stdio.h>

namespace BBP::Graphics::Driver
{
	static SDL_Window* _window;
	static SDL_Renderer* renderer;
}
int r = 0;
int g = 0;
int b = 0;


int BBP::Graphics::Driver::initializeGraphics(BBP::Graphics::window *wind, int width, int height)
{
	_window = NULL;
	SDL_Surface* screenSurface = NULL;

	if(SDL_Init(SDL_INIT_VIDEO) < 0) 
	{
		printf("could not initialize SDL2: %s\n", SDL_GetError());
		return 1;
	}
	SDL_CreateWindowAndRenderer(width, height, 0, &_window, &renderer);

	/*window = SDL_CreateWindow(
			"hello_sdl2",
			SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
			width, height,
			SDL_WINDOW_SHOWN
			);*/
	if(_window == NULL)
	{
		printf("could not create window: %s\n", SDL_GetError());
		return 2;
	}
    screenSurface = SDL_GetWindowSurface(_window);
	SDL_FillRect(screenSurface, NULL, SDL_MapRGB(screenSurface->format, 0x00, 0x00, 0x00));
	SDL_UpdateWindowSurface(_window);


	wind->backBuffer = nullptr;
	R2D::newWindow(wind, width, height, 0,0,0);

	return 0;

	
}

void BBP::Graphics::Driver::drawWindow(BBP::Graphics::window *wind)
{
	SDL_SetRenderDrawColor(renderer, 255,255,255,255);
	SDL_RenderClear(renderer);
	int last = 0;
	for(int X = 0; X < wind->width; X++)
	{
		for(int Y = 0; Y < wind->height; Y++)
		{
			if(last - wind->backBuffer[X+Y*wind->width])
			{
				last = wind->backBuffer[X+Y*wind->width];
				int r = (last & (0xFF000000)) >> 24;
				int g = (last & (0x00FF0000)) >> 16;
				int b = (last & (0x0000FF00)) >> 8;
				int a = (last & (0x000000FF));
				SDL_SetRenderDrawColor(renderer, r,g,b,a);
			}
			SDL_RenderDrawPoint(renderer, X+wind->xPos, Y+wind->yPos);
		}
	}
	
    SDL_RenderPresent(renderer);
}

void BBP::Graphics::Driver::destructGraphics()
{
	SDL_DestroyWindow(_window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();
}

unsigned int BBP::Services::millis()
{
	return SDL_GetTicks();
}


/*
void GraphicsDriver::initialize_window(int width, int height)
{
    printf("Initializing graphics (SDL2 implementation). Resolution: %d x %d\n", width, height);

    window = NULL;
	SDL_Surface* screenSurface = NULL;

	if(SDL_Init(SDL_INIT_VIDEO) < 0) 
	{
		printf("could not initialize SDL2: %s\n", SDL_GetError());
		return;
	}
	SDL_CreateWindowAndRenderer(width, height, 0, &window, &renderer);

	/*window = SDL_CreateWindow(
			"hello_sdl2",
			SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
			width, height,
			SDL_WINDOW_SHOWN
			);
	if(window == NULL)
	{
		printf("could not create window: %s\n", SDL_GetError());
		return;
	}
    screenSurface = SDL_GetWindowSurface(window);
	SDL_FillRect(screenSurface, NULL, SDL_MapRGB(screenSurface->format, 0x00, 0x00, 0x00));
	SDL_UpdateWindowSurface(window);
    printf("Graphics initialized!");

}
void GraphicsDriver::deconstruct_window()
{
    printf("Destroying window\n");
    SDL_Delay(5000);
    printf("Window destroyed\n");
	SDL_DestroyWindow(window);
	SDL_Quit();
}

void GraphicsDriver::SetStroke(int white)
{
	r = white;
	g = white;
	b = white;
	SDL_SetRenderDrawColor(renderer, r, g, b, 255);
}
void GraphicsDriver::SetStroke(int R, int G, int B)
{
	r = R;
	g = G;
	b = B;
	SDL_SetRenderDrawColor(renderer, r, g, b, 255);
}

void GraphicsDriver::Update()
{
	SDL_RenderPresent(renderer);
}

void GraphicsDriver::drawRect(int x, int y, int w, int h)
{
	SDL_Rect toDraw = {x, y, w, h};
	SDL_RenderDrawRect(renderer, &toDraw);
}

void GraphicsDriver::drawPixel(int x, int y)
{
	SDL_RenderDrawPoint(renderer, x, y);
}

void GraphicsDriver::drawPixel(int x, int y, int size)
{
	for(int j = 0; j < size; j++)
		for(int i = 0; i < size; i++)
			SDL_RenderDrawPoint(renderer, x * size + i, y * size + j);
}

void GraphicsDriver::drawPixelAt(int x, int y, int size)
{
	for(int j = 0; j < size; j++)
		for(int i = 0; i < size; i++)
			SDL_RenderDrawPoint(renderer, x + i, y + j);
}

void GraphicsDriver::drawMask(int x, int y, int size, int mask)
{
	for(int i = 0; i < 8; i++)
	{
		if((mask >> i) & 1)
			drawPixel(x + i, y, size);
	}
}

void GraphicsDriver::drawMask(int x, int y, int size, int *mask_v, int mask_c, int dx, int dy)
{
	for(int i = 0; i < mask_c; i++)
	{
		drawMask(x + dx * i, y + dy * i, size, mask_v[i]);
	}
}*/