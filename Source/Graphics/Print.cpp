#include "../include/Graphics.h"

void BBP::std::R2D::print(window &w, ustring_element ch, colour fg, colour bg)
{
	// print a single character and advance forward
	PrintCharacterAt(w, w.cursorPosX, w.cursorPosY, ch, fg, bg);

	// Move forward one character
	w.cursorPosX += w.fontSpacing * w.fontSize;

	// If cursor moved past width, reset x position and increment Y
	if (w.cursorPosX + w.fontSpacing * w.fontSize >= w.width)
	{
		// Reset cursor Pos X to coloumn
		w.cursorPosX = w.coloumn;

		// Reset y cursor
		w.cursorPosY += w.fontSpacing * w.fontSize * 2;
	}

	// If y cursor overflowed, reset position entirely
	if (w.cursorPosY + w.fontSpacing * w.fontSize * 2 >= w.height)
	{
		w.cursorPosX = w.coloumn;
		w.cursorPosY = w.line;
	}

}

void BBP::std::R2D::print(window &w, string str, colour fg, colour bg)
{
	// Iterate over string
	for (std::index_t idx = 0; idx < str.dataSize; idx++)
		print(w, str[idx], fg, bg);
}


void BBP::std::R2D::print(window &w, conststring str, colour fg, colour bg)
{
	// Create string, then print that
	print(w, std::String(str), fg, bg);
}

void BBP::std::R2D::print(window &w, string str)
{
	print(w, str, w.Chroma, colour(0, 0, 0));
}
void BBP::std::R2D::print(window &w, conststring str)
{
	print(w, str, w.Chroma, colour(0, 0, 0));
}