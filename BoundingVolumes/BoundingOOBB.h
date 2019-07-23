#ifndef BOUNDINGOOBB_H_INCLUDED
#define BOUNDINGOOBB_H_INCLUDED

#include "BoundingVolumeObjectSpace.h"

namespace BoundingVolumes
{

class BoundingOOBB :
	public BoundingVolumeObjectSpace
{
	public:

		BoundingOOBB(ObjectSpaceMeshData const * objectSpaceMeshData) :
			BoundingVolumeObjectSpace(objectSpaceMeshData)
		{
			bv_type	= BOUNDING_VOLUME_OOBB;
		}

		virtual ~BoundingOOBB() {}

		bool IntersectsWithRay(const MathLib::ray& intersectRay, float* t) const;

		void GenerateInstanceBounds(const MathLib::vector4& position,
									const MathLib::vector4& rotation,
									const MathLib::vector4& scale, 
									const MathLib::matrix4x4& parentTransform);

        void GenerateInstanceBounds(const MathLib::vector4& position,
                                    const MathLib::quaternion& orientation,
                                    const float scale, 
									const MathLib::matrix4x4& parentTransform);

		bool IsVisibleFromFrustum(const MathLib::plane frustum[]) const;

		const MathLib::vector4& getWorldSpace_FTL()
		{
			return worldSpace_FTL;
		}

		const MathLib::vector4& getWorldSpace_FTR()
		{
			return worldSpace_FTR;
		}

		const MathLib::vector4& getWorldSpace_FBR()
		{
			return worldSpace_FBR;
		}

		const MathLib::vector4& getWorldSpace_FBL()
		{
			return worldSpace_FBL;
		}

		const MathLib::vector4& getWorldSpace_BTL()
		{
			return worldSpace_BTL;
		}

		const MathLib::vector4& getWorldSpace_BTR()
		{
			return worldSpace_BTR;
		}

		const MathLib::vector4& getWorldSpace_BBR()
		{
			return worldSpace_BBR;
		}

		const MathLib::vector4& getWorldSpace_BBL()
		{
			return worldSpace_BBL;
		}

		const MathLib::vector4& getObjectSpaceMin()
		{
			return objectSpaceMeshData->oobb_minExtents;
		}

		const MathLib::vector4& getObjectSpaceMax()
		{
			return objectSpaceMeshData->oobb_maxExtents;
		}

	protected:

		MathLib::vector4 worldSpace_FTL;
		MathLib::vector4 worldSpace_FTR;
		MathLib::vector4 worldSpace_FBR;
		MathLib::vector4 worldSpace_FBL;

		MathLib::vector4 worldSpace_BTL;
		MathLib::vector4 worldSpace_BTR;
		MathLib::vector4 worldSpace_BBR;
		MathLib::vector4 worldSpace_BBL;

		MathLib::matrix4x4 invTransform;
};

}

#endif // BOUNDINGOOBB_H_INCLUDED
