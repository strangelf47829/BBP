#include "../include/Graphics.h"

void BBP::std::R2D::print(window &w, ustring_element ch, colour col)
{
	// print a single character and advance forward
	PrintCharacterAt(w, w.cursorPosX, w.cursorPosY, ch, col);

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

void BBP::std::R2D::print(window &w, string str, colour col)
{
	// Iterate over string
	for (std::index_t idx = 0; idx < str.dataSize; idx++)
		print(w, str[idx], col);
}


void BBP::std::R2D::print(window &w, conststring str, colour col)
{
	// Create string, then print that
	print(w, std::String(str), col);
}

void BBP::std::R2D::print(window &w, string str)
{
	print(w, str, w.Chroma);
}
void BBP::std::R2D::print(window &w, conststring str)
{
	print(w, str, w.Chroma);
}