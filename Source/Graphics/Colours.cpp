#include "../include/Graphics.h"

BBP::std::RGBA_t BBP::std::R2D::convertUCHARtoINT(chan_t r, chan_t g, chan_t b, chan_t a)
{
	RGBA_t ret = (r << 24) | (g << 16) | (b << 8) | a;
	return ret;
}

void BBP::std::R2D::convertINTtoUCHAR(chan_t &r, chan_t &g, chan_t &b, RGBA_t rgb)
{
	r = (rgb >> 24) & 0xFF;
	g = (rgb >> 16) & 0xFF;
	b = (rgb >> 8) & 0xFF;
}

void BBP::std::R2D::convertINTtoUCHAR(chan_t &r, chan_t &g, chan_t &b, chan_t &a, RGBA_t rgba)
{
	r = (rgba >> 24) & 0xFF;
	g = (rgba >> 16) & 0xFF;
	b = (rgba >> 8) & 0xFF;
	a = (rgba) & 0xFF;
}