#include "../include/Graphics.h"

void BBP::std::R2D::Vrule(window &window, pos_t y1, pos_t y2, pos_t x)
{
    // If stroke is disabled, do nothing
    if (window.doStroke == false)
        return;

    Vrule(window, y1, y2, x, window.Stroke.RGBA);
}

void BBP::std::R2D::Vrule(window &window, pos_t y1, pos_t y2, pos_t x, chan_t r, chan_t g, chan_t b)
{
    Vrule(window, y1, y2, x, convertUCHARtoINT(r, g, b, 255));
}

void BBP::std::R2D::Vrule(window &window, pos_t y1, pos_t y2, pos_t x, RGBA_t rgb)
{
    // If y1 is greater than y2, swap them
    if (y1 > y2)
    {
        pos_t swp = y1;
        y1 = y2;
        y2 = swp;
    }

    // If y1 is outside of window borders, do nothing
    if (y1 >= window.height)
        return;

    // If width is outside of window borders, do nothing
    if (x >= window.height)
        return;

    // If x2 is outside window border, set x2 to window border
    if (y2 >= window.width)
        y2 = window.width - 1;

    for (std::index_t i = y1; i < y2; i++)
        window.backBuffer[x + window.width * i] = rgb;
}