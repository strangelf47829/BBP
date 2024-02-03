#include "../include/Graphics.h"
#include "../../build/include/SDL.h"

static SDL_Window *_window;
static SDL_Renderer *renderer;

int r = 0;
int g = 0;
int b = 0;

bool BBP::std::gINIT = false;

int BBP::std::Driver::initializeGraphics(std::window *window, int w, int h)
{
	_window = NULL;
	SDL_Surface *screenSurface = NULL;

	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("could not initialize SDL2: %s\n", SDL_GetError());
		return 1;
	}
	SDL_CreateWindowAndRenderer(w, h, 0, &_window, &renderer);

	/*window = SDL_CreateWindow(
			"hello_sdl2",
			SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
			width, height,
			SDL_WINDOW_SHOWN
			);*/
	if (_window == NULL)
	{
		printf("could not create window: %s\n", SDL_GetError());
		return 2;
	}
	screenSurface = SDL_GetWindowSurface(_window);
	SDL_FillRect(screenSurface, NULL, SDL_MapRGB(screenSurface->format, 0x00, 0x00, 0x00));
	SDL_UpdateWindowSurface(_window);


	window->backBuffer.data = nullptr;
	R2D::newWindow(window, w, h, 0, 0, 0);

	gINIT = true;

	return 0;
}

void BBP::std::Driver::drawWindow(BBP::std::window *wind)
{
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderClear(renderer);
	int last = 0;
	for (int X = 0; X < wind->width; X++)
	{
		for (int Y = 0; Y < wind->height; Y++)
		{
			if (last - wind->backBuffer.data[X + Y * wind->width])
			{
				last = wind->backBuffer.data[X + Y * wind->width];
				int r = (last & (0xFF000000)) >> 24;
				int g = (last & (0x00FF0000)) >> 16;
				int b = (last & (0x0000FF00)) >> 8;
				int a = (last & (0x000000FF));
				SDL_SetRenderDrawColor(renderer, r, g, b, a);
			}
			SDL_RenderDrawPoint(renderer, X + wind->xPos, Y + wind->yPos);
		}
	}

	SDL_RenderPresent(renderer);
}

void BBP::std::Driver::destructGraphics()
{
	SDL_DestroyWindow(_window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();

	gINIT = false;
}