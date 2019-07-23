#include "ObjectSpaceMeshData.h"
#include <limits>

namespace BoundingVolumes
{

void ObjectSpaceMeshData::GenerateBoundsInformation(const float* const vertices, uint32_t numVertices)
{
	// Calculate maximum radius for the bounding sphere precalculatio:
    float maxRadius = 0.0f;

    for (uint32_t i = 0; i < numVertices * 3; i += 3)
    {
        MathLib::vector4 currentVertex(vertices[i],
                                       vertices[i + 1],
                                       vertices[i + 2],
                                       1.0f);

        float currentDistance   = MathLib::vector4_magnitude(currentVertex);

        if (currentDistance > maxRadius)
            maxRadius   = currentDistance;
    }

	radius	= maxRadius;

    // Calculate the min and max extents of the oobb:
    oobb_minExtents.setXYZW(std::numeric_limits<float>::max(),
                            std::numeric_limits<float>::max(),
                            std::numeric_limits<float>::max(),
                            1.0f);

    oobb_maxExtents.setXYZW(-std::numeric_limits<float>::max(),
                            -std::numeric_limits<float>::max(),
                            -std::numeric_limits<float>::max(),
                            1.0f);

    for (uint32_t i = 0; i < numVertices * 3; i += 3)
    {
        float currentX  = vertices[i];
        float currentY  = vertices[i + 1];
        float currentZ  = vertices[i + 2];

        if (currentX > oobb_maxExtents.extractX()) oobb_maxExtents.setX(currentX);
        if (currentX < oobb_minExtents.extractX()) oobb_minExtents.setX(currentX);

        if (currentY > oobb_maxExtents.extractY()) oobb_maxExtents.setY(currentY);
        if (currentY < oobb_minExtents.extractY()) oobb_minExtents.setY(currentY);

        if (currentZ > oobb_maxExtents.extractZ()) oobb_maxExtents.setZ(currentZ);
        if (currentZ < oobb_minExtents.extractZ()) oobb_minExtents.setZ(currentZ);
    }
}

}
