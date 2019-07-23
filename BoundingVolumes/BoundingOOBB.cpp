#include "BoundingOOBB.h"
#include <assert.h>
#include <iostream>
#include <limits>

using namespace BoundingVolumes;

void BoundingOOBB::GenerateInstanceBounds(const MathLib::vector4& position,
										  const MathLib::vector4& rotation,
										  const MathLib::vector4& scale, 
										  const MathLib::matrix4x4& parentTransform)
{
	float objectSpaceMinExtents_x = objectSpaceMeshData->oobb_minExtents.extractX();
	float objectSpaceMinExtents_y = objectSpaceMeshData->oobb_minExtents.extractY();
	float objectSpaceMinExtents_z = objectSpaceMeshData->oobb_minExtents.extractZ();

	float objectSpaceMaxExtents_x = objectSpaceMeshData->oobb_maxExtents.extractX();
	float objectSpaceMaxExtents_y = objectSpaceMeshData->oobb_maxExtents.extractY();
	float objectSpaceMaxExtents_z = objectSpaceMeshData->oobb_maxExtents.extractZ();

	// Construct object space bounding vertices.
	worldSpace_FTL.setXYZW(objectSpaceMinExtents_x, objectSpaceMaxExtents_y, objectSpaceMaxExtents_z, 1.0f);
	worldSpace_FTR.setXYZW(objectSpaceMaxExtents_x, objectSpaceMaxExtents_y, objectSpaceMaxExtents_z, 1.0f);
	worldSpace_FBR.setXYZW(objectSpaceMaxExtents_x, objectSpaceMinExtents_y, objectSpaceMaxExtents_z, 1.0f);
	worldSpace_FBL.setXYZW(objectSpaceMinExtents_x, objectSpaceMinExtents_y, objectSpaceMaxExtents_z, 1.0f);

	worldSpace_BTL.setXYZW(objectSpaceMinExtents_x, objectSpaceMaxExtents_y, objectSpaceMinExtents_z, 1.0f);
	worldSpace_BTR.setXYZW(objectSpaceMaxExtents_x, objectSpaceMaxExtents_y, objectSpaceMinExtents_z, 1.0f);
	worldSpace_BBR.setXYZW(objectSpaceMaxExtents_x, objectSpaceMinExtents_y, objectSpaceMinExtents_z, 1.0f);
	worldSpace_BBL.setXYZW(objectSpaceMinExtents_x, objectSpaceMinExtents_y, objectSpaceMinExtents_z, 1.0f);

	MathLib::matrix4x4 objectTransform;
	MathLib::GenerateTransformMatrix(objectTransform, position, rotation, scale);

	MathLib::matrix4x4_mul(parentTransform, objectTransform, transform);

	MathLib::matrix4x4_inverse(transform, invTransform);

	// Transform to worldspace.
	MathLib::matrix4x4_vectorMul(transform, worldSpace_FTL, worldSpace_FTL);
	MathLib::matrix4x4_vectorMul(transform, worldSpace_FTR, worldSpace_FTR);
	MathLib::matrix4x4_vectorMul(transform, worldSpace_FBR, worldSpace_FBR);
	MathLib::matrix4x4_vectorMul(transform, worldSpace_FBL, worldSpace_FBL);

	MathLib::matrix4x4_vectorMul(transform, worldSpace_BTL, worldSpace_BTL);
	MathLib::matrix4x4_vectorMul(transform, worldSpace_BTR, worldSpace_BTR);
	MathLib::matrix4x4_vectorMul(transform, worldSpace_BBR, worldSpace_BBR);
	MathLib::matrix4x4_vectorMul(transform, worldSpace_BBL, worldSpace_BBL);
}

