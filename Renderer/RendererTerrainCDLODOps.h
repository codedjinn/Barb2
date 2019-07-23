#ifndef RENDERERTERRAINCDLODOPS_H_INCLUDED
#define RENDERERTERRAINCDLODOPS_H_INCLUDED

#include <vector>

#include "RendererTerrainCDLOD.h"
#include "RendererTerrainCDLODNode.h"

namespace RendererLib
{

using namespace std;

class TerrainCDLODOps
{
	public:

		static vector<TerrainCDLODOrderedNode> GetIntersectingNodes(const RendererTerrainCDLOD& terrain, const MathLib::ray& ray);

		/**
		* Intersects the ray with the node's triangle set.
		* @param currentNode			: The (child) node to intersect against.
		* @param landscapePosition		: The position of the landscape, i.e the position of the root node of the quadtree.
		* @param heightsArray			: The array of height values.
		* @param ray 					: The ray to intersect.
		* @param t						: The t value to reconstruct the intersection point with.
		* @returns 						: true if an intersection was found, otherwise false.
		*/
		static bool IntersectNodeTriangles(const RendererTerrainCDLODNode& currentNode,
										   const MathLib::vector4& landscapePosition,
										   float const * const heightsArray,
										   const MathLib::ray& ray,
										   float* t);
};

}

#endif // RENDERERTERRAINCDLODOPS_H_INCLUDED
