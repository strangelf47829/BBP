#include "../include/Graphics.h"

void BBP::std::R2D::RectPixelMask(window &window, pos_t x, pos_t y, mask_t mask)
{
    for (int X = 0; X < 8; X++)
        for (int Y = 0; Y < 4; Y++)
            if ((mask >> (31 - (7 - X) - Y * 8)) & 0x1)
                setPixel(window, x + X, y + Y, window.Chroma.RGBA);
}

void BBP::std::R2D::RectPixelMask(window &window, pos_t x, pos_t y, word size, mask_t mask)
{
    for (int X = 0; X < 8; X++)
        for (int Y = 0; Y < 4; Y++)
            if ((mask >> (31 - (7 - X) - Y * 8)) & 0x1)
                setPixel(window, x + X * size, y + Y * size, size);
}

void BBP::std::R2D::RectPixelMaskAligned(window &window, pos_t x, pos_t y, word size, mask_t mask)
{
    for (int X = 0; X < 8; X++)
        for (int Y = 0; Y < 4; Y++)
            if ((mask >> (31 - (7 - X) - Y * 8)) & 0x1)
                setPixelAligned(window, x + X * size, y + Y * size, window.Stroke.RGBA, size);
}

void BBP::std::R2D::RectPixelMask(window &window, pos_t x, pos_t y, mask_t *maskv, word maskc)
{
    for (std::index_t i = 0; i < maskc; i++)
        RectPixelMask(window, x, y + i * 4, maskv[i]);
}

void BBP::std::R2D::RectPixelMask(window &window, pos_t x, pos_t y, word size, mask_t *maskv, word maskc)
{
    for (std::index_t i = 0; i < maskc; i++)
        RectPixelMask(window, x, y + i * size * 4, size, maskv[i]);
}

void BBP::std::R2D::RectPixelMaskAligned(window &window, pos_t x, pos_t y, word size, mask_t *maskv, word maskc)
{
    for (std::index_t i = 0; i < maskc; i++)
        RectPixelMaskAligned(window, x, y + i * size * 4, size, maskv[i]);
}