void BoundingOOBB::GenerateInstanceBounds(const MathLib::vector4& position,
                                          const MathLib::quaternion& orientation,
                                          const float scale, 
										  const MathLib::matrix4x4& parentTransform)
{
    float objectSpaceMinExtents_x = objectSpaceMeshData->oobb_minExtents.extractX();
    float objectSpaceMinExtents_y = objectSpaceMeshData->oobb_minExtents.extractY();
    float objectSpaceMinExtents_z = objectSpaceMeshData->oobb_minExtents.extractZ();

    float objectSpaceMaxExtents_x = objectSpaceMeshData->oobb_maxExtents.extractX();
    float objectSpaceMaxExtents_y = objectSpaceMeshData->oobb_maxExtents.extractY();
    float objectSpaceMaxExtents_z = objectSpaceMeshData->oobb_maxExtents.extractZ();

    // Constructing object space bounding vertices.
    worldSpace_FTL.setXYZW(objectSpaceMinExtents_x, objectSpaceMaxExtents_y, objectSpaceMaxExtents_z, 1.0f);
	worldSpace_FTR.setXYZW(objectSpaceMaxExtents_x, objectSpaceMaxExtents_y, objectSpaceMaxExtents_z, 1.0f);
	worldSpace_FBR.setXYZW(objectSpaceMaxExtents_x, objectSpaceMinExtents_y, objectSpaceMaxExtents_z, 1.0f);
	worldSpace_FBL.setXYZW(objectSpaceMinExtents_x, objectSpaceMinExtents_y, objectSpaceMaxExtents_z, 1.0f);

	worldSpace_BTL.setXYZW(objectSpaceMinExtents_x, objectSpaceMaxExtents_y, objectSpaceMinExtents_z, 1.0f);
	worldSpace_BTR.setXYZW(objectSpaceMaxExtents_x, objectSpaceMaxExtents_y, objectSpaceMinExtents_z, 1.0f);
	worldSpace_BBR.setXYZW(objectSpaceMaxExtents_x, objectSpaceMinExtents_y, objectSpaceMinExtents_z, 1.0f);
	worldSpace_BBL.setXYZW(objectSpaceMinExtents_x, objectSpaceMinExtents_y, objectSpaceMinExtents_z, 1.0f);

	MathLib::matrix4x4 objectTransform;
    MathLib::GenerateTransformMatrix(objectTransform, position, orientation, scale);

	MathLib::matrix4x4_mul(parentTransform, objectTransform, transform);

	MathLib::matrix4x4_inverse(transform, invTransform);

	// Transform to worldspace.
	MathLib::matrix4x4_vectorMul(transform, worldSpace_FTL, worldSpace_FTL);
	MathLib::matrix4x4_vectorMul(transform, worldSpace_FTR, worldSpace_FTR);
	MathLib::matrix4x4_vectorMul(transform, worldSpace_FBR, worldSpace_FBR);
	MathLib::matrix4x4_vectorMul(transform, worldSpace_FBL, worldSpace_FBL);

	MathLib::matrix4x4_vectorMul(transform, worldSpace_BTL, worldSpace_BTL);
	MathLib::matrix4x4_vectorMul(transform, worldSpace_BTR, worldSpace_BTR);
	MathLib::matrix4x4_vectorMul(transform, worldSpace_BBR, worldSpace_BBR);
	MathLib::matrix4x4_vectorMul(transform, worldSpace_BBL, worldSpace_BBL);
}

