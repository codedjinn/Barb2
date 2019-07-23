#ifndef RAYAABB_H_INCLUDED
#define RAYAABB_H_INCLUDED

#include <MathLib.h>

namespace GeometryLib
{

bool RayIntersectsAABB(const MathLib::ray& ray,
					   float* t,
					   MathLib::vector4 const* aabbVertices);

}

#endif // RAYAABB_H_INCLUDED
