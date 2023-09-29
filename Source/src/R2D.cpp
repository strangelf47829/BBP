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

using namespace BBP::Graphics;


// COLOURS
#pragma region COLOURS

int R2D::convertUCHARtoINT(u_char r, u_char g, u_char b, u_char a)
{
    int ret = (r << 24) | (g << 16) | (b << 8) | a;
    return ret;
}
void R2D::convertINTtoUCHAR(u_char *r, u_char *g, u_char *b, int rgb)
{
    *r = (rgb >> 24) & 0xFF;
    *g = (rgb >> 16) & 0xFF;
    *b = (rgb >> 8) & 0xFF;
}
void R2D::convertINTtoUCHAR(u_char *r, u_char *g, u_char *b, u_char *a, int rgba)
{
    *r = (rgba >> 24) & 0xFF;
    *g = (rgba >> 16) & 0xFF;
    *b = (rgba >> 8) & 0xFF;
    *a = (rgba) & 0xFF;
}

#pragma region STROKE

void R2D::stroke(window *window, u_char r, u_char g, u_char b)
{
    window->Stroke.R = r;
    window->Stroke.G = g;
    window->Stroke.B = b;
    window->Stroke.RGBA = convertUCHARtoINT(r, g, b, window->Stroke.A);
}
void R2D::stroke(window *window, u_char r, u_char g, u_char b, u_char a)
{
    window->Stroke.R = r;
    window->Stroke.G = g;
    window->Stroke.B = b;
    window->Stroke.A = a;
    window->Stroke.RGBA = convertUCHARtoINT(r, g, b, a);
}
void R2D::stroke(window *window, int rgb)
{
    window->Stroke.RGBA = rgb;
    convertINTtoUCHAR(&(window->Stroke.R), &(window->Stroke.G), &(window->Stroke.B), rgb);
}
void R2D::stroke(window *window, int rgb, u_char a)
{
    convertINTtoUCHAR(&(window->Stroke.R), &(window->Stroke.G), &(window->Stroke.B), rgb);
    window->Stroke.A = a;
    window->Stroke.RGBA = convertUCHARtoINT(window->Stroke.R, window->Stroke.G, window->Stroke.B, a);
}

void R2D::noStroke(window *window)
{
    window->doStroke = false;
}

void R2D::strokeWeight(window *window, int weight)
{
    window->strokeWeight = weight;
}

#pragma endregion

#pragma region FILL

void R2D::fill(window *window, u_char r, u_char g, u_char b)
{
    window->Fill.R = r;
    window->Fill.G = g;
    window->Fill.B = b;
    window->Fill.RGBA = convertUCHARtoINT(r, g, b, window->Fill.A);
    window->doFill = true;
}
void R2D::fill(window *window, u_char r, u_char g, u_char b, u_char a)
{
    window->Fill.R = r;
    window->Fill.G = g;
    window->Fill.B = b;
    window->Fill.A = a;
    window->Fill.RGBA = convertUCHARtoINT(r, g, b, a);
    window->doFill = true;
}
void R2D::fill(window *window, int rgb)
{
    window->Fill.RGBA = rgb;
    convertINTtoUCHAR(&(window->Fill.R), &(window->Fill.G), &(window->Fill.B), rgb);
    window->doFill = true;
}
void R2D::fill(window *window, int rgb, u_char a)
{
    convertINTtoUCHAR(&(window->Fill.R), &(window->Fill.G), &(window->Fill.B), rgb);
    window->Fill.A = a;
    window->Fill.RGBA = convertUCHARtoINT(window->Fill.R, window->Fill.G, window->Fill.B, a);
    window->doFill = true;
}

void R2D::noFill(window *window)
{
    window->doFill = false;
}

#pragma endregion

#pragma endregion

// Window
#pragma region WINDOW

void R2D::newWindow(window *window, int width, int height, u_char r, u_char g, u_char b)
{
    if (window == nullptr)
        return;
    window->width = width;
    window->height = height;

    if (window->backBuffer != nullptr)
        BBP::Services::Free(window->backBuffer);

    int colour = convertUCHARtoINT(r, g, b, 255);

    window->backBuffer = (int *)BBP::Services::c_alloc(width * height, sizeof(int));
    R2D::background(window, r, g, b);
}

void R2D::newWindow(window *window, u_char r, u_char g, u_char b)
{
    if (window == nullptr)
        return;

    if (window->backBuffer != nullptr)
        BBP::Services::Free(window->backBuffer);

    int colour = convertUCHARtoINT(r, g, b, 0);

    window->backBuffer = (int *)BBP::Services::c_alloc(window->width * window->height, sizeof(int));
    background(window, r, g, b);
    /*for (int x = 0; x < window->width; x++)
        for (int y = 0; y < window->height; y++)
            window->backBuffer[x + y * window->width] = colour;*/
}

