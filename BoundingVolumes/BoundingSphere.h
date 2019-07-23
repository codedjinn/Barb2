#ifndef BOUNDINGSPHERE_H_INCLUDED
#define BOUNDINGSPHERE_H_INCLUDED

#include "BoundingVolumeObjectSpace.h"

namespace BoundingVolumes
{

/**
* Simple bounding sphere.
*/
class BoundingSphere :
	public BoundingVolumeObjectSpace
{
	public:

		BoundingSphere(ObjectSpaceMeshData const * objectSpaceMeshData);
		BoundingSphere(const MathLib::vector4& position, const float radius);

		virtual void GenerateInstanceBounds(const MathLib::vector4& position,
											const MathLib::vector4& rotation,
											const MathLib::vector4& scale, 
											const MathLib::matrix4x4& parentTransform);


		virtual void GenerateInstanceBounds(const MathLib::vector4& position,
											const MathLib::quaternion& orientation,
											const float scale, 
											const MathLib::matrix4x4& parentTransform);

		virtual bool IntersectsWithRay(const MathLib::ray& intersectRay, float* t) const;

		virtual bool IsVisibleFromFrustum(const MathLib::plane frustum[]) const;

		BOUNDINGVOLUMES_INLINE const MathLib::vector4& GetPosition() const
		{
			return worldSpacePosition;
		}

		BOUNDINGVOLUMES_INLINE float GetRadius() const
		{
			return worldSpaceRadius;
		}

	private:

		MathLib::vector4 worldSpacePosition;

		float worldSpaceRadius;
};

}

#endif // BOUNDINGSPHERE_H_INCLUDED