bool BoundingOOBB::IntersectsWithRay(const MathLib::ray& intersectRay, float* t) const
{
    MathLib::vector4 rayPosition_objectSpace;
    MathLib::vector4 rayDirection_objectSpace;

    // Since our object to world space transformations may be non-orthogonal. We have to ensure that
    // we transform normal vectors properly.
    MathLib::matrix4x4 covariantNormalTransform(transform);
    MathLib::matrix4x4_transpose(covariantNormalTransform);

    // BUG HERE WITH NONUNIFORM SCALING !!! D:

    // Get the ray in object space.
    // Transform ray position.
    MathLib::matrix4x4_vectorMul(invTransform, intersectRay.getPosition(), rayPosition_objectSpace);

    // Transform (and renormalize) ray direction.
    MathLib::matrix4x4_vectorMul(covariantNormalTransform, intersectRay.getDirection(), rayDirection_objectSpace);
    MathLib::vector4_normalize(rayDirection_objectSpace);

    // Now that we have the ray in object space, we can perform the intersection test.
    MathLib::plane planes[3];
    unsigned int numPlanes = 0;

    // Get which planes we have to test:
    if (rayDirection_objectSpace.extractX() > 0.0f)
    {
        planes[numPlanes].setNormal(MathLib::vector4(-1.0f, 0.0f, 0.0f, 0.0f));
        planes[numPlanes].setPointOnPlane(objectSpaceMeshData->oobb_minExtents);
        numPlanes++;
    }
    else if (rayDirection_objectSpace.extractX() < 0.0f)
    {
        planes[numPlanes].setNormal(MathLib::vector4(1.0f, 0.0f, 0.0f, 0.0f));
        planes[numPlanes].setPointOnPlane(objectSpaceMeshData->oobb_maxExtents);
        numPlanes++;
    }

    if (rayDirection_objectSpace.extractY() > 0.0f)
    {
        planes[numPlanes].setNormal(MathLib::vector4(0.0f, -1.0f, 0.0f, 0.0f));
        planes[numPlanes].setPointOnPlane(objectSpaceMeshData->oobb_minExtents);
        numPlanes++;
    }
    else if (rayDirection_objectSpace.extractY() < 0.0f)
    {
        planes[numPlanes].setNormal(MathLib::vector4(0.0f, 1.0f, 0.0f, 0.0f));
        planes[numPlanes].setPointOnPlane(objectSpaceMeshData->oobb_maxExtents);
        numPlanes++;
    }

    if (rayDirection_objectSpace.extractZ() > 0.0f)
    {
        planes[numPlanes].setNormal(MathLib::vector4(0.0f, 0.0f, -1.0f, 0.0f));
        planes[numPlanes].setPointOnPlane(objectSpaceMeshData->oobb_minExtents);
        numPlanes++;
    }
    else if (rayDirection_objectSpace.extractZ() < 0.0f)
    {
        planes[numPlanes].setNormal(MathLib::vector4(0.0f, 0.0f, 1.0f, 0.0f));
        planes[numPlanes].setPointOnPlane(objectSpaceMeshData->oobb_maxExtents);
        numPlanes++;
    }

    // Get object space intersection point.
    MathLib::ray ray(rayPosition_objectSpace, rayDirection_objectSpace);
    float ray_t;

    for (unsigned int i = 0; i < numPlanes; i++)
    {
        MathLib::vector4 intersectionPoint;
        if (MathLib::intersectRayWithPlane(ray, planes[i], &ray_t))
        {
            if (ray_t < 0.0f)
			{
                continue;
			}

            MathLib::vector4_copy(intersectionPoint, ray.getPosition());
            MathLib::vector4_addScaledVector(intersectionPoint, ray.getDirection(), ray_t, intersectionPoint);
            MathLib::vector4_setToPoint(intersectionPoint);

            float intersectionPoint_x = intersectionPoint.extractX();
            float intersectionPoint_y = intersectionPoint.extractY();
            float intersectionPoint_z = intersectionPoint.extractZ();

            const float epsilon = 0.1f;

            if (intersectionPoint_x >= (objectSpaceMeshData -> oobb_minExtents.extractX() - epsilon) &&
                intersectionPoint_x <= (objectSpaceMeshData -> oobb_maxExtents.extractX() + epsilon) &&
                intersectionPoint_y >= (objectSpaceMeshData -> oobb_minExtents.extractY() - epsilon) &&
                intersectionPoint_y <= (objectSpaceMeshData -> oobb_maxExtents.extractY() + epsilon) &&
                intersectionPoint_z >= (objectSpaceMeshData -> oobb_minExtents.extractZ() - epsilon) &&
                intersectionPoint_z <= (objectSpaceMeshData -> oobb_maxExtents.extractZ() + epsilon))
            {
                // Then the intersection point is a point on or inside the bounding box, transform it into world space.
                MathLib::vector4 intersectionPoint_worldSpace;
                MathLib::matrix4x4_vectorMul(transform, intersectionPoint, intersectionPoint_worldSpace);

                // The distance to the point is the t value we must return.
                *t  = MathLib::vector4_distance(intersectRay.getPosition(), intersectionPoint_worldSpace);

                return true;
            }
        }
    }

	return false;
}

