#include "../include/Graphics.h"

// Draws a box at a certain position and width

void BBP::std::R2D::Box(window &window, pos_t x, pos_t y, pos_t w, pos_t h)
{
	// If no fill is selected, do nothing
	if (window.doFill == false)
		return;

	// Draw box with fill colour
	Box(window, x, y, w, h, window.Fill);
}

void BBP::std::R2D::Box(window &window, pos_t x, pos_t y, pos_t w, pos_t h, colour col)
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

	// Calculate bounds
	size_t xCount = (x + w) >= window.width ? (window.width - x) : (w);
	size_t yCount = (y + h) >= window.height ? (window.height - y) : (h);

	// Draw horizontal stuff based on xCount.
	for (std::index_t wCount = 0; wCount <= yCount; wCount++)
		Hrule(window, x, x + xCount, y + wCount, col.RGBA);

}