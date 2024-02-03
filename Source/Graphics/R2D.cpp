/*

Copyright 2023 PCiD

This file is part of BBP.

BBP is free software: you can redistribute it and/or modify it under the terms of the GNU Lesser General
Public License as published by the Free Software Foundation, either version 3 of the License, or (at your
option) any later version.
convertUCHARtoINT
BBP is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even
the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License along with BBP. If not, see
<https://www.gnu.org/licenses/>.

Author: Rafael de Bie

*/

#include "../include/Graphics.h"
#include "../include/FileSys.h"

using namespace BBP::std;

namespace BBP
{

    namespace std
    {

        ResourceManager vmem;

        // COLOURS

        RGBA_t R2D::convertUCHARtoINT(chan_t r, chan_t g, chan_t b, chan_t a)
        {
            int ret = (r << 24) | (g << 16) | (b << 8) | a;
            return ret;
        }
        void R2D::convertINTtoUCHAR(chan_t *r, chan_t *g, chan_t *b, RGBA_t rgb)
        {
            *r = (rgb >> 24) & 0xFF;
            *g = (rgb >> 16) & 0xFF;
            *b = (rgb >> 8) & 0xFF;
        }
        void R2D::convertINTtoUCHAR(chan_t *r, chan_t *g, chan_t *b, chan_t *a, RGBA_t rgba)
        {
            *r = (rgba >> 24) & 0xFF;
            *g = (rgba >> 16) & 0xFF;
            *b = (rgba >> 8) & 0xFF;
            *a = (rgba) & 0xFF;
        }


        void R2D::stroke(window *window, chan_t r, chan_t g, chan_t b)
        {
            window->Stroke.R = r;
            window->Stroke.G = g;
            window->Stroke.B = b;
            window->Stroke.RGBA = convertUCHARtoINT(r, g, b, window->Stroke.A);
        }
        void R2D::stroke(window *window, chan_t r, chan_t g, chan_t b, chan_t a)
        {
            window->Stroke.R = r;
            window->Stroke.G = g;
            window->Stroke.B = b;
            window->Stroke.A = a;
            window->Stroke.RGBA = convertUCHARtoINT(r, g, b, a);
        }
        void R2D::stroke(window *window, RGBA_t rgb)
        {
            window->Stroke.RGBA = rgb;
            convertINTtoUCHAR(&(window->Stroke.R), &(window->Stroke.G), &(window->Stroke.B), rgb);
        }
        void R2D::stroke(window *window, RGBA_t rgb, chan_t a)
        {
            convertINTtoUCHAR(&(window->Stroke.R), &(window->Stroke.G), &(window->Stroke.B), rgb);
            window->Stroke.A = a;
            window->Stroke.RGBA = convertUCHARtoINT(window->Stroke.R, window->Stroke.G, window->Stroke.B, a);
        }

        void R2D::noStroke(window *window)
        {
            window->doStroke = false;
        }

        void R2D::strokeWeight(window *window, word weight)
        {
            window->strokeWeight = weight;
        }

        void R2D::fill(window *window, chan_t r, chan_t g, chan_t b)
        {
            window->Fill.R = r;
            window->Fill.G = g;
            window->Fill.B = b;
            window->Fill.RGBA = convertUCHARtoINT(r, g, b, window->Fill.A);
            window->doFill = true;
        }
        void R2D::fill(window *window, chan_t r, chan_t g, chan_t b, chan_t a)
        {
            window->Fill.R = r;
            window->Fill.G = g;
            window->Fill.B = b;
            window->Fill.A = a;
            window->Fill.RGBA = convertUCHARtoINT(r, g, b, a);
            window->doFill = true;
        }
        void R2D::fill(window *window, RGBA_t rgb)
        {
            window->Fill.RGBA = rgb;
            convertINTtoUCHAR(&(window->Fill.R), &(window->Fill.G), &(window->Fill.B), rgb);
            window->doFill = true;
        }
        void R2D::fill(window *window, RGBA_t rgb, chan_t a)
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

