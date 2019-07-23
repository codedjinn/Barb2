#include "Terrain.h"
#include <cassert>
#include <cstring>

namespace TerrainLib
{

Terrain::Terrain(const MathLib::vector4& position, float* data, int maxExtent)
{
	MathLib::vector4_copy(Terrain::position, position);

	assert(nullptr != data);

	heightData = data;

	rootNode = TerrainQuadtreeNode::GenerateQuadtree(Terrain::position, maxExtent);
}

Terrain::~Terrain()
{
	FreeMemory();
}

float* Terrain::GetHeightData()
{
	return heightData;
}

TerrainQuadtreeNode* Terrain::GetQuadtreeRootNode() const
{
	return rootNode;
}

void Terrain::FreeMemory()
{
	if (nullptr != rootNode)
	{
		delete rootNode;
	}

	if (nullptr != heightData)
	{
		delete heightData;
	}
}

}
