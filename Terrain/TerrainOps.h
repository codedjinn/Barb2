#ifndef TERRAINOPS_H_INCLUDED
#define TERRAINOPS_H_INCLUDED

#include "Terrain.h"
#include <vector>

namespace TerrainLib
{

using namespace std;

struct TerrainOrderedQuadtreeNode
{
	TerrainQuadtreeNode const * node;	/// The node that was intersected.
	float t;							/// The t value of the ray at the point of intersection with this node.

	bool operator==(TerrainOrderedQuadtreeNode& right)
	{
	    return this == &right;
	}
};

class TerrainOps
{
	public:

		static vector<TerrainOrderedQuadtreeNode> GetIntersectingNodes(const Terrain& terrain, const MathLib::ray& ray);

		/**
		* Intersects the ray with the node's triangle set.
		* @param currentNode			: The (child) node to intersect against.
		* @param landscapePosition		: The position of the landscape, i.e the position of the root node of the quadtree.
		* @param maxExtent 				: The maximum extent of the landscape. Dependent on renderer settings.
		* @param heightsArray			: The array of height values.
		* @param ray 					: The ray to intersect.
		* @param t						: The t value to reconstruct the intersection point with.
		* @returns 						: true if an intersection was found, otherwise false.
		*/
		static bool IntersectNodeTriangles(const TerrainQuadtreeNode& currentNode,
										   const MathLib::vector4& landscapePosition,
										   float maxExtent,
										   float const * const heightsArray,
										   const MathLib::ray& ray,
										   float* t);
};
}

#endif // TERRAINOPS_H_INCLUDED
