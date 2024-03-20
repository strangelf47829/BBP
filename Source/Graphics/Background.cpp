#include "../include/Graphics.h"

// Sets the background of a window

void BBP::std::R2D::background(window &w, chan_t r, chan_t g, chan_t b)
{
	// Convert RGBA to word
	RGBA_t colourWord = convertUCHARtoINT(r, g, b, 255);

	// Foreach value in page, set to that value
	for (std::index_t pix = 0; pix < w.backBuffer.dataSize; pix++)
		w.backBuffer[pix] = colourWord;
}

void BBP::std::R2D::background(window &w, chan_t r, chan_t g, chan_t b, chan_t a)
{
	// Convert RGBA to word
	RGBA_t colourWord = convertUCHARtoINT(r, g, b, a);

	// Foreach value in page, set to that value
	for (std::index_t pix = 0; pix < w.backBuffer.dataSize; pix++)
		w.backBuffer[pix] = colourWord;
}

void BBP::std::R2D::background(window &w, RGBA_t rgb)
{
	// Foreach value in page, set value to rgb
	for (std::index_t pix = 0; pix < w.backBuffer.dataSize; pix++)
		w.backBuffer[pix] = rgb;
}

void BBP::std::R2D::background(window &w, RGBA_t rgb, chan_t a)
{
	chan_t r = 0;
	chan_t g = 0;
	chan_t b = 0;

	// Get components
	convertINTtoUCHAR(r, g, b, rgb);

	// Mix alpha into rgba
	RGBA_t colourWord = convertUCHARtoINT(r, g, b, a);

	// Foreach value in page, set to that value
	for (std::index_t pix = 0; pix < w.backBuffer.dataSize; pix++)
		w.backBuffer[pix] = colourWord;
}