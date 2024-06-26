#include "../include/Graphics.h"

void BBP::std::R2D::PrintCharacterAt(window &window, pos_t x, pos_t y, ustring_element ch)
{
	// print with default stuff
	PrintCharacterAt(window, x, y, ch, window.fontSize, window.Chroma, colour(0, 0, 0));
}


void BBP::std::R2D::PrintCharacterAt(window &w, pos_t x, pos_t y, ustring_element ch, word size)
{
	PrintCharacterAt(w, x, y, ch, size, w.Chroma, colour(0, 0, 0));
}


void BBP::std::R2D::PrintCharacterAt(window &window, pos_t x, pos_t y, ustring_element ch, colour fg, colour bg)
{
	// print with default stuff
	PrintCharacterAt(window, x, y, ch, window.fontSize, fg, bg);
}


void BBP::std::R2D::PrintCharacterAt(window &w, pos_t x, pos_t y, ustring_element ch, word size, colour fg, colour bg)
{
	// Check if window has font selected
	if (w.activeFont.glyphs.data == nullptr)
		return;

	// Check if window has correct mapping
	if (w.activeFont.mapping.data == nullptr)
		return;

	// Check if x or y is outside bounds
	if (x >= w.width || y >= w.height)
		return;

	// Get mapping
	std::index_t mapping = w.activeFont.mapping[ch];

	// Get word count
	std::size_t wordCount = 0;

	// Select based on header type
	if (w.activeFont.version == Font::PSF1)
		wordCount = w.activeFont.headers.headerPSF1.glyphSize / 4;

	// Get offset
	std::offset_t fontOffset = mapping;

	// Get first element
	std::mask_t *mask = &w.activeFont.glyphs[fontOffset];

	// Character
	BBP::std::R2D::RectPixelMask(w, x, y, size, mask, wordCount, fg, bg);
}