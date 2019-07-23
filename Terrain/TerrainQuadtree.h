#ifndef TERRAINQUADTREE_H_INCLUDED
#define TERRAINQUADTREE_H_INCLUDED

#include "TerrainCommon.h"

namespace TerrainLib
{

class TerrainQuadtreeNode
{
	public:

		static const int QUADTREE_NODE_TL = 0;
		static const int QUADTREE_NODE_TR = 1;
		static const int QUADTREE_NODE_BL = 2;
		static const int QUADTREE_NODE_BR = 3;

		MathLib::vector4 position;

		unsigned short scale;

		TerrainQuadtreeNode* nodes[4];

		TerrainQuadtreeNode();
		~TerrainQuadtreeNode();

		bool IsChild() const;

		void CalculateExtents(MathLib::vector4 extentsArray[]) const;

		/**
		* The total available terrain size is dependent on the renderer's patch sizes.
		* To this end, the system quadtree needs to provide for that maximum extent i.e.
		* the system quadtree should encompass the maximum extent of the renderer quadtree
		* at least and should simply discard any results that fall outside of that.
		* This formula provides the system quadtree depth given the maximum extent available.
		* In order to use, create the root node, and set it's scale factor to 2 to the power of this result.
		* Then create the quadtree as usual.
		*/
		static int GetSystemQuadtreeDepth(int maxExtent);

		static TerrainQuadtreeNode* GenerateQuadtree(const MathLib::vector4 position, int rendererQuadtreeMaxExtent);

	protected:

		void FreeResources();
};

}

#endif // TERRAINQUADTREE_H_INCLUDED
