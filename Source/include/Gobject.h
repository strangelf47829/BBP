#ifndef BBP_GLIB_GOBJECT_H
#define BBP_GLIB_GOBJECT_H

#include "stddef.h"
#include "Memory.h"
#include "Pixels.h"
#include "Vmem.h"

namespace BBP
{
	namespace std
	{
		namespace R2D
		{

			// The type used to store positional information
			using dim_t = int32_t;

			// The type used to represent magnitudes
			using mag_t = float;

			// The type used to represent angles (in degrees)
			using angle_t = int16_t;

			// This structure holds information about a 2d vertex
			struct GObj_2DVertex
			{
				dim_t x;
				dim_t h;
			};

			// This structure holds information about a 2d vertex in a linked-list manner.
			struct GObj_2DVertex_linked
			{
				GObj_2DVertex vertex;
				GObj_2DVertex_linked *next;
			};

			// This structure holds information about an n dimensional thing.
			template<int N>
			struct GObj_StaticPolygon
			{
				// Static vertex information
				STATIC_PAGE<GObj_2DVertex, N> verticies;

				// Static order information
				STATIC_PAGE<index_t, N> order;
			};

			// This structure holds information about a 2d polygon 
			struct GObj_2DPolygon
			{
				// The verticies themselves
				PAGE<GObj_2DVertex> verticies;
				
				// The order in which they appear in
				PAGE<index_t> order;
			};

			// Useful for declaring stuff
			using Line_t = GObj_StaticPolygon<2>;
			using Triangle_t = GObj_StaticPolygon<3>;
			using Quad_t = GObj_StaticPolygon<4>;

			// This structure holds information about a 2d polygon using linked verticies
			struct GObj_2DPolygon_linked
			{
				// The first vertex
				GObj_2DVertex_linked head;

				// The amount of verticies
				size_t vertexCount;
			};

			// Structure used to store information about a single fill-surface.
			// A fill surface is a surface which can quickly be filled in
			struct GObj_2DFillRegion
			{
				// Three points to fill stuff with
				GObj_2DVertex *a;
				GObj_2DVertex *b;
				GObj_2DVertex *c;
			};

			// Structure used to store information about the polygon surface.
			struct GObj_2DFillInfo
			{
				// A page containing these fill regions
				PAGE<GObj_2DFillRegion> regions;
			};
			
			// Rotation information (in whole degrees)
			struct GObj_2DRotation
			{
				// Anchor point
				GObj_2DVertex anchor;

				// The amount of rotation (in degrees)
				angle_t rotation;
			};

			// Translation information
			struct GObj_2DTranslation
			{
				// Coordinates
				GObj_2DVertex position;
			};

			// Transformation information
			struct GObj_2DTransform
			{
				// The rotation
				GObj_2DRotation rotation;

				// The translation
				GObj_2DTranslation translation;
			};

			// An element is a bit of information about ... 
			struct GObj_2DElement
			{
				// Colour
				pixel_t pixelInformation;

				// Size information
				size_t sizeInformation;
			};

			// A shape has at least a polygon, fill information, and a transform
			struct GObj_2DShape
			{
				// The polygon making up the shape
				GObj_2DPolygon polygon;

				// The fill info for this shape
				GObj_2DFillInfo fillInformation;

				// The transform of this shape
				GObj_2DTransform transform;

			};

			struct GObj_2DColouredShape
			{
				// The shape to draw
				GObj_2DShape *shape;

				// The infil for this shape
				GObj_2DElement Fill;

				// The stroke for this shape
				GObj_2DElement Stroke;
			};

			// A shallow object does not need to be copied over and over again
			struct GObj_2DShallowShape
			{
				// Reference to shape
				GObj_2DShape *shape;

				// Transform of this shape (not shallow)
				GObj_2DTransform transform;
			};

			// A textured shape is a structure that holds information about a shape and its texture.
			// This is pretty much everything that is needed to perform 
			struct GObj_2DTexturedShape
			{
				// The shape
				GObj_2DShape *shape;

				// Texture information
				Vmem *texture;

				// Where to draw this texture
				Vmem target;
			};


		}
	}
}


#endif
