#include "RendererTerrainCDLODRanges.h"
#include <iostream>
#include <vector>

namespace RendererLib
{


void RendererTerrainCDLODRanges::FreeResources()
{
	if (NULL != ranges)
	{
		delete [] ranges;
	}
	rangesLength = 0;
}

RendererTerrainCDLODRanges::RendererTerrainCDLODRanges()
{
	ranges = NULL;
	rangesLength = 0;
}

RendererTerrainCDLODRanges::~RendererTerrainCDLODRanges()
{
	FreeResources();
}

void RendererTerrainCDLODRanges::ComputeRanges(float viewingDistance, unsigned int quadtreeDepth)
{
	FreeResources();

	float morphStart = 0.95f;

	ranges = new CDLODRange[quadtreeDepth];
	ranges[0].distance = (TERRAIN_PATCH_SIZE + 10.0f) * 2.0f;
	ranges[0].morphAreaStart = ranges[0].distance * morphStart;
	for (unsigned int i = 1; i < quadtreeDepth; i++)
	{
		ranges[i].distance = ranges[i -1].distance * 3.0f;
		ranges[i].morphAreaStart = ranges[i].distance * morphStart;
	}

	rangesLength = quadtreeDepth;

	for (int i = static_cast<int>(rangesLength) - 1; i >= 0; i--)
	{
        std::cerr<<"ranges[" << i << "]:" << ranges[i].distance << std::endl;
	}
}

}



