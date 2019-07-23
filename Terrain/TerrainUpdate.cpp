#include "TerrainUpdate.h"

namespace TerrainLib
{

static int ClipExtents(int value)
{
	if (value < 0)
	{
		return 0;
	}
	if (value > (TERRAIN_LANDSCAPE_SIZE - 1))
	{
		return (TERRAIN_LANDSCAPE_SIZE - 1);
	}

	return (value);
}

static int HeightmapOffset(int x, int z)
{
	return (x + (z * TERRAIN_LANDSCAPE_SIZE));
}


static void UpdateHeightMapAdditive(float* heightmap, const TerrainUpdateData& updateData, const MathLib::vector4& terrainPosition)
{
	// First we need to get the index of the top left of the update region of the heightmap data.
	float relativeUpdatePositionX = terrainPosition.extractX() - updateData.x;
	float relativeUpdatePositionZ = terrainPosition.extractZ() - updateData.z;

	const int updateRegionExtent = static_cast<int>(updateData.radius) * 2;
	int updateRegionStartX = ClipExtents(static_cast<int>(updateData.x - updateData.radius));
	int updateRegionStartZ = ClipExtents(static_cast<int>(updateData.z - updateData.radius));
	int updateRegionEndX = ClipExtents(updateRegionStartX + updateRegionExtent);
	int updateRegionEndZ = ClipExtents(updateRegionStartZ + updateRegionExtent); 

	MathLib::vector4 updatePoint(updateData.x, 0.0f, updateData.z, 1.0f);

	for (int z = updateRegionStartZ; z < updateRegionEndZ; z++)
	{
		for (int x = updateRegionStartX; x < updateRegionEndX; x++)
		{
			float distanceFactor = updateData.radius - MathLib::vector4_distance(MathLib::vector4((float)x, 0, (float)z, 1), updatePoint);
			if (distanceFactor < 0.0f)
				distanceFactor = 0.0f;
			distanceFactor /= updateData.radius;

			heightmap[HeightmapOffset(x, TERRAIN_LANDSCAPE_SIZE - 1 - z)] += distanceFactor;
		}
	}

}

static void UpdateHeightMapSubtractive(float* heightmap, const TerrainUpdateData& updateData, const MathLib::vector4& terrainPosition)
{
	// First we need to get the index of the top left of the update region of the heightmap data.
	float relativeUpdatePositionX = terrainPosition.extractX() - updateData.x;
	float relativeUpdatePositionZ = terrainPosition.extractZ() - updateData.z;

	const int updateRegionExtent = static_cast<int>(updateData.radius) * 2;
	int updateRegionStartX = ClipExtents(static_cast<int>(updateData.x - updateData.radius));
	int updateRegionStartZ = ClipExtents(static_cast<int>(updateData.z - updateData.radius));
	int updateRegionEndX = ClipExtents(updateRegionStartX + updateRegionExtent);
	int updateRegionEndZ = ClipExtents(updateRegionStartZ + updateRegionExtent);

	MathLib::vector4 updatePoint(updateData.x, 0.0f, updateData.z, 1.0f);

	for (int z = updateRegionStartZ; z < updateRegionEndZ; z++)
	{
		for (int x = updateRegionStartX; x < updateRegionEndX; x++)
		{
			float distanceFactor = updateData.radius - MathLib::vector4_distance(MathLib::vector4((float)x, 0, (float)z, 1.0f), updatePoint);
			if (distanceFactor < 0.0f)
				distanceFactor = 0.0f;
			distanceFactor /= updateData.radius;

			int currentIndex = HeightmapOffset(x, TERRAIN_LANDSCAPE_SIZE - 1 - z);
			heightmap[currentIndex] -= distanceFactor;
			if (heightmap[currentIndex] < 0.0f)
			{
				heightmap[currentIndex] = 0.0f;
			}
		}
	}
}

static void UpdateHeightMapSmoothing(float* heightmap, const TerrainUpdateData& updateData, const MathLib::vector4& terrainPosition)
{
	// First we need to get the index of the top left of the update region of the heightmap data.
	float relativeUpdatePositionX = terrainPosition.extractX() - updateData.x;
	float relativeUpdatePositionZ = terrainPosition.extractZ() - updateData.z;

	const int updateRegionExtent = static_cast<int>(updateData.radius) * 2;
	int updateRegionStartX = ClipExtents(static_cast<int>(updateData.x - updateData.radius));
	int updateRegionStartZ = ClipExtents(static_cast<int>(updateData.z - updateData.radius));
	int updateRegionEndX = ClipExtents(updateRegionStartX + updateRegionExtent);
	int updateRegionEndZ = ClipExtents(updateRegionStartZ + updateRegionExtent);

	MathLib::vector4 updatePoint(updateData.x, 0.0f, updateData.z, 1.0f);

	for (int z = updateRegionStartZ; z < updateRegionEndZ; z++)
	{
		for (int x = updateRegionStartX; x < updateRegionEndX; x++)
		{
			float distanceFactor = updateData.radius - MathLib::vector4_distance(MathLib::vector4((float)x, 0, (float)z, 1), updatePoint);
			if (distanceFactor > 0.0f)
			{
				int indexTLx = x - 1;
				indexTLx = (indexTLx < 0) ? 0 : indexTLx;

				int indexTLz = z - 1;
				indexTLz = (indexTLz < 0) ? 0 : indexTLz;

				int indexBRx = x + 1;
				indexBRx = (indexBRx > TERRAIN_LANDSCAPE_SIZE - 1) ? TERRAIN_LANDSCAPE_SIZE - 1 : indexBRx;

				int indexBRz = z + 1;
				indexBRz = (indexBRz > TERRAIN_LANDSCAPE_SIZE - 1) ? TERRAIN_LANDSCAPE_SIZE - 1 : indexBRz;

				int heightmapZ = TERRAIN_LANDSCAPE_SIZE - 1 - z;
				indexTLz = TERRAIN_LANDSCAPE_SIZE - 1 - indexTLz;
				indexBRz = TERRAIN_LANDSCAPE_SIZE - 1 - indexBRz;

				float _00 = heightmap[HeightmapOffset(indexTLx, indexTLz)];
				float _01 = heightmap[HeightmapOffset(x, indexTLz)];
				float _02 = heightmap[HeightmapOffset(indexBRx, indexTLz)];

				float _10 = heightmap[HeightmapOffset(indexTLx, heightmapZ)];
				float _11 = heightmap[HeightmapOffset(x, heightmapZ)];
				float _12 = heightmap[HeightmapOffset(indexBRx, heightmapZ)];

				float _20 = heightmap[HeightmapOffset(indexTLx, indexBRz)];
				float _21 = heightmap[HeightmapOffset(x, indexBRz)];
				float _22 = heightmap[HeightmapOffset(indexBRx, indexBRz)];

				heightmap[HeightmapOffset(x, heightmapZ)] = (_00 + _01 + _02 + _10 + _11 + _12 + _20 + _21 + _22) / 9.0f;
			}
		}
	}
}

static void UpdateHeightMapLevel(float* heightmap, const TerrainUpdateData& updateData, const MathLib::vector4& terrainPosition)
{
	// First we need to get the index of the top left of the update region of the heightmap data.
	float relativeUpdatePositionX = terrainPosition.extractX() - updateData.x;
	float relativeUpdatePositionZ = terrainPosition.extractZ() - updateData.z;

	const int updateRegionExtent = static_cast<int>(updateData.radius) * 2;
	int updateRegionStartX = ClipExtents(static_cast<int>(updateData.x - updateData.radius));
	int updateRegionStartZ = ClipExtents(static_cast<int>(updateData.z - updateData.radius));
	int updateRegionEndX = ClipExtents(updateRegionStartX + updateRegionExtent);
	int updateRegionEndZ = ClipExtents(updateRegionStartZ + updateRegionExtent);

	MathLib::vector4 updatePoint(updateData.x, 0.0f, updateData.z, 1.0f);

	for (int z = updateRegionStartZ; z < updateRegionEndZ; z++)
	{
		for (int x = updateRegionStartX; x < updateRegionEndX; x++)
		{
			float distanceFactor = updateData.radius - MathLib::vector4_distance(MathLib::vector4((float)x, 0, (float)z, 1.0f), updatePoint);
			if (distanceFactor > 0.0f)
			{
				int heightmapZ = TERRAIN_LANDSCAPE_SIZE - 1 - z;
				heightmap[HeightmapOffset(x, heightmapZ)] = updateData.strength;
			}
		}
	}
}

void TerrainUpdate::UpdateHeightMap(float* heightmap, const TerrainUpdateData& updateData, const MathLib::vector4& terrainPosition)
{

	if (TerrainUpdateData::UPDATE_MODE_ADD == updateData.mode)
	{
		UpdateHeightMapAdditive(heightmap, updateData, terrainPosition);
	}
	else if (TerrainUpdateData::UPDATE_MODE_SUB == updateData.mode)
	{
		UpdateHeightMapSubtractive(heightmap, updateData, terrainPosition);
	}
	else if (TerrainUpdateData::UPDATE_MODE_SMOOTH == updateData.mode)
	{
		UpdateHeightMapSmoothing(heightmap, updateData, terrainPosition);
	}
	else if (TerrainUpdateData::UPDATE_MODE_LEVEL == updateData.mode)
	{
		UpdateHeightMapLevel(heightmap, updateData, terrainPosition);
	}
}

}
