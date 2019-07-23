#include "TerrainOps.h"
#include <cassert>

namespace TerrainLib
{

static MathLib::vector4 aabbExtents[2];

static void RecursiveNodeIntersection(const TerrainQuadtreeNode& currentNode, vector<TerrainOrderedQuadtreeNode>& orderedList, const MathLib::ray& ray)
{
	float t;

	currentNode.CalculateExtents(aabbExtents);

	bool intersects = GeometryLib::RayIntersectsAABB(ray, &t, aabbExtents);
	if (intersects)
	{
		// Is this a child node?
		if (currentNode.IsChild())
		{
			// If so, then create a new ordered node and insert it into the correct place in the list.
			TerrainOrderedQuadtreeNode currentOrderedNode;
			currentOrderedNode.node = &currentNode;
			currentOrderedNode.t = t;

			if (orderedList.size() > 0)
			{
				int currentIndex = 0;
				while ((currentIndex < orderedList.size()) && (t > orderedList[currentIndex].t))
				{
					currentIndex++;
				}

				if (currentIndex == orderedList.size())
				{
                    orderedList.push_back(currentOrderedNode);
				}
				else
				{
                    orderedList.insert(orderedList.begin() + currentIndex, currentOrderedNode);
				}
			}
			else
			{
				orderedList.push_back(currentOrderedNode);
			}
		}
		else
		{
			// Perform operation on each of the quadtree children.
			for (int i = 0; i < 4; i++)
			{
				RecursiveNodeIntersection(*currentNode.nodes[i], orderedList, ray);
			}
		}
	}
}

vector<TerrainOrderedQuadtreeNode> TerrainOps::GetIntersectingNodes(const Terrain& terrain, const MathLib::ray& ray)
{
    vector<TerrainOrderedQuadtreeNode> orderedList;

	const TerrainQuadtreeNode& rootNode = *terrain.GetQuadtreeRootNode();

	RecursiveNodeIntersection(rootNode, orderedList, ray);

	return orderedList;
}

static int HeightArrayOffset(int x, int z)
{
	assert(x >= 0 && x < TERRAIN_LANDSCAPE_SIZE);
	assert(z >= 0 && z < TERRAIN_LANDSCAPE_SIZE);

	return (x + ((TERRAIN_LANDSCAPE_SIZE - 1 - z) * (TERRAIN_LANDSCAPE_SIZE)));
}

bool TerrainOps::IntersectNodeTriangles(const TerrainQuadtreeNode& currentNode,
				 					    const MathLib::vector4& landscapePosition,
										float maxExtent,
										float const * const heightsArray,
										const MathLib::ray& ray,
										float* t)
{
	// Clamp results to maxExtent.
	if (currentNode.position.extractX() >= maxExtent - 1 ||
		currentNode.position.extractZ() >= maxExtent - 1)
	{
		return false;
	}

	MathLib::vector4 relativePosition;
	MathLib::vector4_sub(currentNode.position, landscapePosition, relativePosition);

	const unsigned int gridSize = 2;
	const unsigned int numTriangles = 8;

	int currentX = static_cast<int>(relativePosition.extractX());
	int currentZ = static_cast<int>(relativePosition.extractZ());

	for (unsigned int z = 0; z < gridSize; z++)
	{
		for (unsigned int x = 0; x < gridSize; x++)
		{
			MathLib::vector4 point0;
			MathLib::vector4 point1;
			MathLib::vector4 point2;

			// Generate first triangle.
			point0.setXYZW(currentNode.position.extractX() + static_cast<float>(x),
						   heightsArray[HeightArrayOffset(currentX + x, currentZ + z)],
						   currentNode.position.extractZ() + static_cast<float>(z),
						   1.0f);

			point1.setXYZW(currentNode.position.extractX() + static_cast<float>(x + 1),
						   heightsArray[HeightArrayOffset(currentX + x + 1, currentZ + z)],
						   currentNode.position.extractZ() + static_cast<float>(z),
						   1.0f);

			point2.setXYZW(currentNode.position.extractX() + static_cast<float>(x),
						   heightsArray[HeightArrayOffset(currentX + x, currentZ + z + 1)],
						   currentNode.position.extractZ() + static_cast<float>(z + 1),
						   1.0f);

			if (GeometryLib::RayIntersectsTriangle(ray,
												   t,
												   point0,
												   point1,
												   point2))
			{
				return true;
			}

			// Generate second triangle.
			point0.setXYZW(currentNode.position.extractX() + static_cast<float>(x + 1),
						   heightsArray[HeightArrayOffset(currentX + x + 1, currentZ + z)],
						   currentNode.position.extractZ() + static_cast<float>(z),
						   1.0f);

			point1.setXYZW(currentNode.position.extractX() + static_cast<float>(x + 1),
						   heightsArray[HeightArrayOffset(currentX + x + 1, currentZ + z + 1)],
						   currentNode.position.extractZ() + static_cast<float>(z + 1),
						   1.0f);

			point2.setXYZW(currentNode.position.extractX() + static_cast<float>(x),
						   heightsArray[HeightArrayOffset(currentX + x, currentZ + z + 1)],
						   currentNode.position.extractZ() + static_cast<float>(z + 1),
						   1.0f);

			if (GeometryLib::RayIntersectsTriangle(ray,
												   t,
												   point0,
												   point1,
												   point2))
			{
				return true;
			}
		}
	}

	return false;
}

}
