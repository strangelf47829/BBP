#ifndef BBP_TERMINALSTATE_H
#define BBP_TERMINALSTATE_H

#include "Graphics.h"

namespace BBP
{
	namespace std
	{
		namespace Terminal
		{
			// This structure stores the possible states of a terminal.
			// Although it is possible to control this state manually,
			// Using a ECMA35_48Controller would allow for more
			// Conventional conformity.

			// Using 'CC' as a single byte
			using CC = BBP::std::byte;

			struct TerminalState
			{
				// This structure stores information about a 2D section of a terminal
				struct TerminalRect
				{
					size_t vertical;
					size_t horizontal;
				};

				// The total amount of lines
				size_t lineCount;

				// The total size of the viewport of this terminal (characters x characters)
				TerminalRect viewportSize;

				// The current position of the viewport (vertical in lines, horizontal in pages)
				TerminalRect viewportPosition;

				// The current active presentation position
				TerminalRect activePresentationPosition;

				// The current foreground and background colours
				colour fg;
				colour bg;

				// Default colours
				static const RGBA_t defaultFg = 0xFFCCCCCC;
				static const RGBA_t defaultBg = 0xFF000000;

				// First line of the terminal
				w_string firstLine;

				// Page of terminal lines.
				PAGE<w_string *> terminalLines;

				// Set of active fonts
				constexpr static size_t fontCount = 10;
				STATIC_PAGE<Font, fontCount> fonts;

				// Index of currently active font
				index_t activeFont;

				// The allocator used to allocate stuff
				ResourceManager *allocator;

				// The amount of tabulation
				TerminalRect tabularSettings;

				// DCSM - Device Component Select Mode
				enum DCSMSetting
				{
					Presentation,
					Data
				};

				DCSMSetting DCSM;

				// SIMD - Select movement implicit direction
				word SIMD;

				// Terminal limit settings
				TerminalRect Limits;

				// Superscript, subscript, and normal
				size_t defaultFontSize;
				size_t defaultFontSubscriptSize;
				size_t defaultFontSuperscriptSize;

				// If LF should also implicitly mean CR
				bool implicitCR = true;
			
				// Whether a move operation should also mean a scroll instruction
				bool implicitScroll = true;

				enum TextMode
				{
					normal,
					Superscript,
					Subscript
				} textmode;

				// ... implement stuff based on necessity.

				// Constructors
				TerminalState(size_t lc, size_t vp_w, size_t vp_h, ResourceManager *man);

				// Initialize (called once)
				void initialize();

			};

		}
	}
}

#endif
