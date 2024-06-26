#include "../include/Graphics.h"

bool BBP::std::window::setPixel(std::pos_t x, std::pos_t y, std::RGBA_t col)
{
	// Check if within bounds
	if (x >= width || y >= height)
		return false; // If not, do nothing

	// Otherwise calculate pixel offset
	std::offset_t pixelOffset = x + width * y;

	// Then set
	backBuffer[pixelOffset] = col;

	// Then return success
	return true;
}

BBP::std::RGBA_t BBP::std::window::getPixel(std::pos_t x, std::pos_t y)
{
	// Check if within bounds
	if (x >= width || y >= height)
		return 0; // If not, do nothing

	// Otherwise calculate pixel offset
	std::offset_t pixelOffset = x + width * y;

	// Then return
	return backBuffer[pixelOffset];
}