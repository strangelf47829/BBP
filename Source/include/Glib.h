#ifndef BBP_GLIB_GLIB_H
#define BBP_GLIB_GLIB_H

#include "Gobject.h"
#include "Resources.h"

// This file defines the actual functions and constructs used to manipulate GObjects.
// One such example is the creation and destruction of GObjects, creating copies, etc...

namespace BBP
{
	namespace std
	{
		namespace R2D
		{

			// Vertex operators, addition
			GObj_2DVertex operator+(GObj_2DVertex &a, GObj_2DVertex &b);
			GObj_2DVertex operator-(GObj_2DVertex &a, GObj_2DVertex &b);

			// Vertex operators, cross product and dot product
			GObj_2DVertex operator%(GObj_2DVertex &a, GObj_2DVertex &b);
			GObj_2DVertex operator*(GObj_2DVertex &a, GObj_2DVertex &b);

			// Vertex operators, magnitude operations
			GObj_2DVertex operator*(GObj_2DVertex &a, mag_t b);
			GObj_2DVertex operator/(GObj_2DVertex &a, mag_t b);

			// Vertex operators, magnitude and argument
			mag_t magnitude(GObj_2DVertex &a);
			angle_t argument(GObj_2DVertex &a);

			// Vertex operators, normalization
			GObj_2DVertex normalize(GObj_2DVertex &a);

			// Vertex operators, rotation
			GObj_2DVertex operator<<(GObj_2DVertex &a, angle_t rotation); // Rotate amount anti-clockwise
			GObj_2DVertex operator>>(GObj_2DVertex &a, angle_t rotation); // Rotate amount clockwise

			/*
			* Polygon manipulation
			*/

			// Polygon creation and deletion
			GObj_2DPolygon createPolygon(ResourceManager &allocator, size_t dimCount);
			void destroyPolygon(GObj_2DPolygon &polygon, ResourceManager &allocator);

			// Polygon builder
			index_t appendVertex(index_t at, GObj_2DPolygon &);
			index_t appendVertex(index_t at, dim_t x, dim_t y);

			// 





		}
	}
}


#endif