void R2D::GetFontFromROM(window *window, int offset_start, int nums)
{
    // Get Font from Memory. This returns a calloc-ed pointer to the specified chunk of memory.
    // Since this pointer is c-alloced, and the function doesn't touch it, it is safe to just store into the window font.
    int *font = BBP::IO::Memory::ROM::readROMIntChunk(offset_start, offset_start + nums * 2 - 1);
    mem_t *mapping = BBP::IO::Memory::ROM::readROMChunk((offset_start + nums * 2) * 4, ((offset_start + nums * 2) * 4) + nums * 8);

    // If window pointer is invalid, something went wrong. Throw BIOS error for now to test functionality
    if (window == nullptr)
        BBP::Services::Throw(KERNEL_BIOSERROR);

    // Since the window may have had a previous font placed in it (which was c-alloced), overriding it would cause memory leak.
    // This checks if the font pointer is a nullptr, and if not (meaning it holds a font), release the c-alloced data.
    if (window->typeFace != nullptr)
        BBP::Services::Free(window->typeFace);

    if (window->charMapping != nullptr)
        BBP::Services::Free(window->charMapping);

    // Now write the font mapping to the window font mapping
    // TODO inefficient, optimize later though

    // First find the \0 character from ROM
    int nullCharacterIndex = 0;
    for (int i = 0; i < nums; i++)
    {
        if (mapping[i]) // If the character is not '\0', continue
            continue;
        nullCharacterIndex = i; // \0 found!
        break;
    }
    // Then write that font map to each character
    for (int i = 0; i < 256; i++)
        window->fontMapping[i] = nullCharacterIndex;

    // Then overwrite the known fonts from the map
    for (int i = 0; i < nums; i++)
        window->fontMapping[mapping[i]] = i;

    // Now the window font can be overriden without causing memory leak.
    window->typeFace = font;
    window->charMapping = mapping;

}

#pragma endregion

// GRAPHICS
#pragma region GRAPHICS

#pragma region BACKGROUND
void R2D::background(window *window, u_char r, u_char g, u_char b)
{
    int colour = R2D::convertUCHARtoINT(r, g, b, 255);
    for (int n = 0; n < window->width * window->height; n++)
        window->backBuffer[n] = colour;
}
void R2D::background(window *window, u_char r, u_char g, u_char b, u_char a)
{
    int colour = R2D::convertUCHARtoINT(r, g, b, a);
    for (int n = 0; n < window->width * window->height; n++)
        window->backBuffer[n] = colour;
}
void R2D::background(window *window, int rgba)
{
    for (int n = 0; n < window->width * window->height; n++)
        window->backBuffer[n] = rgba;
}
void R2D::background(window *window, int rgb, u_char a)
{
    int colour = (rgb & ~0xFF) ^ a;
    for (int n = 0; n < window->width * window->height; n++)
        window->backBuffer[n] = colour;
}
#pragma endregion

#pragma region LINE

void R2D::Hrule(window *window, int x1, int x2, int y)
{
    int colour = R2D::convertUCHARtoINT(window->Stroke.R, window->Stroke.G, window->Stroke.B, window->Stroke.A);
    for (int i = x1; i < x2; i++)
        window->backBuffer[i + window->width * y] = colour;
}

void R2D::Vrule(window *window, int y1, int y2, int x)
{
    int colour = R2D::convertUCHARtoINT(window->Stroke.R, window->Stroke.G, window->Stroke.B, window->Stroke.A);
    for (int i = y1; i < y2; i++)
        window->backBuffer[x + window->width * i] = colour;
}

void R2D::Hrule(window *window, int x1, int x2, int y, u_char r, u_char g, u_char b)
{
    int colour = R2D::convertUCHARtoINT(r, g, b, 255);
    for (int i = x1; i < x2; i++)
        window->backBuffer[i + window->width * y] = colour;
}

void R2D::Vrule(window *window, int y1, int y2, int x, u_char r, u_char g, u_char b)
{
    int colour = R2D::convertUCHARtoINT(r, g, b, 255);
    for (int i = y1; i < y2; i++)
        window->backBuffer[x + window->width * i] = colour;
}

void R2D::Hrule(window *window, int x1, int x2, int y, int rgb)
{
    for (int i = x1; i < x2; i++)
        window->backBuffer[i + window->width * y] = rgb;
}