        void R2D::newWindow(window *window, pos_t width, pos_t height, chan_t r, chan_t g, chan_t b)
        {
            if (window == nullptr)
                return;

            // If dimensions match last window, no need to re-allocate memory
            if (window->width != width || window->height != height)
            {
                // Check if backbuffer is actually allocated
                if (window->backBuffer.data)
                    vmem.free(window->backBuffer.data);

                // Allocate new backbuffer.
                window->backBuffer = std::PAGE<RGBA_t>(width * height, (RGBA_t *)vmem.calloc(width * height, sizeof(RGBA_t)));
            }

            // If nothing was stored last time, then we might as well just re-alloc
            if ((width && !window->width) || (height && !window->height))
            {
                // Allocate new backbuffer.
                //window->backBuffer = std::PAGE<int>(width * height, (int *)vmem.calloc(width * height, sizeof(int)));
            }
            
            window->width = width;
            window->height = height;

            R2D::background(window, r, g, b);
        }

        void R2D::newWindow(window *window, chan_t r, chan_t g, chan_t b)
        {
            if (window == nullptr)
                return;

            R2D::background(window, r, g, b);
        }

        void R2D::GetFontFromFile(window *window, PATH &path)
        {
            // If window pointer is invalid, do nothing.
            if (window == nullptr)
                return;

            // Open file from path
            BBP::std::FILE fontFile(path);

            byte *data = (byte *)fontFile.b().page->data;

            // Font is now in fontFile. From this, we can now obtain pointers to the data
            size_t nums = data[0];
            RGBA_t *font = (RGBA_t *)read_a(fontFile.b().page, 4);
            ustring_element *mapping = (ustring_element *)read_ar(fontFile.b().page, 4 + nums * 8);

            // First find the \0 character from ROM
            int nullCharacterIndex = 0;
            for (std::index_t i = 0; i < nums; i++)
            {
                if (mapping[i]) // If the character is not '\0', continue
                    continue;
                nullCharacterIndex = i; // \0 found!
                break;
            }
            // Then write that font map to each character
            for (std::index_t i = 0; i < 256; i++)
                window->fontMapping[i] = nullCharacterIndex;

            // Then overwrite the known fonts from the map
            for (std::index_t i = 0; i < nums; i++)
                window->fontMapping[mapping[i]] = i;

            // If there is another font stored in window, clear it
            if (window->typeFace.data)
                vmem.free(window->typeFace.data);

            // Then allocate typeface
            window->typeFace = BBP::std::PAGE<mask_t>(2 * nums, (mask_t *)vmem.calloc(2 * nums, sizeof(mask_t)));

            // Copy data
            for (BBP::std::index_t idx = 0; idx < window->typeFace.dataSize; idx++)
            {
                // TODO: Not compiler agnostic! Needs to be changed.
                word swapped = __builtin_bswap32(font[idx]);
                window->typeFace.data[idx] = swapped;
            }

            // Since everything has been copied, close the font file
            fontFile.close();
        }


        void R2D::background(window *window, chan_t r, chan_t g, chan_t b)
        {
            RGBA_t colour = R2D::convertUCHARtoINT(r, g, b, 255);
            for (std::index_t n = 0; n < window->width * window->height; n++)
                write(&window->backBuffer, colour, n);
        }
        void R2D::background(window *window, chan_t r, chan_t g, chan_t b, chan_t a)
        {
            RGBA_t colour = R2D::convertUCHARtoINT(r, g, b, a);
            for (std::index_t n = 0; n < window->width * window->height; n++)
                window->backBuffer.data[n] = colour;
        }
        void R2D::background(window *window, RGBA_t rgba)
        {
            for (std::index_t n = 0; n < window->width * window->height; n++)
                window->backBuffer.data[n] = rgba;
        }
        void R2D::background(window *window, RGBA_t rgb, chan_t a)
        {
            RGBA_t colour = (rgb & ~0xFF) ^ a;
            for (std::index_t n = 0; n < window->width * window->height; n++)
                window->backBuffer.data[n] = colour;
        }

        void R2D::Hrule(window *window, pos_t x1, pos_t x2, pos_t y)
        {
            RGBA_t colour = R2D::convertUCHARtoINT(window->Stroke.R, window->Stroke.G, window->Stroke.B, window->Stroke.A);
            for (std::index_t i = x1; i < x2; i++)
                window->backBuffer.data[i + window->width * y] = colour;
        }

