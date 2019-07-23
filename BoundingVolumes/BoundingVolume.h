#ifndef BOUNDINGVOLUME_H_INCLUDED
#define BOUNDINGVOLUME_H_INCLUDED

#include "BoundingVolumeCommon.h"
#include "ObjectSpaceMeshData.h"

namespace BoundingVolumes
{

enum BoundingVolumeType
{
	BOUNDING_VOLUME_SPHERE	= 0,
	BOUNDING_VOLUME_OOBB	= 1,
	BOUNDING_VOLUME_AABB	= 2
};

class BoundingVolume
{
	public:

        BoundingVolume()
        {
            bv_type = BOUNDING_VOLUME_SPHERE;
        }

		virtual ~BoundingVolume() {}

		virtual bool IsVisibleFromFrustum(const MathLib::plane frustum[]) const = 0;
		/// Returns true if this bounding volume is contained within, or intersects,
		/// the frustum specified by the array of planes passed into it.
		/// @note The array specified by @param frustum MUST contain at least 6 elements.

		virtual bool IntersectsWithRay(const MathLib::ray& intersectRay, float* t) const = 0;
		/// Returns true if an intersection. False if not. If there is an intersection, then
		/// t holds the magnitude of the ray to get the intersection point.

		BOUNDINGVOLUMES_INLINE BoundingVolumeType GetBoundingVolumeType() const
		{
			return bv_type;
		}

	protected:

		BoundingVolumeType bv_type;
};

}

#endif // BOUNDINGVOLUME_H_INCLUDED
