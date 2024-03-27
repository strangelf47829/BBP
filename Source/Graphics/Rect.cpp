#include "../include/Graphics.h"

void BBP::std::R2D::Rect(window &window, pos_t x, pos_t y, pos_t w, pos_t h)
{
	// Calculate inner box offset
	pos_t offset = window.doStroke ? (window.strokeWeight) : 0;

	// Draw box then frame
	Frame(window, x, y, w, h);

	// Only draw box if it would be visible
	if (offset * 2 < w && offset * 2 < h)
		Box(window, x + offset, y + offset, w - offset * 2, h - offset * 2);

}