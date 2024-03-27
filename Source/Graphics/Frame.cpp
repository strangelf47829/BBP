#include "../include/Graphics.h"

// Draws a frame at a certain position and width

void BBP::std::R2D::Frame(window &window, pos_t x, pos_t y, pos_t w, pos_t h)
{
	// If no stroke is selected, do nothing
	if (window.doStroke == false)
		return;

	// Default is stroke
	Frame(window, x, y, w, h, window.Stroke);
}

void BBP::std::R2D::Frame(window &window, pos_t x, pos_t y, pos_t w, pos_t h, colour col)
{
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

	// Calculate if x or y should be drawn.
	bool drawY = (window.strokeWeight * 2 < h);
	bool drawX = (window.strokeWeight * 2 < w);

	// If neither should be, draw box instead
	if ((drawY || drawX) == false)
	{
		Box(window, x, y, w, h);
		return;
	}

	// Draw horizontal stuff based on stroke weight.
	for (std::index_t wCount = 0; wCount < window.strokeWeight; wCount++)
	{

		// Draw hrule if x should be drawn
		if (drawX)
		{
			Hrule(window, x, x + w, y + wCount, col.RGBA);
			Hrule(window, x, x + w, y + h - wCount, col.RGBA);
		}

		// Draw vrule if y should be draw
		if (drawY)
		{
			Vrule(window, y + window.strokeWeight, y + h - window.strokeWeight + 1, x + wCount, col.RGBA);
			Vrule(window, y + window.strokeWeight, y + h - window.strokeWeight + 1, x + w - wCount - 1, col.RGBA);
		}
	}




	/*
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
	*/


}