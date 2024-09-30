#include "../include/ScreenData.h"

BBP::std::STATIC_PAGE<BBP::std::RGBA_t, Host::w *Host::h> vmem;
BBP::std::window Host::hostDisplay;

static SDL_Window *_window;
static SDL_Renderer *_renderer;
static SDL_Surface *screenSurface;

// VBuffer
BBP::std::RGBA_t *Host::VBuff = 0;

// Page for BIOS font
BBP::std::STATIC_PAGE<BBP::std::string_element, 6000> biosFont;

void Host::startDisplay(Host::BIOS &bios)
{
	_window = NULL;
	screenSurface = NULL;

	// Set backbuffer
	Host::hostDisplay.backBuffer = BBP::std::PAGE<BBP::std::RGBA_t>(vmem.dataSize, vmem.static_data);
	Host::hostDisplay.width = w;
	Host::hostDisplay.height = h;

	// Set VBuff
	VBuff = vmem.data;

	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		BBP::std::printf("could not initialize SDL2: %s\n", SDL_GetError());
		return;
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
		BBP::std::printf("could not create window: %s\n", SDL_GetError());
		return;
	}
	screenSurface = SDL_GetWindowSurface(_window);
	SDL_FillRect(screenSurface, NULL, SDL_MapRGB(screenSurface->format, 0x00, 0x00, 0x00));
	SDL_UpdateWindowSurface(_window);

	// Open font
	bios.fopen((BBP::std::c_string)"V:\\usr\\share\\zap-vga16.psf");

	// Copy into bios font
	BBP::std::index_t fontIdx = 0;
	while (bios.fread(biosFont[fontIdx++]) == ENONE);

	// Open font
	BBP::std::loadFontPSF1(hostDisplay.activeFont, biosFont);

	// Set window
	Host::gterm.window = &Host::hostDisplay;

	// Set window draw method
	Host::gterm.drawFunc = drawDisplay;

	// Set background to 0 and foreground to 255
	BBP::std::R2D::background(Host::hostDisplay, 0, 125);

	// Set colours
	BBP::std::R2D::stroke(Host::hostDisplay, 0xFF0000);
	BBP::std::R2D::fill(Host::hostDisplay, 0x00FF00);
	Host::hostDisplay.Chroma.RGBA = 0xFFFFFFFF;

	// Set render thingy to 1
	SDL_RenderSetScale(_renderer, 1, 1);

	// Set other data
	Host::hostDisplay.line = 20;
	Host::hostDisplay.coloumn = 20;

	Host::hostDisplay.cursorPosX = 20;
	Host::hostDisplay.cursorPosY = 20;

	Host::hostDisplay.fontSize = 1;

	Host::gterm.windowX = 5;
	Host::gterm.windowY = 5;
}

void Host::closeDisplay()
{
	SDL_DestroyWindow(_window);
	SDL_DestroyRenderer(_renderer);
	SDL_Quit();
}

void Host::drawDisplay()
{
	drawWindow(&hostDisplay, _renderer, *screenSurface);
}