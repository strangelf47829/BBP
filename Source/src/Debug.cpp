/*

Copyright 2023 PCiD

This file is part of BBP.

BBP is free software: you can redistribute it and/or modify it under the terms of the GNU Lesser General
Public License as published by the Free Software Foundation, either version 3 of the License, or (at your
option) any later version.

BBP is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even
the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License along with BBP. If not, see
<https://www.gnu.org/licenses/>.

Author: Rafael de Bie

*/

#include "../include/Kernel.h"

#ifdef BBP_DEBUG

void BBP::Debug::printToConsole(char a)
{
	if (a != '\n')
		printf("%c", a);
	else
	{
		printf("\n");
	}
}

void BBP::Debug::SetTerminalColor(int colour)
{
	char buffer[11];
	sprintf(buffer, "\033[1;%dm", colour);
	printf(buffer);
}

int BBP::Debug::ConvertColorToOffset(int colour)
{
	switch (colour)
	{
	case 0x000000FF: // Black
		return 0;
	case 0xFFFFFFFF: // White
		return 7;
	case 0xFF0000FF: // Red
		return 1;
	case 0x00FF00FF: // Green
		return 2;
	case 0x0000FFFF: // Blue
		return 4;
	case 0xFFFF00FF: // Yellow
		return 3;
	case 0xFF00FFFF: // Magenta
		return 5;
	case 0x00FFFFFF: // Cyan
		return 6;
	default:
		printf("UNKOWN COLOR 0x%08x\n", colour);
		return 9;
	}
}

int fg = 39;
int bg = 49;

int fgACC = 39;
int bgACC = 49;

void BBP::Debug::setTerminalForegroundColor(int colour)
{
	int setTo = BBP::Debug::ConvertColorToOffset(colour) + 30;
	fg = setTo;
	BBP::Debug::SetTerminalColor(setTo);
}

void BBP::Debug::setTerminalBackgroundColor(int colour)
{
	int setTo = BBP::Debug::ConvertColorToOffset(colour) + 40;
	bg = setTo;
	BBP::Debug::SetTerminalColor(setTo);
}

void BBP::Debug::Capture()
{
	fgACC = fg;
	bgACC = bg;
}

void BBP::Debug::Restore()
{
	fg = fgACC;
	bg = bgACC;

	BBP::Debug::SetTerminalColor(fg);
	BBP::Debug::SetTerminalColor(bg);
}

#endif