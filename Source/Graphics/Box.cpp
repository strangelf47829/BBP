#include "../include/Graphics.h"

// Draws a box at a certain position and width

void BBP::std::R2D::Box(window &window, pos_t x, pos_t y, pos_t w, pos_t h)
{
	// If no stroke is selected, do nothing
	if (window.doStroke == false)
		return;

	// If 'x' is beyond window size, do nothing
	if (x >= window.width)
		return;

	// If 'y' is beyond window size, do nothing
	if (y >= window.height)
		return;

	// If 'w' is 0, do nothing
	if (w == 0)
		return;

	// If 'h' is 0, do nothing
	if (h == 0)
		return;

	// Calculate bounds
	size_t xCount = (x + w) >= window.width ? (window.width - x) : (w - x);
	size_t yCount = (y + h) >= window.height ? (window.height - y) : (h - y);

	// Go over each horizontal pixel
	for (pos_t xPixel = 0; xPixel < xCount; xPixel++)
	{
		// Set pixel to current stroke.
		setPixel(window, x + xPixel, y + 0, window.Stroke.RGBA);
		setPixel(window, x + xPixel, y + h, window.Stroke.RGBA);
	}

	// Go over each vertical pixel
	for (pos_t yPixel = 0; yPixel < yCount; yPixel++)
	{
		// Set pixel to current stroke.
		setPixel(window, x + 0, y + yPixel, window.Stroke.RGBA);
		setPixel(window, x + w, y + yPixel, window.Stroke.RGBA);
	}


}