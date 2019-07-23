#ifndef RENDERERTERRAINCDLODNODE_H_INCLUDED
#define RENDERERTERRAINCDLODNODE_H_INCLUDED

#include <BoundingVolumes.h>

namespace RendererLib
{

#define TERRAIN_CDLOD_NODE_TL		0
#define TERRAIN_CDLOD_NODE_TR		1
#define TERRAIN_CDLOD_NODE_BL		2
#define TERRAIN_CDLOD_NODE_BR		3

class RendererTerrainCDLODNode
{
	public:

		MathLib::vector4 position;

		unsigned int scale;

		RendererTerrainCDLODNode* nodes[4];

		RendererTerrainCDLODNode();
		~RendererTerrainCDLODNode();

		void GenerateQuadtree();

		bool IsChild() const;

		MathLib::plane* GetPlanes();
		MathLib::vector4 const * const GetBoundingBoxVertices() const;
		MathLib::vector4 const * const GetBoundingBoxExtents() const;

		/**
		* Convenience method to get the number of quadtree levels that will result from using a
		* terrain of terrainSize with the corresponding patchSize.
		*/
		static unsigned int GetNumQuadtreeLevels(unsigned int terrainSize, unsigned int patchSize);

		/**
		* Generates the planes and bounding vertices for this node. Used for LOD selection and
		* frustum culling.
		*/
		static void GenerateBoundingVolumeInformation(RendererTerrainCDLODNode* node);

	protected:

		void FreeResources();

		MathLib::plane planes[6];
		MathLib::vector4 vertices[8];
		MathLib::vector4 extents[2];
};

}

#endif // RENDERERTERRAINCDLODNODE_H_INCLUDED
