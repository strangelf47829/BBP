#include "../include/Graphics.h"

BBP::std::RGBA_t BBP::std::R2D::readPixel(window &window, pos_t x, pos_t y)
{
    return window.backBuffer[window.width * y + x];
}

void BBP::std::R2D::setPixel(window &window, pos_t x, pos_t y, RGBA_t colour)
{
    window.backBuffer[window.width * y + x] = colour;
}

void BBP::std::R2D::setPixel(window &window, pos_t x, pos_t y, RGBA_t colour, word size)
{
    // TODO: Implement checks
    for (std::index_t idx = 0; idx < size; idx++)
        Hrule(window, x, x + size, y + size, colour);
}

void BBP::std::R2D::setPixelAligned(window &window, pos_t x, pos_t y, RGBA_t colour, word size)
{
    R2D::Box(window, x * size, y * size, size, size);
}