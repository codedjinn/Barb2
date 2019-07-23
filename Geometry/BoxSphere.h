#ifndef BOXSPHERE_H_INCLUDED
#define BOXSPHERE_H_INCLUDED

#include "GeometryLibCommon.h"
#include <MathLib.h>

namespace GeometryLib
{

bool SphereInsideBox(const MathLib::vector4& spherePosition, float sphereRadius, const MathLib::plane boxPlanes[]);

bool SphereBoxIntersection(const MathLib::vector4& spherePosition, float sphereRadius, const MathLib::plane boxPlanes[]);

bool BoxInsideSphere(const MathLib::vector4& spherePosition, float sphereRadius, const MathLib::vector4 boxVertices[]);

}


#endif // BOXSPHERE_H_INCLUDED
