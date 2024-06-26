#include "../include/ScreenData.h"

// Draw a window
void Host::drawWindow(BBP::std::window *wind, SDL_Renderer *_Renderer, SDL_Surface &surface)
{
	// Lock surface
	SDL_LockSurface(&surface);

	// Then get pointer to surface
	BBP::std::pos_t *pixels = (BBP::std::pos_t *)surface.pixels;

	SDL_SetRenderDrawColor(_Renderer, 255, 255, 255, 255);
	SDL_RenderClear(_Renderer);
	int last = 0;
	for (int X = 0; X < wind->width; X++)
	{
		for (int Y = 0; Y < wind->height; Y++)
		{
			if (last - wind->backBuffer.data[X + Y * wind->width])
			{
				last = wind->backBuffer.data[X + Y * wind->width];
				int a = (last & (0xFF000000)) >> 24;
				int r = (last & (0x00FF0000)) >> 16;
				int g = (last & (0x0000FF00)) >> 8;
				int b = (last & (0x000000FF));
				SDL_SetRenderDrawColor(_Renderer, r, g, b, a);
			}
			SDL_RenderDrawPoint(_Renderer, X + wind->xPos, Y + wind->yPos);
		}
	}

	// Unlock pixels
	SDL_UnlockSurface(&surface);

	SDL_RenderPresent(_Renderer);
}