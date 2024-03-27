#include "../include/Graphics.h"

BBP::std::RGBA_t BBP::std::R2D::convertUCHARtoINT(chan_t r, chan_t g, chan_t b, chan_t a)
{
	RGBA_t ret = (a << 24) | (r << 16) | (g << 8) | b;
	return ret;
}

void BBP::std::R2D::convertINTtoUCHAR(chan_t &r, chan_t &g, chan_t &b, RGBA_t rgb)
{
	r = (rgb >> 16) & 0xFF;
	g = (rgb >> 8) & 0xFF;
	b = (rgb >> 0) & 0xFF;
}

void BBP::std::R2D::convertINTtoUCHAR(chan_t &r, chan_t &g, chan_t &b, chan_t &a, RGBA_t rgba)
{
	a = (rgba >> 24) & 0xFF;
	r = (rgba >> 16) & 0xFF;
	g = (rgba >> 8) & 0xFF;
	b = (rgba) & 0xFF;
}