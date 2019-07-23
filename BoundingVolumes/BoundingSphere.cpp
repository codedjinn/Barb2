#include "BoundingSphere.h"
#include <assert.h>
#include <stdlib.h>

namespace BoundingVolumes
{

BoundingSphere::BoundingSphere(ObjectSpaceMeshData const * objectSpaceMeshData) :
	BoundingVolumeObjectSpace(objectSpaceMeshData)
{
	bv_type = BOUNDING_VOLUME_SPHERE;
}

BoundingSphere::BoundingSphere(const MathLib::vector4& position, const float radius) : 
	BoundingVolumeObjectSpace(NULL)
{
	bv_type = BOUNDING_VOLUME_SPHERE;

	MathLib::vector4_copy(worldSpacePosition, position);
	worldSpaceRadius = radius;
}

void BoundingSphere::GenerateInstanceBounds(const MathLib::vector4& position,
											const MathLib::vector4& rotation,
											const MathLib::vector4& scale, 
											const MathLib::matrix4x4& parentTransform)
{
	// This method is actually broken and needs to be refactored.
	float maxRadiusScale = scale.extractX();
	if (scale.extractY() > scale.extractX())
		maxRadiusScale = scale.extractY();
	if (scale.extractZ() > maxRadiusScale)
		maxRadiusScale	= scale.extractZ();

	worldSpaceRadius = objectSpaceMeshData -> radius * maxRadiusScale;

	MathLib::vector4_copy(worldSpacePosition, position);

	MathLib::matrix4x4_vectorMul(parentTransform, worldSpacePosition, worldSpacePosition);
}

void BoundingSphere::GenerateInstanceBounds(const MathLib::vector4& position,
											const MathLib::quaternion& quaternion,
											float scale, 
											const MathLib::matrix4x4& parentTransform)
{
	// This method is actually broken and needs to be refactored.
	worldSpaceRadius = objectSpaceMeshData->radius * scale;

	MathLib::vector4_copy(worldSpacePosition, position);
	
	MathLib::matrix4x4_vectorMul(parentTransform, worldSpacePosition, worldSpacePosition);
}

bool BoundingSphere::IntersectsWithRay(const MathLib::ray& intersectRay, float* t) const
{
	MathLib::vector4 tempVector;

	// First determine if the eye point is inside the sphere, if so, then return false.
	float distance = MathLib::vector4_distance(intersectRay.getPosition(), worldSpacePosition);
	if (distance <= worldSpaceRadius)
	{
		return false;
	}

	// Calculate coefficients for the quadratic equation.
	float a = MathLib::vector4_dotProduct(intersectRay.getDirection(), intersectRay.getDirection());

	MathLib::vector4_sub(intersectRay.getPosition(), worldSpacePosition, tempVector);

	float b = 2.0f * MathLib::vector4_dotProduct(intersectRay.getDirection(), tempVector);

	float c = (tempVector.extractX() * tempVector.extractX()) +
			  (tempVector.extractY() * tempVector.extractY()) +
			  (tempVector.extractZ() * tempVector.extractZ()) -
			  (worldSpaceRadius * worldSpaceRadius);

	float discriminant = (b * b) - 4.0f * a * c;

	if (discriminant < 0.0f)
	{
		// No solution.
		return false;
	}
	else if (discriminant > 0.0f)
	{
		// Two solutions, pick the solution with the smallest t > 1.
		float t0 = (-b + sqrtf(discriminant)) / (2.0f * a);
		float t1 = (-b - sqrtf(discriminant)) / (2.0f * a);

		if (t0 < 1.0f)
		{
			*t = t1;
		}
		else if (t1 < 1.0f)
		{
			*t = t0;
		}
		else
		{
			// Both are greater than 1.0f.
			// Choose the smaller t.
			if (t0 < t1)
			{
				*t = t0;
			}
			else
			{
				*t	= t1;
			}
		}

		return true;
	}
	else
	{
		*t = (-b - sqrtf(discriminant)) / (2.0f * a);

		return true;
	}
}

bool BoundingSphere::IsVisibleFromFrustum(const MathLib::plane frustum[]) const
{
	return false;
}

}
