#include "TerrainGenerationAlgorithms.h"
#include <cstdlib>
#include <cmath>

namespace TerrainLib
{

static unsigned int GetVertexCount(unsigned int gridSize)
{
	return (gridSize + 1) * (gridSize + 1);
}

static float HillAlgorithmHeightFactor(float x, float z, float hillX, float hillZ, float hillRadius)
{
	float radiusSquared = hillRadius * hillRadius;

	float xVal = (x - hillX) * (x - hillX);
	float zVal = (z - hillZ) * (z - hillZ);

	float heightFactor = 0.0f;
	if (radiusSquared - xVal - zVal > 0)
		heightFactor = sqrtf(radiusSquared - xVal - zVal);

	return heightFactor;
}

static void HillAlgorithmIteration(float* heightsArray, unsigned int gridSize, float heightModifier)
{
	// Create new hill.
	float hillX = (float)(rand() % (gridSize + 1));
	float hillZ = (float)(rand() % (gridSize + 1));
	float hillRadius = (float)(rand() % 1000);

	// Iterate through all vertices and compute result.
	for (unsigned int i = 0; i < GetVertexCount(gridSize); i++)
	{
		float vertexX = (float)(i % (gridSize + 1));
		float vertexZ = (float)(i / (gridSize + 1));
		heightsArray[i] += HillAlgorithmHeightFactor(vertexX, vertexZ, hillX, hillZ, hillRadius) * heightModifier;
	}
}

static void Smooth(float* heightArray, unsigned int gridSize)
{
	int vertsPerRow = gridSize + 1;

	int lastVertIndex = (int)(GetVertexCount(gridSize) - 1);

	for (unsigned int i = 0; i < GetVertexCount(gridSize); i++)
	{
		float heights[3][3];
		int currentIndex;

		heights[1][1] = heightArray[i];

		currentIndex = i - vertsPerRow - 1;
		heights[0][0] = currentIndex < 0 ? heightArray[i] : heightArray[currentIndex];

		currentIndex = i - vertsPerRow;
		heights[0][1] = currentIndex < 0 ? heightArray[i] : heightArray[currentIndex];

		currentIndex = i - vertsPerRow + 1;
		heights[0][2] = currentIndex < 0 ? heightArray[i] : heightArray[currentIndex];

		currentIndex = i - 1;
		heights[1][0] = currentIndex < 0 ? heightArray[i] : heightArray[currentIndex];

		currentIndex = i + 1;
		heights[1][2] = currentIndex > lastVertIndex ? heightArray[i] : heightArray[currentIndex];

		currentIndex = i + vertsPerRow - 1;
		heights[2][0] = currentIndex > lastVertIndex ? heightArray[i] : heightArray[currentIndex];

		currentIndex = i + vertsPerRow;
		heights[2][1] = currentIndex > lastVertIndex ? heightArray[i] : heightArray[currentIndex];

		currentIndex = i + vertsPerRow + 1;
		heights[2][2] = currentIndex > lastVertIndex ? heightArray[i] : heightArray[currentIndex];

		float total = 0.0f;
		for (int j = 0; j < 3; j++)
			for (int k = 0; k < 3; k++)
				total += heights[j][k];
		heightArray[i] = total / 9.0f;
	}
}

void TerrainGenerationAlgorithms::HillGenerationAlgorithm(float* heightsArray, unsigned int gridSize, unsigned int numIterations, float heightModifier)
{
	for (unsigned int i = 0; i < numIterations; i++)
	{
		HillAlgorithmIteration(heightsArray, gridSize, heightModifier);
	}

	for (unsigned int i = 0; i < numIterations; i++)
		Smooth(heightsArray, gridSize);
}

}
