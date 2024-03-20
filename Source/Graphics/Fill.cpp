#include "../include/Graphics.h"

void BBP::std::R2D::fill(window &w, chan_t r, chan_t g, chan_t b)
{
	// Convert colours, and set fill flag
	w.Fill.RGBA = convertUCHARtoINT(r, g, b, 255);
	w.Fill.R = r;
	w.Fill.G = g;
	w.Fill.B = b;

	w.doFill = true;
}

void BBP::std::R2D::fill(window &w, chan_t r, chan_t g, chan_t b, chan_t a)
{
	// Convert colours, and set fill flag
	w.Fill.RGBA = convertUCHARtoINT(r, g, b, a);
	w.Fill.R = r;
	w.Fill.G = g;
	w.Fill.B = b;
	w.Fill.A = a;

	w.doFill = true;
}

void BBP::std::R2D::fill(window &w, RGBA_t rgb)
{
	// Convert colours, and set fill flag
	w.Fill.RGBA = rgb;
	convertINTtoUCHAR(w.Fill.R, w.Fill.G, w.Fill.B, w.Fill.A, rgb);

	w.doFill = true;
}


void BBP::std::R2D::fill(window &w, RGBA_t rgb, chan_t a)
{
	// Convert colours, and set fill flag
	w.Fill.A = a;
	convertINTtoUCHAR(w.Fill.R, w.Fill.G, w.Fill.B, rgb);
	w.Fill.RGBA = convertUCHARtoINT(w.Fill.R, w.Fill.G, w.Fill.B, w.Fill.A);

	w.doFill = true;
}

void BBP::std::R2D::noFill(window &w)
{
	// Disable flag
	w.doFill = false;
}