#include "../include/Graphics.h"

void BBP::std::R2D::Hrule(window &window, pos_t x1, pos_t x2, pos_t y)
{
    // If stroke is disabled, do nothing
    if (window.doStroke == false)
        return;

    Hrule(window, x1, x2, y, window.Stroke.RGBA);
}

void BBP::std::R2D::Hrule(window &window, pos_t x1, pos_t x2, pos_t y, chan_t r, chan_t g, chan_t b)
{
    Hrule(window, x1, x2, y, convertUCHARtoINT(r, g, b, 255));
}

void BBP::std::R2D::Hrule(window &window, pos_t x1, pos_t x2, pos_t y, RGBA_t rgb)
{
    // If x1 is greater than x2, swap them
    if (x1 > x2)
    {
        pos_t swp = x1;
        x1 = x2;
        x2 = swp;
    }

    for (std::index_t i = x1; i < x2; i++)
        window.setPixel(i, y, rgb);
}