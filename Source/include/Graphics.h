#ifndef BBP_STDLIB_GRAPHICS_H
#define BBP_STDLIB_GRAPHICS_H

#include "Strings.h"

namespace BBP
{
	namespace std
	{
		/* The type for a whole colour. */
		typedef word RGBA_t;

		/* The type for a single channel */
		typedef byte chan_t;

		/* The type for a thing to specify position */
		typedef word pos_t;

		/* The type used to signify a mask */
		typedef word mask_t;
		
		// Structure to hold colour information
		struct colour
		{
			chan_t R = 0xFF;
			chan_t G = 0xFF;
			chan_t B = 0xFF;
			chan_t A = 0xFF;
			RGBA_t RGBA = 0xFFFFFFFF;
		};

		// Structure to hold font information (in PSF1/2 format)
		struct Font
		{
			static constexpr halfword PSF1Magic = 0x3604;
			static constexpr word PSF2Magic = 0x72b54a86;

			// The header for each font
			union
			{
				// Header for PSF1 type fonts
				struct PSF1Header
				{
					halfword magic;	// 0x3604
					byte mode;		// Unused
					byte glyphSize;	// Glyph size in bytes.
				} headerPSF1;

				// Header for PSF2 type fonts
				struct PSF2Header
				{
					word magic;		// 0x72b54a86
					word version;	// Always 0
					word headerSize;// Size of this header
					word flags;		// Unused
					word length;	// Glyph count
					word glyphSize; // Bytes per glyph
					word height;	// Height per glyph
					word width;		// Width per glyph
				} headerPSF2;
			} headers;

			// Stores the font version
			enum PSFVersion
			{
				PSF1,
				PSF2
			} version;

			// Glyphs and such
			PAGE<mask_t> glyphs;
			STATIC_PAGE<offset_t, 256> mapping;

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

			Font activeFont;

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

		namespace R2D
		{
			RGBA_t convertUCHARtoINT(chan_t r, chan_t g, chan_t b, chan_t a);
			void convertINTtoUCHAR(chan_t &r, chan_t &g, chan_t &b, RGBA_t rgb);
			void convertINTtoUCHAR(chan_t &r, chan_t &g, chan_t &b, chan_t &a, RGBA_t rgba);

			void newWindow(window &, pos_t width, pos_t height, chan_t r, chan_t g, chan_t b);
			void newWindow(window &, chan_t r, chan_t g, chan_t b);

			void background(window &, chan_t r, chan_t g, chan_t b);
			void background(window &, chan_t r, chan_t g, chan_t b, chan_t a);
			void background(window &, RGBA_t rgb);
			void background(window &, RGBA_t rgb, chan_t a);

			void stroke(window &, chan_t r, chan_t g, chan_t b);
			void stroke(window &, chan_t r, chan_t g, chan_t b, chan_t a);
			void stroke(window &, RGBA_t rgb);
			void stroke(window &, RGBA_t rgb, chan_t a);
			void strokeWeight(window &, word weight);
			void noStroke(window &);

			void fill(window &, chan_t r, chan_t g, chan_t b);
			void fill(window &, chan_t r, chan_t g, chan_t b, chan_t a);
			void fill(window &, RGBA_t rgb);
			void fill(window &, RGBA_t rgb, chan_t a);
			void noFill(window &);

			void Line(window &, int x1, int y1, int x2, int y2);
			void Hrule(window &, pos_t x1, pos_t x2, pos_t y);
			void Vrule(window &, pos_t y1, pos_t y2, pos_t x);

			void Line(window &, int x1, int y1, int x2, int y2, chan_t r, chan_t g, chan_t b);
			void Hrule(window &, pos_t x1, pos_t x2, pos_t y, chan_t r, chan_t g, chan_t b);
			void Vrule(window &, pos_t y1, pos_t y2, pos_t x, chan_t r, chan_t g, chan_t b);

			void Line(window &, int x1, int y1, int x2, int y2, RGBA_t rgb);
			void Hrule(window &, pos_t x1, pos_t x2, pos_t y, RGBA_t rgb);
			void Vrule(window &, pos_t y1, pos_t y2, pos_t x, RGBA_t rgb);

			void Rect(window &, pos_t x, pos_t y, pos_t w, pos_t h);
			void Box(window &, pos_t x, pos_t y, pos_t w, pos_t h);
			void Frame(window &, pos_t x, pos_t y, pos_t w, pos_t h);

			void Rect(window &, pos_t x, pos_t y, pos_t w, pos_t h, colour);
			void Box(window &, pos_t x, pos_t y, pos_t w, pos_t h, colour);
			void Frame(window &, pos_t x, pos_t y, pos_t w, pos_t h, colour);

			void RectPixelMask(window &, pos_t x, pos_t y, mask_t mask, colour);
			void RectPixelMask(window &, pos_t x, pos_t y, word size, mask_t mask, colour);
			void RectPixelMaskAligned(window &, pos_t x, pos_t y, word size, mask_t mask, colour);

			void RectPixelMask(window &, pos_t x, pos_t y, mask_t *maskv, word maskc, colour);
			void RectPixelMask(window &, pos_t x, pos_t y, word size, mask_t *maskv, word maskc, colour);
			void RectPixelMaskAligned(window &, pos_t x, pos_t y, word size, mask_t *maskv, word maskc, colour);

			RGBA_t readPixel(window &, pos_t x, pos_t y);

			void setPixel(window &, pos_t x, pos_t y, RGBA_t colour);
			void setPixel(window &, pos_t x, pos_t y, RGBA_t colour, word size);
			void setPixelAligned(window &, pos_t x, pos_t y, RGBA_t colour, word size);

			void PixelMask(window &, pos_t x, pos_t y, mask_t mask, colour);
			void PixelMask(window &, pos_t x, pos_t y, word size, mask_t mask, colour);
			void PixelMaskAligned(window &, pos_t x, pos_t y, word size, mask_t mask, colour);

			void PixelMask(window &, pos_t x, pos_t y, mask_t *maskv, word maskc, colour);
			void PixelMask(window &, pos_t x, pos_t y, word size, mask_t *maskv, word maskc, colour);
			void PixelMaskAligned(window &, pos_t x, pos_t y, word size, mask_t *maskv, word maskc, colour);

			void PrintCharacterAt(window &, pos_t x, pos_t y, ustring_element ch);
			void PrintCharacterAt(window &, pos_t x, pos_t y, ustring_element ch, word size);

			void PrintCharacterAt(window &, pos_t x, pos_t y, ustring_element ch, colour col);
			void PrintCharacterAt(window &, pos_t x, pos_t y, ustring_element ch, word size, colour col);

			void print(window &, ustring_element ch, colour col);
			void print(window &, conststring str, colour col);
			void print(window &, string str, colour col);

			void print(window &, string str);
			void print(window &, conststring str);

			void setCursorPos(window &, pos_t x, pos_t y);

			// Set active Fonts
			void setActiveFont(window &, Font &);

			// Active window stuff
			void setActiveWindow(window &);
			window &activeWindow();
		}

		// Load a font from memory
		void loadFont(Font &, ResourceManager &, std::PAGE<std::string_element> &);

		// Load font per type
		void loadFontPSF1(Font &, ResourceManager &, std::PAGE<std::string_element> &);
		void loadFontPSF2(Font &, ResourceManager &, std::PAGE<std::string_element> &);

		// Unload a font
		void unloadFont(Font &);

		// Load font from disk
		void loadFontFromDisk(Font &, ResourceManager &, std::string);
	}
}

#endif