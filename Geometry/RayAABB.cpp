#include "RayAABB.h"
#include <CommonDefines.h>

namespace GeometryLib
{

bool RayIntersectsAABB(const MathLib::ray& ray,
					   float* t,
					   MathLib::vector4 const* aabbVertices)
{
	// Intersect with front plane.
	// MathLib::plane frontPlane(MathLib::vector4(aabbVertices[TERRAIN_CD
	// Test against front plane.
	const MathLib::vector4& rayPosition = ray.getPosition();
	const MathLib::vector4& rayDirection = ray.getDirection();

	// Containment test. Is the ray inside the aabb?
	if (rayPosition.extractX() >= aabbVertices[AABB_EXTENTS_MIN].extractX() &&
		rayPosition.extractX() <= aabbVertices[AABB_EXTENTS_MAX].extractX() &&
		rayPosition.extractY() >= aabbVertices[AABB_EXTENTS_MIN].extractY() &&
		rayPosition.extractY() <= aabbVertices[AABB_EXTENTS_MAX].extractY() &&
		rayPosition.extractZ() >= aabbVertices[AABB_EXTENTS_MIN].extractZ() &&
		rayPosition.extractZ() <= aabbVertices[AABB_EXTENTS_MAX].extractZ())
	{
		*t = 0.0f;

		return true;
	}

	bool tFound = false;
	float minimumTValue = FLT_MAX;

	// Intersect ray with the minimum x plane of the aabb.
	MathLib::plane minXPlane(MathLib::vector4(aabbVertices[AABB_EXTENTS_MIN].extractX(), 0.0f, 0.0f, 1.0f),
							 MathLib::vector4(-1.0f, 0.0f, 0.0f, 0.0f));

	float minXPlaneIntersection;
	if (MathLib::intersectRayWithPlane(ray, minXPlane, &minXPlaneIntersection))
	{
		if (minXPlaneIntersection > 0.0f)
		{
			// Construct the point on the plane. Test if in bounds of the aabb.
			MathLib::vector4 point;
			MathLib::vector4_addScaledVector(rayPosition, rayDirection, minXPlaneIntersection, point);

			if (point.extractY() >= aabbVertices[AABB_EXTENTS_MIN].extractY() &&
				point.extractY() <= aabbVertices[AABB_EXTENTS_MAX].extractY() &&
				point.extractZ() >= aabbVertices[AABB_EXTENTS_MIN].extractZ() &&
				point.extractZ() <= aabbVertices[AABB_EXTENTS_MAX].extractZ())
			{
				// Finally if the t value is smaller than the smallest t value,
				// set new t value.
				if (minXPlaneIntersection < minimumTValue)
				{
					minimumTValue = minXPlaneIntersection;
					tFound = true;
				}
			}
		}
	}

	// Intersect ray with the maximum x plane of the aabb.
	MathLib::plane maxXPlane(MathLib::vector4(aabbVertices[AABB_EXTENTS_MAX].extractX(), 0.0f, 0.0f, 1.0f),
							 MathLib::vector4(1.0f, 0.0f, 0.0f, 0.0f));

	float maxXPlaneIntersection;
	if (MathLib::intersectRayWithPlane(ray, maxXPlane, &maxXPlaneIntersection))
	{
		if (maxXPlaneIntersection > 0.0f)
		{
			// Construct the point on the plane. Test if in bounds of the aabb.
			MathLib::vector4 point;
			MathLib::vector4_addScaledVector(rayPosition, rayDirection, maxXPlaneIntersection, point);

			if (point.extractY() >= aabbVertices[AABB_EXTENTS_MIN].extractY() &&
				point.extractY() <= aabbVertices[AABB_EXTENTS_MAX].extractY() &&
				point.extractZ() >= aabbVertices[AABB_EXTENTS_MIN].extractZ() &&
				point.extractZ() <= aabbVertices[AABB_EXTENTS_MAX].extractZ())
			{
				// Finally if the t value is smaller than the smallest t value,
				// set new t value.
				if (maxXPlaneIntersection < minimumTValue)
				{
					minimumTValue = maxXPlaneIntersection;
					tFound = true;
				}
			}
		}
	}

	// Intersect ray with the minimum y plane of the aabb.
	MathLib::plane minYPlane(MathLib::vector4(0.0f, aabbVertices[AABB_EXTENTS_MIN].extractY(), 0.0f, 1.0f),
							 MathLib::vector4(0.0f, -1.0f, 0.0f, 0.0f));

	float minYPlaneIntersection;
	if (MathLib::intersectRayWithPlane(ray, minYPlane, &minYPlaneIntersection))
	{
		if (minYPlaneIntersection > 0.0f)
		{
			// Construct the point on the plane. Test if in bounds of the aabb.
			MathLib::vector4 point;
			MathLib::vector4_addScaledVector(rayPosition, rayDirection, minYPlaneIntersection, point);

			if (point.extractX() >= aabbVertices[AABB_EXTENTS_MIN].extractX() &&
				point.extractX() <= aabbVertices[AABB_EXTENTS_MAX].extractX() &&
				point.extractZ() >= aabbVertices[AABB_EXTENTS_MIN].extractZ() &&
				point.extractZ() <= aabbVertices[AABB_EXTENTS_MAX].extractZ())
			{
				// Finally if the t value is smaller than the smallest t value,
				// set new t value.
				if (minYPlaneIntersection < minimumTValue)
				{
					minimumTValue = minYPlaneIntersection;
					tFound = true;
				}
			}
		}
	}

	// Intersect ray with the maximum y plane of the aabb.
	MathLib::plane maxYPlane(MathLib::vector4(0.0f, aabbVertices[AABB_EXTENTS_MAX].extractY(), 0.0f, 1.0f),
							 MathLib::vector4(0.0f, 1.0f, 0.0f, 0.0f));

	float maxYPlaneIntersection;
	if (MathLib::intersectRayWithPlane(ray, maxYPlane, &maxYPlaneIntersection))
	{
		if (maxYPlaneIntersection> 0.0f)
		{
			// Construct the point on the plane. Test if in bounds of the aabb.
			MathLib::vector4 point;
			MathLib::vector4_addScaledVector(rayPosition, rayDirection, maxYPlaneIntersection, point);

			if (point.extractX() >= aabbVertices[AABB_EXTENTS_MIN].extractX() &&
				point.extractX() <= aabbVertices[AABB_EXTENTS_MAX].extractX() &&
				point.extractZ() >= aabbVertices[AABB_EXTENTS_MIN].extractZ() &&
				point.extractZ() <= aabbVertices[AABB_EXTENTS_MAX].extractZ())
			{
				// Finally if the t value is smaller than the smallest t value,
				// set new t value.
				if (maxYPlaneIntersection < minimumTValue)
				{
					minimumTValue = maxYPlaneIntersection;
					tFound = true;
				}
			}
		}
	}

	// Intersect ray with the minimum z plane of the aabb.
	MathLib::plane minZPlane(MathLib::vector4(0.0f, 0.0f, aabbVertices[AABB_EXTENTS_MIN].extractZ(), 1.0f),
							 MathLib::vector4(0.0f, 0.0f, -1.0f, 0.0f));

	float minZPlaneIntersection;
	if (MathLib::intersectRayWithPlane(ray, minZPlane, &minZPlaneIntersection))
	{
		if (minZPlaneIntersection > 0.0f)
		{
			// Construct the point on the plane. Test if in bounds of the aabb.
			MathLib::vector4 point;
			MathLib::vector4_addScaledVector(rayPosition, rayDirection, minZPlaneIntersection, point);

			if (point.extractX() >= aabbVertices[AABB_EXTENTS_MIN].extractX() &&
				point.extractX() <= aabbVertices[AABB_EXTENTS_MAX].extractX() &&
				point.extractY() >= aabbVertices[AABB_EXTENTS_MIN].extractY() &&
				point.extractY() <= aabbVertices[AABB_EXTENTS_MAX].extractY())
			{
				// Finally if the t value is smaller than the smallest t value,
				// set new t value.
				if (minZPlaneIntersection < minimumTValue)
				{
					minimumTValue = minZPlaneIntersection;
					tFound = true;
				}
			}
		}
	}

	// Intersect ray with the maximum z plane of the aabb.
	MathLib::plane maxZPlane(MathLib::vector4(0.0f, 0.0f, aabbVertices[AABB_EXTENTS_MAX].extractZ(), 1.0f),
							 MathLib::vector4(0.0f, 0.0f, 1.0f, 0.0f));

	float maxZPlaneIntersection;
	if (MathLib::intersectRayWithPlane(ray, maxZPlane, &maxZPlaneIntersection))
	{
		if (maxZPlaneIntersection > 0.0f)
		{
			// Construct the point on the plane. Test if in bounds of the aabb.
			MathLib::vector4 point;
			MathLib::vector4_addScaledVector(rayPosition, rayDirection, maxZPlaneIntersection, point);

			if (point.extractX() >= aabbVertices[AABB_EXTENTS_MIN].extractX() &&
				point.extractX() <= aabbVertices[AABB_EXTENTS_MAX].extractX() &&
				point.extractY() >= aabbVertices[AABB_EXTENTS_MIN].extractY() &&
				point.extractY() <= aabbVertices[AABB_EXTENTS_MAX].extractY())
			{
				// Finally if the t value is smaller than the smallest t value,
				// set new t value.
				if (maxZPlaneIntersection < minimumTValue)
				{
					minimumTValue = maxZPlaneIntersection;
					tFound = true;
				}
			}
		}
	}

	if (tFound)
	{
		*t = minimumTValue;
	}

	return tFound;
}

}
