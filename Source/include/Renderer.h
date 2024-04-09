#ifndef BBP_GLIB_RENDERER_H
#define BBP_GLIB_RENDERER_H

#include "Vmem.h"

namespace BBP
{
	namespace std
	{
		namespace R2D
		{

			// This class contains a bit of vmem and can render stuff to that memory
			class Renderer
			{
			public:

				// Set pixels 
				virtual void setPixel(size_t x, size_t h, pixel_t) = 0;
				virtual void setPixel(size_t x, size_t h, channel_t r, channel_t g, channel_t b) = 0;
				virtual void setPixel(size_t x, size_t h, channel_t r, channel_t g, channel_t b, channel_t a) = 0;

				// Read pixels
				virtual pixel_t readPixel(size_t, size_t) = 0;

				// Get and set render depth
				virtual void setDepth(index_t) = 0;
				virtual index_t readDepth() = 0;

			private:

				// The memory stored within this renderer
				Vmem memory;

				// 

			};


		}
	}
}

#endif
