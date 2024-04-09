#ifndef BBP_GLIB_VMEM_H
#define BBP_GLIB_VMEM_H

#include "Pixels.h"
#include "Memory.h"

namespace BBP
{
	namespace std
	{
		namespace R2D
		{
			// This structure holds the information about a specific piece of vmem
			struct Vmem
			{
				// The width and height of this structure
				size_t width;
				size_t height;

				// A page to the pixel information
				PAGE<pixel_t> data;

				// The actual width of the data
				size_t data_width;
				size_t data_height;

				// The offset within the vmem
				offset_t width_offset;
				offset_t height_offset;
			};

		}
	}
}

#endif
#pragma once
