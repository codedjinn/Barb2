#ifndef BOUNDINGAABB_H_INCLUDED
#define BOUNDINGAABB_H_INCLUDED

#include "BoundingVolume.h"

namespace BoundingVolumes
{

class BoundingAABB :
	 public BoundingVolume
{
	public:

		MathLib::vector4 maxExtents;
		MathLib::vector4 minExtents;

		BoundingAABB() :
			BoundingVolume()
		{
		}

		BoundingAABB(float maxX, float maxY, float maxZ, float minX, float minY, float minZ) :
			BoundingVolume()
		{
			maxExtents.setXYZW(maxX, maxY, maxZ, 1.0f);
			minExtents.setXYZW(minX, minY, minZ, 1.0f);

			bv_type = BOUNDING_VOLUME_AABB;
		}

		bool IsVisibleFromFrustum(const MathLib::plane frustum[]) const;

		bool IntersectsWithRay(const MathLib::ray& intersectRay, float* t) const;

};

}

#endif // BOUNDINGAABB_H_INCLUDED
