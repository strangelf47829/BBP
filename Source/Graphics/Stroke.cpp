#include "../include/Graphics.h"

void BBP::std::R2D::fill(window &w, chan_t r, chan_t g, chan_t b)
{
	// Convert colours, and set fill flag
	w.Stroke.RGBA = convertUCHARtoINT(r, g, b, 255);
	w.Stroke.R = r;
	w.Stroke.G = g;
	w.Stroke.B = b;

	w.doFill = true;
}

void BBP::std::R2D::fill(window &w, chan_t r, chan_t g, chan_t b, chan_t a)
{
	// Convert colours, and set fill flag
	w.Stroke.RGBA = convertUCHARtoINT(r, g, b, a);
	w.Stroke.R = r;
	w.Stroke.G = g;
	w.Stroke.B = b;
	w.Stroke.A = a;

	w.doFill = true;
}

void BBP::std::R2D::fill(window &w, RGBA_t rgb)
{
	// Convert colours, and set fill flag
	w.Stroke.RGBA = rgb;
	convertINTtoUCHAR(w.Stroke.R, w.Stroke.G, w.Stroke.B, w.Stroke.A, rgb);

	w.doFill = true;
}


void BBP::std::R2D::fill(window &w, RGBA_t rgb, chan_t a)
{
	// Convert colours, and set fill flag
	w.Stroke.A = a;
	convertINTtoUCHAR(w.Stroke.R, w.Stroke.G, w.Stroke.B, rgb);
	w.Stroke.RGBA = convertUCHARtoINT(w.Stroke.R, w.Stroke.G, w.Stroke.B, w.Stroke.A);

	w.doFill = true;
}

void BBP::std::R2D::noFill(window &w)
{
	// Disable flag
	w.doFill = false;
}