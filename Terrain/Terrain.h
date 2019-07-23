#ifndef TERRAIN_H_INCLUDED
#define TERRAIN_H_INCLUDED

#include <MathLib.h>
#include "TerrainCommon.h"
#include "TerrainQuadtree.h"
#include <iostream>

namespace TerrainLib
{

class Terrain
{
	public:

		Terrain(const MathLib::vector4& position, float* data, int maxExtent);
		~Terrain();

		float* GetHeightData();

		TerrainQuadtreeNode* GetQuadtreeRootNode() const;

		const MathLib::vector4& GetPosition() {return position;}

		static int GetHeightmapOffset(int x, int z)
		{
			return x + (z * (TERRAIN_LANDSCAPE_SIZE));
		}

	protected:

		/// Position of this terrain landscape.
		MathLib::vector4 position;

		TerrainQuadtreeNode* rootNode;

		float* heightData;

		void FreeMemory();
};

}

#endif // TERRAIN_H_INCLUDED
