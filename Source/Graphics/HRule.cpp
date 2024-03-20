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

    // If x1 is outside of window borders, do nothing
    if (x1 >= window.width)
        return;

    // If height is outside of window borders, do nothing
    if (y >= window.height)
        return;

    // If x2 is outside window border, set x2 to window border
    if (x2 >= window.width)
        x2 = window.width - 1;

    for (std::index_t i = x1; i < x2; i++)
        window.backBuffer[i + window.width * y] = rgb;
}