bool BoundingOOBB::IsVisibleFromFrustum(const MathLib::plane frustum[]) const
{
	for (unsigned int i = 0; i < 6; i++)
	{
		bool FTLVisible = false;
		bool FTRVisible = false;
		bool FBRVisible = false;
		bool FBLVisible = false;
		bool BTLVisible = false;
		bool BTRVisible = false;
		bool BBRVisible = false;
		bool BBLVisible = false;

		const MathLib::vector4& pointOnPlane = frustum[i].getPointOnPlane();
		const MathLib::vector4& planeNormal = frustum[i].getNormal();

		MathLib::vector4 pointInRelationToPlane;
		float projection;

		// Check FTL
		MathLib::vector4_sub(worldSpace_FTL, pointOnPlane, pointInRelationToPlane);

		projection = MathLib::vector4_dotProduct(pointInRelationToPlane, planeNormal);
		if (projection >= 0.0f)
		{
			FTLVisible = true;
		}

		// Check FTR
		MathLib::vector4_sub(worldSpace_FTR, pointOnPlane, pointInRelationToPlane);

		projection = MathLib::vector4_dotProduct(pointInRelationToPlane, planeNormal);
		if (projection >= 0.0f)
		{
			FTRVisible = true;
		}

		// Check FBR
		MathLib::vector4_sub(worldSpace_FBR, pointOnPlane, pointInRelationToPlane);

		projection = MathLib::vector4_dotProduct(pointInRelationToPlane, planeNormal);
		if (projection >= 0.0f)
		{
			FBRVisible = true;
		}

		// Check FBL
		MathLib::vector4_sub(worldSpace_FBR, pointOnPlane, pointInRelationToPlane);

		projection = MathLib::vector4_dotProduct(pointInRelationToPlane, planeNormal);
		if (projection >= 0.0f)
		{
			FBLVisible = true;
		}

		// Check BTL
		MathLib::vector4_sub(worldSpace_BTL, pointOnPlane, pointInRelationToPlane);

		projection = MathLib::vector4_dotProduct(pointInRelationToPlane, planeNormal);
		if (projection >= 0.0f)
		{
			BTLVisible = true;
		}

		MathLib::vector4_sub(worldSpace_BTR, pointOnPlane, pointInRelationToPlane);

		projection = MathLib::vector4_dotProduct(pointInRelationToPlane, planeNormal);
		if (projection >= 0.0f)
		{
			BTRVisible = true;
		}

		// Check BBR
		MathLib::vector4_sub(worldSpace_BBR, pointOnPlane, pointInRelationToPlane);

		projection = MathLib::vector4_dotProduct(pointInRelationToPlane, planeNormal);
		if (projection >= 0.0f)
		{
			BBRVisible = true;
		}

		// Check BBL
		MathLib::vector4_sub(worldSpace_BBL, pointOnPlane, pointInRelationToPlane);

		projection = MathLib::vector4_dotProduct(pointInRelationToPlane, planeNormal);
		if (projection >= 0.0f)
		{
			BBLVisible = true;
		}

		if ((!FTLVisible) && (!FTRVisible) && (!FBRVisible) && (!FBLVisible) &&
			(!BTLVisible) && (!BTRVisible) && (!BBRVisible) && (!BBLVisible))
		{
			return false;
		}
	}

	return true;
}

