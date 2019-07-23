#ifndef OBJECTSPACEMESHDATA_H_INCLUDED
#define OBJECTSPACEMESHDATA_H_INCLUDED

#include "BoundingVolumeCommon.h"

namespace BoundingVolumes
{

struct ObjectSpaceMeshData
{
	/// OOBB.
	MathLib::vector4 oobb_minExtents;
	MathLib::vector4 oobb_maxExtents;

	/// Bounding sphere.
	float radius;

    void GenerateBoundsInformation(float const * const vertices, unsigned int numVertices);
};

}

#endif // OBJECTSPACEMESHDATA_H_INCLUDED
