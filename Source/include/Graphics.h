#ifndef BBP_STDLIB_GRAPHICS_H
#define BBP_STDLIB_GRAPHICS_H

#include "Memory.h"
#include "Resources.h"
#include "FileSys.h"

namespace BBP
{
	namespace std
	{

		extern ResourceManager vmem;

		/* The type for a whole colour. */
		typedef word RGBA_t;

		/* The type for a single channel */
		typedef byte chan_t;

		/* The type for a thing to specify position */
		typedef word pos_t;

		/* The type used to signify a mask */
		typedef word mask_t;

		/* Global flag to check if graphics are initialized */
		extern bool gINIT;
		
		struct colour
		{
			chan_t R = 0xFF;
			chan_t G = 0xFF;
			chan_t B = 0xFF;
			chan_t A = 0xFF;
			RGBA_t RGBA = 0xFFFFFFFF;
		};

		struct window
		{
			PAGE<RGBA_t> backBuffer;

			size_t width = 0;
			size_t height = 0;

			pos_t xPos = 0;
			pos_t yPos = 0;
			pos_t zPos = 0;

			colour Stroke = colour();
			colour Fill = colour();
			colour Chroma = colour();

			bool doStroke = true;
			bool doFill = true;

			chan_t strokeWeight = 5;

			PAGE<mask_t> typeFace;
			int fontCount;

			PAGE<string_element> charMapping;
			string_element fontMapping[256];

			chan_t fontSize = 3;
			chan_t fontSpacing = 8;

			pos_t cursorPosX = 0;
			pos_t cursorPosY = 0;

			chan_t coloumn = 0;
			chan_t line = 0;

			window()
			{
				width = 0;
				height = 0;

				xPos = 0;
				yPos = 0;
				zPos = 0;
			}

		};

		namespace Driver
		{
			extern void drawWindow(window *);

			extern int initializeGraphics(window *, int width, int height);
			extern void destructGraphics();
		}

		namespace R2D
		{
			RGBA_t convertUCHARtoINT(chan_t r, chan_t g, chan_t b, chan_t a);
			void convertINTtoUCHAR(chan_t *r, chan_t *g, chan_t *b, RGBA_t rgb);
			void convertINTtoUCHAR(chan_t *r, chan_t *g, chan_t *b, chan_t *a, RGBA_t rgba);

			void newWindow(window *, pos_t width, pos_t height, chan_t r, chan_t g, chan_t b);
			void newWindow(window *, chan_t r, chan_t g, chan_t b);

			void background(window *, chan_t r, chan_t g, chan_t b);
			void background(window *, chan_t r, chan_t g, chan_t b, chan_t a);
			void background(window *, RGBA_t rgb);
			void background(window *, RGBA_t rgb, chan_t a);

			void stroke(window *, chan_t r, chan_t g, chan_t b);
			void stroke(window *, chan_t r, chan_t g, chan_t b, chan_t a);
			void stroke(window *, RGBA_t rgb);
			void stroke(window *, RGBA_t rgb, chan_t a);
			void strokeWeight(window *, word weight);
			void noStroke(window *);

			void fill(window *, chan_t r, chan_t g, chan_t b);
			void fill(window *, chan_t r, chan_t g, chan_t b, chan_t a);
			void fill(window *, RGBA_t rgb);
			void fill(window *, RGBA_t rgb, chan_t a);
			void noFill(window *);

			void Line(window *, int x1, int y1, int x2, int y2);
			void Hrule(window *, pos_t x1, pos_t x2, pos_t y);
			void Vrule(window *, pos_t y1, pos_t y2, pos_t x);

			void Line(window *, int x1, int y1, int x2, int y2, chan_t r, chan_t g, chan_t b);
			void Hrule(window *, pos_t x1, pos_t x2, pos_t y, chan_t r, chan_t g, chan_t b);
			void Vrule(window *, pos_t y1, pos_t y2, pos_t x, chan_t r, chan_t g, chan_t b);

			void Line(window *, int x1, int y1, int x2, int y2, RGBA_t rgb);
			void Hrule(window *, pos_t x1, pos_t x2, pos_t y, RGBA_t rgb);
			void Vrule(window *, pos_t y1, pos_t y2, pos_t x, RGBA_t rgb);

			void Rect(window *, pos_t x, pos_t y, pos_t w, pos_t h);
			void Box(window *, pos_t x, pos_t y, pos_t w, pos_t h);
			void Frame(window *, pos_t x, pos_t y, pos_t w, pos_t h);

			void RectPixelMask(window *, pos_t x, pos_t y, mask_t mask);
			void RectPixelMask(window *, pos_t x, pos_t y, word size, mask_t mask);
			void RectPixelMaskAligned(window *, pos_t x, pos_t y, word size, mask_t mask);

			void RectPixelMask(window *, pos_t x, pos_t y, mask_t *maskv, word maskc);
			void RectPixelMask(window *, pos_t x, pos_t y, word size, mask_t *maskv, word maskc);
			void RectPixelMaskAligned(window *, pos_t x, pos_t y, word size, mask_t *maskv, word maskc);

			RGBA_t readPixel(window *, pos_t x, pos_t y);

			inline void setPixel(window *, pos_t x, pos_t y);
			inline void setPixel(window *, pos_t x, pos_t y, word size);
			inline void setPixelAligned(window *, pos_t x, pos_t y, word size);

			void PixelMask(window *, pos_t x, pos_t y, mask_t mask);
			void PixelMask(window *, pos_t x, pos_t y, word size, mask_t mask);
			void PixelMaskAligned(window *, pos_t x, pos_t y, word size, mask_t mask);

			void PixelMask(window *, pos_t x, pos_t y, mask_t *maskv, word maskc);
			void PixelMask(window *, pos_t x, pos_t y, word size, mask_t *maskv, word maskc);
			void PixelMaskAligned(window *, pos_t x, pos_t y, word size, mask_t *maskv, word maskc);

			void PrintCharacterAt(window *, pos_t x, pos_t y, ustring_element ch);
			void PrintCharacterAt(window *, pos_t x, pos_t y, ustring_element ch, word size);

			void print(window *, ustring_element ch);
			void print(window *, ustring_element *str);
			void print(window *, conststring str);
			void print(window *, mem_t *str, word str_c);
			void setCursorPos(window *, pos_t x, pos_t y);


			void GetFontFromFile(window *, std::PATH &);
		}

		namespace R3D
		{
			// TODO: Implement
		}
	}
}

#endif