#include "BoundingAABB.h"
#include <assert.h>

namespace BoundingVolumes
{

bool BoundingAABB::IsVisibleFromFrustum(const MathLib::plane frustum[]) const
{
	MathLib::vector4 vertices[8];

	// Top right front vertex.
	vertices[0].setXYZW(maxExtents.extractX(), maxExtents.extractY(), maxExtents.extractZ(), 1.0f);

	// Bottom right front vertex.
	vertices[1].setXYZW(maxExtents.extractX(), minExtents.extractY(), maxExtents.extractZ(), 1.0f);

	// Bottom left front vertex.
	vertices[2].setXYZW(minExtents.extractX(), minExtents.extractY(), maxExtents.extractZ(), 1.0f);

	// Top left front vertex.
	vertices[3].setXYZW(minExtents.extractX(), maxExtents.extractY(), maxExtents.extractZ(), 1.0f);

	// Top right back vertex.
	vertices[4].setXYZW(maxExtents.extractX(), maxExtents.extractY(), minExtents.extractZ(), 1.0f);

	// Bottom right back vertex.
	vertices[5].setXYZW(maxExtents.extractX(), minExtents.extractY(), minExtents.extractZ(), 1.0f);

	// Bottom left back vertex.
	vertices[6].setXYZW(minExtents.extractX(), minExtents.extractY(), minExtents.extractZ(), 1.0f);

	// Top left back vertex.
	vertices[7].setXYZW(minExtents.extractX(), maxExtents.extractY(), minExtents.extractZ(), 1.0f);

	for (unsigned int i = 0; i < 6; i++)
	{
		bool vertexVisible[8];

		const MathLib::vector4& pointOnPlane = frustum[i].getPointOnPlane();
		const MathLib::vector4& planeNormal = frustum[i].getNormal();

		MathLib::vector4 pointInRelationToPlane;
		float projection;

		// Test vertices against plane.
		for (unsigned int j = 0; j < 8; j++)
		{
			vertexVisible[j] = false;

			MathLib::vector4_sub(vertices[j], pointOnPlane, pointInRelationToPlane);
			projection = MathLib::vector4_dotProduct(pointInRelationToPlane, planeNormal);
			if (projection >= 0.0f)
			{
				vertexVisible[j] = true;
			}
		}

		bool anyVerticesVisible = false;
		for (int j = 0; j < 8 && !anyVerticesVisible; j++)
		{
			if (vertexVisible[j])
			{
				anyVerticesVisible = true;
			}
		}

		if (!anyVerticesVisible)
		{
			return false;
		}
	}

	return true;
}

bool BoundingAABB::IntersectsWithRay(const MathLib::ray& intersectRay, float* t) const
{
	const MathLib::vector4& rayDirection = intersectRay.getDirection();
	const MathLib::vector4& rayPosition = intersectRay.getPosition();

	MathLib::plane planes[3];
	unsigned int numPlanes = 0;

	// Get which planes we have to test.
	if (rayDirection.extractX() > 0.0f)
	{
		planes[numPlanes].setNormal(MathLib::vector4(-1.0f, 0.0f, 0.0f, 0.0f));
		planes[numPlanes].setPointOnPlane(minExtents);
		numPlanes++;
	}
	else if (rayDirection.extractX() < 0.0f)
	{
		planes[numPlanes].setNormal(MathLib::vector4(1.0f, 0.0f, 0.0f, 0.0f));
		planes[numPlanes].setPointOnPlane(maxExtents);
		numPlanes++;
	}

	if (rayDirection.extractY() > 0.0f)
	{
		planes[numPlanes].setNormal(MathLib::vector4(0.0f, -1.0f, 0.0f, 0.0f));
		planes[numPlanes].setPointOnPlane(minExtents);
		numPlanes++;
	}
	else if (rayDirection.extractY() < 0.0f)
	{
		planes[numPlanes].setNormal(MathLib::vector4(0.0f, 1.0f, 0.0f, 0.0f));
		planes[numPlanes].setPointOnPlane(maxExtents);
		numPlanes++;
	}

	if (rayDirection.extractZ() > 0.0f)
	{
		planes[numPlanes].setNormal(MathLib::vector4(0.0f, 0.0f, -1.0f, 0.0f));
		planes[numPlanes].setPointOnPlane(minExtents);
		numPlanes++;
	}
	else if (rayDirection.extractZ() < 0.0f)
	{
		planes[numPlanes].setNormal(MathLib::vector4(0.0f, 0.0f, 1.0f, 0.0f));
		planes[numPlanes].setPointOnPlane(maxExtents);
		numPlanes++;
	}

	for (unsigned int i = 0; i < numPlanes; i++)
	{
		MathLib::vector4 intersectionPoint;
		if (MathLib::intersectRayWithPlane(intersectRay, planes[i], t))
		{
			if (*t < 0.0f)
			{
				continue;
			}

			MathLib::vector4_copy(intersectionPoint, rayPosition);
			MathLib::vector4_addScaledVector(intersectionPoint, rayDirection, *t, intersectionPoint);
			MathLib::vector4_setToPoint(intersectionPoint);

			float intersectionPointX = intersectionPoint.extractX();
			float intersectionPointY = intersectionPoint.extractY();
			float intersectionPointZ = intersectionPoint.extractZ();

			const float epsilon = 0.10f;

			if (intersectionPointX >= (minExtents.extractX() - epsilon) &&
				intersectionPointX <= (maxExtents.extractX() + epsilon) &&
				intersectionPointY >= (minExtents.extractY() - epsilon) &&
				intersectionPointY <= (maxExtents.extractY() + epsilon) &&
				intersectionPointZ >= (minExtents.extractZ() - epsilon) &&
				intersectionPointZ <= (maxExtents.extractZ() + epsilon))
			{
				return true;
			}
		}
	}

	return false;
}

}