        void R2D::Vrule(window *window, pos_t y1, pos_t y2, pos_t x)
        {
            RGBA_t colour = R2D::convertUCHARtoINT(window->Stroke.R, window->Stroke.G, window->Stroke.B, window->Stroke.A);
            for (std::index_t i = y1; i < y2; i++)
                window->backBuffer.data[x + window->width * i] = colour;
        }

        void R2D::Hrule(window *window, pos_t x1, pos_t x2, pos_t y, chan_t r, chan_t g, chan_t b)
        {
            RGBA_t colour = R2D::convertUCHARtoINT(r, g, b, 255);
            for (std::index_t i = x1; i < x2; i++)
                window->backBuffer.data[i + window->width * y] = colour;
        }

        void R2D::Vrule(window *window, pos_t y1, pos_t y2, pos_t x, chan_t r, chan_t g, chan_t b)
        {
            RGBA_t colour = R2D::convertUCHARtoINT(r, g, b, 255);
            for (std::index_t i = y1; i < y2; i++)
                window->backBuffer.data[x + window->width * i] = colour;
        }

        void R2D::Hrule(window *window, pos_t x1, pos_t x2, pos_t y, RGBA_t rgb)
        {
            for (std::index_t i = x1; i < x2; i++)
                window->backBuffer.data[i + window->width * y] = rgb;
        }

        void R2D::Vrule(window *window, pos_t y1, pos_t y2, pos_t x, RGBA_t rgb)
        {
            for (std::index_t i = y1; i < y2; i++)
                window->backBuffer.data[x + window->width * i] = rgb;
        }


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

        void R2D::Rect(window *window, pos_t x, pos_t y, pos_t w, pos_t h)
        {
            if (window->doFill)
                R2D::Box(window, x, y, w, h);
            if (window->doStroke)
                for (std::index_t i = 0; i < window->strokeWeight; i++)
                    R2D::Frame(window, x + i, y + i, w - i * 2, h - i * 2);
        }

        void R2D::Box(window *window, pos_t x, pos_t y, pos_t w, pos_t h)
        {
            for (std::index_t Y = y; Y < y + h; Y++)
                R2D::Hrule(window, x, x + w, Y, window->Fill.R, window->Fill.G, window->Fill.B);
        }

        void R2D::Frame(window *window, pos_t x, pos_t y, pos_t w, pos_t h)
        {
            R2D::Hrule(window, x, x + w, y);
            R2D::Hrule(window, x, x + w, y + h);

            R2D::Vrule(window, y, y + h, x);
            R2D::Vrule(window, y, y + h, x + w);
        }

        void R2D::RectPixelMask(window *window, pos_t x, pos_t y, mask_t mask)
        {
            for (int X = 0; X < 8; X++)
                for (int Y = 0; Y < 4; Y++)
                    if ((mask >> (31 - (7 - X) - Y * 8)) & 0x1)
                        setPixel(window, x + X, y + Y);
        }

        void R2D::RectPixelMask(window *window, pos_t x, pos_t y, word size, mask_t mask)
        {
            for (int X = 0; X < 8; X++)
                for (int Y = 0; Y < 4; Y++)
                    if ((mask >> (31 - (7 - X) - Y * 8)) & 0x1)
                        setPixel(window, x + X * size, y + Y * size, size);
        }

        void R2D::RectPixelMaskAligned(window *window, pos_t x, pos_t y, word size, mask_t mask)
        {
            for (int X = 0; X < 8; X++)
                for (int Y = 0; Y < 4; Y++)
                    if ((mask >> (31 - (7 - X) - Y * 8)) & 0x1)
                        setPixelAligned(window, x + X * size, y + Y * size, size);
        }

        void R2D::RectPixelMask(window *window, pos_t x, pos_t y, mask_t *maskv, word maskc)
        {
            for (std::index_t i = 0; i < maskc; i++)
                RectPixelMask(window, x, y + i * 4, maskv[i]);
        }