void R2D::Vrule(window *window, int y1, int y2, int x, int rgb)
{
    for (int i = y1; i < y2; i++)
        window->backBuffer[x + window->width * i] = rgb;
}

#pragma endregion

#pragma region XIAO

// swaps two numbers
void swap(int *a, int *b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}

// returns absolute value of number
float absolute(float x)
{
    if (x < 0) return -x;
    else return x;
}

//returns integer part of a floating point number
int iPartOfNumber(float x)
{
    return (int)x;
}

//rounds off a number
int roundNumber(float x)
{
    return iPartOfNumber(x + 0.5);
}

//returns fractional part of a number
float fPartOfNumber(float x)
{
    if (x > 0) return x - iPartOfNumber(x);
    else return x - (iPartOfNumber(x) + 1);

}

//returns 1 - fractional part of number
float rfPartOfNumber(float x)
{
    return 1 - fPartOfNumber(x);
}

// draws a pixel on screen of given brightness
// 0<=brightness<=1. We can use your own library
// to draw on screen

void R2D::Line(window *window, int x0, int y0, int x1, int y1)
{
    int steep = absolute(y1 - y0) > absolute(x1 - x0);

    // swap the co-ordinates if slope > 1 or we
    // draw backwards
    if (steep)
    {
        swap(&x0, &y0);
        swap(&x1, &y1);
    }
    if (x0 > x1)
    {
        swap(&x0, &x1);
        swap(&y0, &y1);
    }

    //compute the slope
    float dx = x1 - x0;
    float dy = y1 - y0;
    float gradient = dy / dx;
    if (dx == 0.0)
        gradient = 1;

    int xpxl1 = x0;
    int xpxl2 = x1;
    float intersectY = y0;

    // main loop
    if (steep)
    {
        int x;
        for (x = xpxl1; x <= xpxl2; x++)
        {
            // pixel coverage is determined by fractional
            // part of y co-ordinate
            R2D::setPixel(window, iPartOfNumber(intersectY), x);
            R2D::setPixel(window, iPartOfNumber(intersectY) - 1, x);
            intersectY += gradient;
        }
    }
    else
    {
        int x;
        for (x = xpxl1; x <= xpxl2; x++)
        {
            // pixel coverage is determined by fractional
            // part of y co-ordinate
            R2D::setPixel(window, x, iPartOfNumber(intersectY));
            R2D::setPixel(window, x, iPartOfNumber(intersectY) - 1);
            intersectY += gradient;
        }
    }

}

#pragma endregion

#pragma region RECT
void R2D::Rect(window *window, int x, int y, int w, int h)
{
    if (window->doFill)
        R2D::Box(window, x, y, w, h);
    if (window->doStroke)
        for (int i = 0; i < window->strokeWeight; i++)
            R2D::Frame(window, x + i, y + i, w - i * 2, h - i * 2);
}

void R2D::Box(window *window, int x, int y, int w, int h)
{
    for (int Y = y; Y < y + h; Y++)
        R2D::Hrule(window, x, x + w, Y, window->Fill.R, window->Fill.G, window->Fill.B);
}

void R2D::Frame(window *window, int x, int y, int w, int h)
{
    R2D::Hrule(window, x, x + w, y);
    R2D::Hrule(window, x, x + w, y + h);

    R2D::Vrule(window, y, y + h, x);
    R2D::Vrule(window, y, y + h, x + w);
}

void R2D::RectPixelMask(window *window, int x, int y, int mask)
{
    for (int X = 0; X < 8; X++)
        for (int Y = 0; Y < 4; Y++)
            if ((mask >> (31 - (7 - X) - Y * 8)) & 0x1)
                setPixel(window, x + X, y + Y);
}

void R2D::RectPixelMask(window *window, int x, int y, int size, int mask)
{
    for (int X = 0; X < 8; X++)
        for (int Y = 0; Y < 4; Y++)
            if ((mask >> (31 - (7 - X) - Y * 8)) & 0x1)
                setPixel(window, x + X * size, y + Y * size, size);
}

void R2D::RectPixelMaskAligned(window *window, int x, int y, int size, int mask)
{
    for (int X = 0; X < 8; X++)
        for (int Y = 0; Y < 4; Y++)
            if ((mask >> (31 - (7 - X) - Y * 8)) & 0x1)
                setPixelAligned(window, x + X * size, y + Y * size, size);
}

void R2D::RectPixelMask(window *window, int x, int y, int *maskv, int maskc)
{
    for (int i = 0; i < maskc; i++)
        RectPixelMask(window, x, y + i * 4, maskv[i]);
}

