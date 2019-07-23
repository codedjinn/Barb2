#ifndef BOUNDINGVOLUMEOBJECTSPACE_H_INCLUDED
#define BOUNDINGVOLUMEOBJECTSPACE_H_INCLUDED

#include "BoundingVolumeCommon.h"
#include "BoundingVolume.h"
#include "ObjectSpaceMeshData.h"
#include <MathLib.h>

namespace BoundingVolumes
{

/**
* Base class for all bounding volume types that contain an object space representation.
*/
class BoundingVolumeObjectSpace :
	public BoundingVolume
{
	public:

		virtual ~BoundingVolumeObjectSpace() {}

		BoundingVolumeObjectSpace(ObjectSpaceMeshData const * objectSpaceMeshData);

		/**
		* Generate the per instance bounds.
		*/
		virtual void GenerateInstanceBounds(const MathLib::vector4& position,
											const MathLib::vector4& rotation,
											const MathLib::vector4& scale, 
											const MathLib::matrix4x4& parentTransform) = 0;

        virtual void GenerateInstanceBounds(const MathLib::vector4& position,
                                            const MathLib::quaternion& orientation,
                                            const float scale, 
											const MathLib::matrix4x4& parentTransform) = 0;



	protected:

		ObjectSpaceMeshData const * objectSpaceMeshData;

		MathLib::matrix4x4 transform;
};

}

#endif // BOUNDINGVOLUMEOBJECTSPACE_H_INCLUDED
