#include "../include/Graphics.h"
#include "../include/FileSys.h"

#include <cstdio>
#include <cstdlib>

void BBP::std::R2D::setActiveFont(window &w, Font &font)
{
	w.activeFont = font;
}

// Load a font from memory
void BBP::std::loadFontPSF1(Font & font, std::PAGE<std::string_element> &element)
{
	// Check if font has at least 4 bytes
	if (element.dataSize < 4)
		return;

	// Get first word
	std::word firstWord = ((std::word*)element.data)[0];

	// Get mask
	std::halfword magic = firstWord & 0xFFFF;
	std::byte mode = (firstWord >> 16) & 0xFF;
	std::byte size = (firstWord >> 24) & 0xFF;

	// Check if magic matches
	bool endianMatchesMachine = false;

	if (magic == 0x3604)
		endianMatchesMachine = true;
	else if (magic != 0x0436)
		return;

	// Set magic
	font.headers.headerPSF1.magic = magic;

	// Set mode and glyphsize
	font.headers.headerPSF1.mode = mode;
	font.headers.headerPSF1.glyphSize = size;

	// Set version
	font.version = BBP::std::Font::PSF1;

	// print out first 

	// Print each 256 characters
	for (std::index_t idx = 0; idx < 256; idx++)
	{
		// Get word 
		for (BBP::std::index_t _word = 0; _word < size; _word += 4)
		{
			// Get word for element
			std::halfword b1 = element[4 + (idx * 16) + 0 + _word] & 0xFF;
			std::halfword b2 = element[4 + (idx * 16) + 1 + _word] & 0xFF;
			std::halfword b3 = element[4 + (idx * 16) + 2 + _word] & 0xFF;
			std::halfword b4 = element[4 + (idx * 16) + 3 + _word] & 0xFF;

			std::word reordered = 0;
			
			// Reorder them based on endian-ness
			if (endianMatchesMachine)
				reordered = (b1 << 24) | (b2 << 16) | (b3 << 8) | b4;
			else
				reordered = (b4 << 24) | (b3 << 16) | (b2 << 8) | b1;

			// Set that as glyph
			font.glyphs[idx * 4 + ((_word / 4))] = reordered;
		}

		// Set mapping
		font.mapping[idx] = idx * (size / 4);

	}

}