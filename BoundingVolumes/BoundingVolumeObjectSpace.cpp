#include "BoundingVolumeObjectSpace.h"

using namespace BoundingVolumes;

BoundingVolumeObjectSpace::BoundingVolumeObjectSpace(ObjectSpaceMeshData const * objectSpaceMeshData) :
	BoundingVolume()
{
	BoundingVolumeObjectSpace::objectSpaceMeshData = objectSpaceMeshData;
}

