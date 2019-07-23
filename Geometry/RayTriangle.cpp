#include "RayTriangle.h"
#include <iostream>

namespace GeometryLib
{

bool RayIntersectsTriangle(const MathLib::ray& ray,
						   float* t,
						   const MathLib::vector4& point0,
						   const MathLib::vector4& point1,
						   const MathLib::vector4& point2)
{
	// Construct plane that is associated with the triangle.
	MathLib::plane trianglePlane;
	trianglePlane.setPointOnPlane(point0);

	MathLib::vector4 p02;
	MathLib::vector4 p01;
	MathLib::vector4 planeNormal;

	MathLib::vector4_sub(point2, point0, p02);
	MathLib::vector4_sub(point1, point0, p01);
	MathLib::vector4_crossProduct(p02, p01, planeNormal);
	MathLib::vector4_normalize(planeNormal);
	trianglePlane.setNormal(planeNormal);

	// Intersect ray with the plane, calculate intersection point.
	float planeT;
	if (!MathLib::intersectRayWithPlane(ray, trianglePlane, &planeT))
	{
		return false;
	}

	MathLib::vector4 intersectionPoint;
	MathLib::vector4_addScaledVector(ray.getPosition(), ray.getDirection(), planeT, intersectionPoint);

	// Now we need to perform half space tests on the plane.
	MathLib::plane halfSpace0;
	halfSpace0.setPointOnPlane(point0);

	MathLib::vector4 halfSpace0Normal;
	MathLib::vector4_crossProduct(p01, planeNormal, halfSpace0Normal);
	MathLib::vector4_normalize(halfSpace0Normal);
	halfSpace0.setNormal(halfSpace0Normal);

	MathLib::plane halfSpace1;
	halfSpace1.setPointOnPlane(point1);

	MathLib::vector4 p12;
	MathLib::vector4_sub(point2, point1, p12);
	MathLib::vector4 halfSpace1Normal;
	MathLib::vector4_crossProduct(p12, planeNormal, halfSpace1Normal);
	MathLib::vector4_normalize(halfSpace1Normal);
	halfSpace1.setNormal(halfSpace1Normal);

	MathLib::plane halfSpace2;
	halfSpace2.setPointOnPlane(point2);

	MathLib::vector4 p20;
	MathLib::vector4_sub(point0, point2, p20);
	MathLib::vector4 halfSpace2Normal;
	MathLib::vector4_crossProduct(p20, planeNormal, halfSpace2Normal);
	MathLib::vector4_normalize(halfSpace2Normal);
	halfSpace2.setNormal(halfSpace2Normal);

	if (MathLib::pointOnPositivePlaneSide(halfSpace0, intersectionPoint) &&
		MathLib::pointOnPositivePlaneSide(halfSpace1, intersectionPoint) &&
		MathLib::pointOnPositivePlaneSide(halfSpace2, intersectionPoint))
	{
		*t = planeT;

		return true;
	}

	return false;
}

}
