#include "BoxSphere.h"

namespace GeometryLib
{

bool SphereInsideBox(const MathLib::vector4& spherePosition, float sphereRadius, const MathLib::plane boxPlanes[])
{
	for (unsigned int i = 0; i < 6; i++)
	{
		if (!MathLib::sphereInsidePlane(spherePosition, sphereRadius, boxPlanes[i]))
		{
			return false;
		}
	}
	return true;
}

bool SphereBoxIntersection(const MathLib::vector4& spherePosition, float sphereRadius, const MathLib::plane boxPlanes[])
{
	bool inLeft		= !MathLib::sphereOutsidePlane(spherePosition, sphereRadius, boxPlanes[BOX_FACES_LEFT]);
	bool inRight	= !MathLib::sphereOutsidePlane(spherePosition, sphereRadius, boxPlanes[BOX_FACES_RIGHT]);
	bool inTop		= !MathLib::sphereOutsidePlane(spherePosition, sphereRadius, boxPlanes[BOX_FACES_TOP]);
	bool inBottom	= !MathLib::sphereOutsidePlane(spherePosition, sphereRadius, boxPlanes[BOX_FACES_BOTTOM]);
	bool inFront	= !MathLib::sphereOutsidePlane(spherePosition, sphereRadius, boxPlanes[BOX_FACES_FRONT]);
	bool inBack		= !MathLib::sphereOutsidePlane(spherePosition, sphereRadius, boxPlanes[BOX_FACES_BACK]);

	if (MathLib::sphereIntersectsPlane(spherePosition, sphereRadius, boxPlanes[BOX_FACES_TOP]) &&
		inLeft && inRight && inFront && inBack)
	{
		return true;
	}

	if (MathLib::sphereIntersectsPlane(spherePosition, sphereRadius, boxPlanes[BOX_FACES_BOTTOM]) &&
		inLeft && inRight && inFront && inBack)
	{
		return true;
	}

	if (MathLib::sphereIntersectsPlane(spherePosition, sphereRadius, boxPlanes[BOX_FACES_LEFT]) &&
		inTop && inBottom && inFront && inBack)
	{
		return true;
	}

	if (MathLib::sphereIntersectsPlane(spherePosition, sphereRadius, boxPlanes[BOX_FACES_RIGHT]) &&
		inTop && inBottom && inFront && inBack)
	{
		return true;
	}

	if (MathLib::sphereIntersectsPlane(spherePosition, sphereRadius, boxPlanes[BOX_FACES_FRONT]) &&
		inTop && inBottom && inLeft && inRight)
	{
		return true;
	}

	if (MathLib::sphereIntersectsPlane(spherePosition, sphereRadius, boxPlanes[BOX_FACES_BACK]) &&
		inTop && inBottom && inLeft && inRight)
	{
		return true;
	}

	return false;
}

bool SphereOutsideBox(const MathLib::vector4& spherePosition, float sphereRadius, const MathLib::plane boxPlanes[])
{
	return !(SphereInsideBox(spherePosition, sphereRadius, boxPlanes) || SphereBoxIntersection(spherePosition, sphereRadius, boxPlanes));
}

bool BoxInsideSphere(const MathLib::vector4& spherePosition, float sphereRadius, const MathLib::vector4 boxVertices[])
{
	for (unsigned int i = 0; i < 8; i++)
	{
		if (MathLib::vector4_distance(spherePosition, boxVertices[i]) > sphereRadius)
		{
			return false;
		}
	}
	return true;
}

}

