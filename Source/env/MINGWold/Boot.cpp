#include "../../include/Graphics.h"
#include "../../include/Resources.h"
#include "include/GTerm.h"
#include "include/SDL2/SDL.h"
#include <cstdio>

#include <fstream>

constexpr BBP::std::pos_t w = 1200;
constexpr BBP::std::pos_t h = 800;

BBP::std::STATIC_PAGE<BBP::std::RGBA_t, w *h> vmem;
BBP::std::window window;
BBP::std::Font activeFont;
BBP::std::ResourceManager manager;

static SDL_Window *_window;
static SDL_Renderer *_renderer;
static SDL_Surface *screenSurface;

OS::Terminal::GTerm device;
BBP::std::Terminal::TerminalApplication terminal({30, 80}, 60, &manager, &device);

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
				int a = (last & (0xFF000000)) >> 24;
				int r = (last & (0x00FF0000)) >> 16;
				int g = (last & (0x0000FF00)) >> 8;
				int b = (last & (0x000000FF));
				SDL_SetRenderDrawColor(_renderer, r, g, b, a);
			}
			SDL_RenderDrawPoint(_renderer, X + wind->xPos, Y + wind->yPos);
		}
	}

	SDL_RenderPresent(_renderer);
}

void draw()
{
	drawWindow(&window);
}

bool loadFile(BBP::std::c_string _path, BBP::std::PAGE<BBP::std::string_element> &page)
{
	// Open fstream
	::std::ifstream _file(_path, ::std::ios::binary);

	// Check if file is open
	if (!_file.is_open())
		return false;

	// Get bytecount 
	_file.seekg(0, ::std::ios::end);
	BBP::std::size_t size = _file.tellg();
	_file.seekg(0, ::std::ios::beg);

	// If not enough room, exit
	if (page.dataSize < size)
		return false;

	// Read from file, and put into n.data
	char next;
	BBP::std::size_t idx = 0;
	while (_file.get(next))
		page.data[idx++] = next;

	// Close file
	_file.close();

	return true;
}

int main(int, char **)
{
	// Initialize
	init();

	// Set window stuff
	window.backBuffer = BBP::std::PAGE<BBP::std::RGBA_t>(vmem.dataSize, vmem.static_data);
	window.width = w;
	window.height = h;

	// Set background
	BBP::std::R2D::background(window, 0, 255);

	// Set colours
	BBP::std::R2D::stroke(window, 0xFF0000);
	BBP::std::R2D::fill(window, 0x00FF00);

	window.Chroma.RGBA = 0xFFFFFFFF;

	device.drawFunc = draw;

	// Draw frame
	//BBP::std::R2D::Rect(window, 20, 20, 50, 50);

	// Create page for font
	BBP::std::STATIC_PAGE<BBP::std::string_element, 6000> fontMem;
	BBP::std::PAGE<BBP::std::string_element> fontStr(fontMem.dataSize, fontMem.data);

	// Load font
	bool succ = loadFile("V:usr\\share\\zap-vga16.psf", fontMem);

	int top = 0x00003018;
	int bot = 0x0c0c0c0c;

	device.window = &window;

	// If could do so
	if (succ)
	{
		// Load font from memory
		BBP::std::loadFontPSF1(activeFont, manager, fontMem);
		BBP::std::R2D::setActiveFont(window, activeFont);

		window.line = 20;
		window.coloumn = 20;

		window.cursorPosX = 20;
		window.cursorPosY = 20;

		window.fontSize = 1;

		device.windowX = 50;
		device.windowY = 50;

		for (int i = 0; i < 40; i++)
		{
			char buff[500];
			sprintf(buff, "line %d  \t\033[0mHello!\n", i);

			BBP::std::string str = BBP::std::String(buff);

			terminal.render(str);

			SDL_Delay(100);
		}
		
		SDL_Delay(3000);

		BBP::std::string scroll = BBP::std::String("\033[S");

		int scrollAmount = 4;
		
		for (int i = 0; i < scrollAmount; i++)
		{
			terminal.render(scroll);

			SDL_Delay(1000);
		}
		// Delay
		SDL_Delay(5000);

	}

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