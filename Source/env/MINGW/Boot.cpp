#include "../../include/Graphics.h"
#include "include/SDL2/SDL.h"
#include <cstdio>

constexpr BBP::std::pos_t w = 800;
constexpr BBP::std::pos_t h = 600;

BBP::std::STATIC_PAGE<BBP::std::RGBA_t, w *h> vmem;
BBP::std::window window;

static SDL_Window *_window;
static SDL_Renderer *_renderer;
static SDL_Surface *screenSurface;

int init();
void fini();

void drawWindow(BBP::std::window *wind)
{
	SDL_SetRenderDrawColor(_renderer, 255, 255, 255, 255);
	SDL_RenderClear(_renderer);
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
				SDL_SetRenderDrawColor(_renderer, r, g, b, a);
			}
			SDL_RenderDrawPoint(_renderer, X + wind->xPos, Y + wind->yPos);
		}
	}

	SDL_RenderPresent(_renderer);
}

int main(int, char **)
{
	// Initialize
	init();

	// Set window stuff
	window.backBuffer = BBP::std::PAGE<BBP::std::RGBA_t>(vmem.dataSize, vmem.static_data);
	window.width = w;
	window.height = h;

	BBP::std::R2D::stroke(window, 255, 255, 255);
	BBP::std::R2D::Line(window, 0, 0, 500, 500);

	drawWindow(&window);

	// Delay
	SDL_Delay(5000);

	// Then finish
	fini();
}

int init()
{

	_window = NULL;
	screenSurface = NULL;

	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("could not initialize SDL2: %s\n", SDL_GetError());
		return 1;
	}
	SDL_CreateWindowAndRenderer(w, h, 0, &_window, &_renderer);

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
}

void fini()
{
	SDL_DestroyWindow(_window);
	SDL_DestroyRenderer(_renderer);
	SDL_Quit();
}