        void R2D::RectPixelMask(window *window, pos_t x, pos_t y, word size, mask_t *maskv, word maskc)
        {
            for (std::index_t i = 0; i < maskc; i++)
                RectPixelMask(window, x, y + i * size * 4, size, maskv[i]);
        }

        void R2D::RectPixelMaskAligned(window *window, pos_t x, pos_t y, word size, mask_t *maskv, word maskc)
        {
            for (std::index_t i = 0; i < maskc; i++)
                RectPixelMaskAligned(window, x, y + i * size * 4, size, maskv[i]);
        }

        RGBA_t R2D::readPixel(window *window, pos_t x, pos_t y)
        {
            return window->backBuffer.data[window->width * y + x];
        }

        void R2D::setPixel(window *window, pos_t x, pos_t y)
        {
            window->backBuffer.data[window->width * y + x] = window->Stroke.RGBA;
        }

        void R2D::setPixel(window *window, pos_t x, pos_t y, word size)
        {
            R2D::Box(window, x, y, size, size);
        }

        void R2D::setPixelAligned(window *window, pos_t x, pos_t y, word size)
        {
            R2D::Box(window, x * size, y * size, size, size);
        }

        void R2D::PixelMask(window *window, pos_t x, pos_t y, mask_t mask)
        {
            for (std::index_t i = 0; i < 32; i++)
                if ((mask >> (i)) & 0x1)
                    setPixel(window, x + i, y);
        }

        void R2D::PixelMask(window *window, pos_t x, pos_t y, word size, mask_t mask)
        {
            for (std::index_t i = 0; i < 32; i++)
                if ((mask >> (i)) & 0x1)
                    setPixel(window, x + i * size, y, size);
        }

        void R2D::PixelMaskAligned(window *window, pos_t x, pos_t y, word size, mask_t mask)
        {
            for (std::index_t i = 0; i < 32; i++)
                if ((mask >> (i)) & 0x1)
                    setPixelAligned(window, x + i * size, y, size);
        }

        void R2D::PixelMask(window *window, pos_t x, pos_t y, mask_t *maskv, word maskc)
        {
            for (std::index_t i = 0; i < maskc; i++)
                PixelMask(window, x, y + i, maskv[i]);
        }

        void R2D::PixelMask(window *window, pos_t x, pos_t y, word size, mask_t *maskv, word maskc)
        {
            for (std::index_t i = 0; i < maskc; i++)
                PixelMask(window, x, y + i * size, size, maskv[i]);
        }

        void R2D::PixelMaskAligned(window *window, pos_t x, pos_t y, word size, mask_t *maskv, word maskc)
        {
            for (std::index_t i = 0; i < maskc; i++)
                PixelMaskAligned(window, x, y + i * size, size, maskv[i]);
        }


        void R2D::PrintCharacterAt(window *window, pos_t x, pos_t y, unsigned char ch)
        {
            
            R2D::RectPixelMask(window, x, y, window->typeFace.data + window->fontMapping[ch] * 2, 2);
        }

        void R2D::PrintCharacterAt(window *window, pos_t x, pos_t y, unsigned char ch, word size)
        {
            R2D::RectPixelMask(window, x, y, size, window->typeFace.data + window->fontMapping[ch] * 2, 2);
        }

        void R2D::print(window *window, unsigned char ch)
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

        void R2D::print(window *window, uc_string str)
        {
            ustring_element *str_ptr = str;
            while (*str_ptr)
            {
                R2D::print(window, *str_ptr);
                str_ptr++;
            }
        }

        void R2D::print(window *window, conststring str)
        {
            ustring_element *str_ptr = (ustring_element *)str;
            while (*str_ptr)
            {
                R2D::print(window, (ustring_element)(*str_ptr));
                str_ptr++;
            }
        }

        void R2D::print(window *window, uc_string str, word str_len)
        {
            // TODO: handle enters and such
            for (std::index_t i = 0; i < str_len; i++)
                print(window, str[i]);
        }

        void R2D::setCursorPos(window *window, pos_t x, pos_t y)
        {
            window->cursorPosX = x;
            window->cursorPosY = y;
            window->coloumn = 0;
            window->line = 0;
        }
    }
}
