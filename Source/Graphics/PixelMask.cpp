#include "../include/Graphics.h"

void BBP::std::R2D::PixelMask(window &window, pos_t x, pos_t y, mask_t mask, colour fg, colour bg)
{
    for (std::index_t i = 0; i < 32; i++)
        if ((mask >> (i)) & 0x1)
            setPixel(window, x + i, y, window.Chroma.RGBA);
}

void BBP::std::R2D::PixelMask(window &window, pos_t x, pos_t y, word size, mask_t mask, colour fg, colour bg)
{
    for (std::index_t i = 0; i < 32; i++)
        if ((mask >> (i)) & 0x1)
            setPixel(window, x + i * size, y, fg.RGBA, size);
        else
            setPixel(window, x + i * size, y, bg.RGBA, size);
}

void BBP::std::R2D::PixelMaskAligned(window &window, pos_t x, pos_t y, word size, mask_t mask, colour fg, colour bg)
{
    for (std::index_t i = 0; i < 32; i++)
        if ((mask >> (i)) & 0x1)
            setPixelAligned(window, x + i * size, y, fg.RGBA, size);
        else
            setPixelAligned(window, x + i * size, y, bg.RGBA, size);
}

void BBP::std::R2D::PixelMask(window &window, pos_t x, pos_t y, mask_t *maskv, word maskc, colour fg, colour bg)
{
    for (std::index_t i = 0; i < maskc; i++)
        PixelMask(window, x, y + i, maskv[i], fg, bg);
}

void BBP::std::R2D::PixelMask(window &window, pos_t x, pos_t y, word size, mask_t *maskv, word maskc, colour fg, colour bg)
{
    for (std::index_t i = 0; i < maskc; i++)
        PixelMask(window, x, y + i * size, size, maskv[i], fg, bg);
}

void BBP::std::R2D::PixelMaskAligned(window &window, pos_t x, pos_t y, word size, mask_t *maskv, word maskc, colour fg, colour bg)
{
    for (std::index_t i = 0; i < maskc; i++)
        PixelMaskAligned(window, x, y + i * size, size, maskv[i], fg, bg);
}