void R2D::RectPixelMask(window *window, int x, int y, int size, int *maskv, int maskc)
{
    for (int i = 0; i < maskc; i++)
        RectPixelMask(window, x, y + i * size * 4, size, maskv[i]);
}

void R2D::RectPixelMaskAligned(window *window, int x, int y, int size, int *maskv, int maskc)
{
    for (int i = 0; i < maskc; i++)
        RectPixelMaskAligned(window, x, y + i * size * 4, size, maskv[i]);
}

#pragma endregion

#pragma region PIXELMANIPULATION

int R2D::readPixel(window *window, int x, int y)
{
    return window->backBuffer[window->width * y + x];
}

void R2D::setPixel(window *window, int x, int y)
{
    window->backBuffer[window->width * y + x] = window->Stroke.RGBA;
}

void R2D::setPixel(window *window, int x, int y, int size)
{
    R2D::Box(window, x, y, size, size);
}

void R2D::setPixelAligned(window *window, int x, int y, int size)
{
    R2D::Box(window, x * size, y * size, size, size);
}

void R2D::PixelMask(window *window, int x, int y, int mask)
{
    for (int i = 0; i < 32; i++)
        if ((mask >> (i)) & 0x1)
            setPixel(window, x + i, y);
}

void R2D::PixelMask(window *window, int x, int y, int size, int mask)
{
    for (int i = 0; i < 32; i++)
        if ((mask >> (i)) & 0x1)
            setPixel(window, x + i * size, y, size);
}

void R2D::PixelMaskAligned(window *window, int x, int y, int size, int mask)
{
    for (int i = 0; i < 32; i++)
        if ((mask >> (i)) & 0x1)
            setPixelAligned(window, x + i * size, y, size);
}

void R2D::PixelMask(window *window, int x, int y, int *maskv, int maskc)
{
    for (int i = 0; i < maskc; i++)
        PixelMask(window, x, y + i, maskv[i]);
}

void R2D::PixelMask(window *window, int x, int y, int size, int *maskv, int maskc)
{
    for (int i = 0; i < maskc; i++)
        PixelMask(window, x, y + i * size, size, maskv[i]);
}

void R2D::PixelMaskAligned(window *window, int x, int y, int size, int *maskv, int maskc)
{
    for (int i = 0; i < maskc; i++)
        PixelMaskAligned(window, x, y + i * size, size, maskv[i]);
}

#pragma endregion

#pragma region TEXT

void R2D::PrintCharacterAt(window *window, int x, int y, mem_t ch)
{
    R2D::RectPixelMask(window, x, y, window->typeFace + window->fontMapping[ch] * 2, 2);
}

void R2D::PrintCharacterAt(window *window, int x, int y, mem_t ch, int size)
{
    R2D::RectPixelMask(window, x, y, size, window->typeFace + window->fontMapping[ch] * 2, 2);
}

void R2D::print(window *window, mem_t ch)
{
    // TODO expand
#ifdef BBP_DEBUG
    BBP::Debug::printToConsole(ch);
#endif
    switch (ch)
    {
    case '\n':
        window->coloumn = 0;
        window->line++;
        return;
    default:
        R2D::PrintCharacterAt(window, window->cursorPosX + window->fontSize * window->fontSpacing * window->coloumn++, window->cursorPosY + window->fontSize * window->fontSpacing * window->line * 2, ch, window->fontSize);
        break;
    }

}

void R2D::print(window *window, mem_t *str)
{
    mem_t *str_ptr = str;
    while (*str_ptr)
    {
        R2D::print(window, *str_ptr);
        str_ptr++;
    }
}

void R2D::print(window *window, const char *str)
{
    char *str_ptr = (char *)str;
    while (*str_ptr)
    {
        R2D::print(window, (mem_t)(*str_ptr));
        str_ptr++;
    }
}

void R2D::print(window *window, const mem_t *str)
{
    mem_t *str_ptr = (mem_t *)str;
    while (*str_ptr)
    {
        R2D::print(window, *str_ptr);
        str_ptr++;
    }
}

void R2D::print(window *window, mem_t *str, int str_len)
{
    // TODO: handle enters and such
    for (int i = 0; i < str_len; i++)
        print(window, str[i]);
}



void R2D::setCursorPos(window *window, int x, int y)
{
    window->cursorPosX = x;
    window->cursorPosY = y;
    window->coloumn = 0;
    window->line = 0;
}

#pragma endregion

#pragma endregion