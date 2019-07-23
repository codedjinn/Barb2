#ifndef TERRAINUPDATE_H_INCLUDED
#define TERRAINUPDATE_H_INCLUDED

#include "Terrain.h"

namespace TerrainLib
{

struct TerrainUpdateData
{
	static const int UPDATE_MODE_ADD	= 0;
	static const int UPDATE_MODE_SUB	= 1;
	static const int UPDATE_MODE_SMOOTH	= 2;
	static const int UPDATE_MODE_LEVEL	= 3;

	float x;
	float z;

	float radius;
	float strength;

	int mode;
};

class TerrainUpdate
{
	public:

		static void UpdateHeightMap(float* heightmap, const TerrainUpdateData& updateData, const MathLib::vector4& terrainPosition);
};

}

#endif // TERRAINUPDATE_H_INCLUDED
