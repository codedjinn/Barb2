#ifndef TERRAINGENERATIONALGORITHMS_H
#define TERRAINGENERATIONALGORITHMS_H

#include "TerrainCommon.h"

namespace TerrainLib
{

class TerrainGenerationAlgorithms
{
	public:

		static void HillGenerationAlgorithm(float* heightsArray, unsigned int gridSize, unsigned int numIterations, float heightModifier);
};

}

#endif // TERRAINGENERATIONALGORITHMS_H
