#include "../include/Graphics.h"

void BBP::std::R2D::RectPixelMask(window &window, pos_t x, pos_t y, mask_t mask, colour fg, colour bg)
{
    for (int X = 0; X < 8; X++)
        for (int Y = 0; Y < 4; Y++)
            if ((mask >> (31 - (X) - (3-Y) * 8)) & 0x1)
                setPixel(window, x + X, y + Y, fg.RGBA);
            else
                setPixel(window, x + X, y + Y, bg.RGBA);
}

void BBP::std::R2D::RectPixelMask(window &window, pos_t x, pos_t y, word size, mask_t mask, colour fg, colour bg)
{
    for (int X = 0; X < 8; X++)
        for (int Y = 0; Y < 4; Y++)
            if ((mask >> (31 - (X) -(3 - Y) * 8)) & 0x1)
                setPixel(window, x + X * size, y + Y * size, fg.RGBA, size);
            else
                setPixel(window, x + X * size, y + Y * size, bg.RGBA, size);
}

void BBP::std::R2D::RectPixelMaskAligned(window &window, pos_t x, pos_t y, word size, mask_t mask, colour fg, colour bg)
{
    for (int X = 0; X < 8; X++)
        for (int Y = 0; Y < 4; Y++)
            if ((mask >> (31 - (X) -(3 - Y) * 8)) & 0x1)
                setPixelAligned(window, x + X * size, y + Y * size, fg.RGBA, size);
            else
                setPixelAligned(window, x + X * size, y + Y * size, bg.RGBA, size);
}

void BBP::std::R2D::RectPixelMask(window &window, pos_t x, pos_t y, mask_t *maskv, word maskc, colour fg, colour bg)
{
    for (std::index_t i = 0; i < maskc; i++)
        RectPixelMask(window, x, y + i * 4, maskv[i], fg, bg);
}

void BBP::std::R2D::RectPixelMask(window &window, pos_t x, pos_t y, word size, mask_t *maskv, word maskc, colour fg, colour bg)
{
    for (std::index_t row = 0; row < maskc; row++)
        for (std::index_t repetition = 0; repetition < size; repetition++)
        RectPixelMask(window, x, y + row * size * 4 + repetition, size, maskv[row], fg, bg);
}

void BBP::std::R2D::RectPixelMaskAligned(window &window, pos_t x, pos_t y, word size, mask_t *maskv, word maskc, colour fg, colour bg)
{
    for (std::index_t i = 0; i < maskc; i++)
        RectPixelMaskAligned(window, x, y + i * size * 4, size, maskv[i], fg, bg);
}