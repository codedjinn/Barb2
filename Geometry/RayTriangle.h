#ifndef RAYTRIANGLE_H_INCLUDED
#define RAYTRIANGLE_H_INCLUDED

#include <MathLib.h>

namespace GeometryLib
{

/**
* Intersects a ray with a triangle.
* @note 		: The triangle points must be in CLOCKWISE order.
* @returns		: true if intersecting, false if not.
*/
bool RayIntersectsTriangle(const MathLib::ray& ray,
						   float* t,
						   const MathLib::vector4& point0,
						   const MathLib::vector4& point1,
						   const MathLib::vector4& point2);
}


#endif // RAYTRIANGLE_H_INCLUDED
