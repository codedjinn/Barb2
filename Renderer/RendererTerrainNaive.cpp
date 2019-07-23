#include "RendererTerrainNaive.h"
#include "RendererUtils.h"
#include "RenderToken.h"
#include <Terrain.h>
#include <MathLib.h>
#include <vector>
#include <iostream>

struct IndexedTriangle
{
	unsigned int i[3];
};

namespace RendererLib
{

RendererTerrainNaive::RendererTerrainNaive()
{
}

void RendererTerrainNaive::Initialize(float const * const terrainData, unsigned int gridSize, std::string texture)
{
	using namespace std;

	terrainTexture = texture;
	terrainGridSize = gridSize;

	const unsigned int numVertices = (gridSize + 1) * (gridSize + 1);

	GLfloat* vertexArray = new GLfloat[numVertices * 3];
	GLfloat* normalArray = new GLfloat[numVertices * 3];

	// Convert and transfer vertex arrays.
	int positionModifier = 0;						// Adjust if you want to move the grid at all.
	for (int i = 0; i < static_cast<int>(numVertices); i++)
	{
		vertexArray[(i * 3) + 0] = static_cast<GLfloat>((i % (gridSize + 1)) - positionModifier);
		vertexArray[(i * 3) + 1] = static_cast<GLfloat>(terrainData[i]);
		vertexArray[(i * 3) + 2] = static_cast<GLfloat>((i / (gridSize + 1)) - positionModifier);
	}

	// Generate a list of Triangles that index the vertex array.
	const unsigned int numTriangles = gridSize * gridSize * 2;

	IndexedTriangle* triangles = new IndexedTriangle[numTriangles];
	MathLib::vector4* triangleNormals = new MathLib::vector4[numTriangles];

	unsigned int currentTriangleIndex = 0;
	for (unsigned int y = 0; y < gridSize; y++)
	{
		for (unsigned int x = 0; x < gridSize; x++)
		{
			MathLib::vector4 vertices[3];
			MathLib::vector4 u, v;
			MathLib::vector4 normal;

			// Generate first triangle.
			IndexedTriangle& firstTriangle = triangles[currentTriangleIndex];
			firstTriangle.i[0] = x + (y * (gridSize + 1));
			firstTriangle.i[1] = firstTriangle.i[0] + 1;
			firstTriangle.i[2] = x + ((y + 1) * (gridSize + 1));

			// Compute normal for first triangle.
			vertices[0].setXYZ(vertexArray[(firstTriangle.i[0] * 3) + 0],
							   vertexArray[(firstTriangle.i[0] * 3) + 1],
							   vertexArray[(firstTriangle.i[0] * 3) + 2]);

			vertices[1].setXYZ(vertexArray[(firstTriangle.i[1] * 3) + 0],
							   vertexArray[(firstTriangle.i[1] * 3) + 1],
							   vertexArray[(firstTriangle.i[1] * 3) + 2]);

			vertices[2].setXYZ(vertexArray[(firstTriangle.i[2] * 3) + 0],
							   vertexArray[(firstTriangle.i[2] * 3) + 1],
							   vertexArray[(firstTriangle.i[2] * 3) + 2]);

			MathLib::vector4_sub(vertices[2], vertices[0], u);
			MathLib::vector4_sub(vertices[1], vertices[0], v);
			MathLib::vector4_crossProduct(u, v, normal);
			MathLib::vector4_normalize(normal);
			MathLib::vector4_copy(triangleNormals[currentTriangleIndex], normal);

			// Generate second triangle.
			IndexedTriangle& secondTriangle = triangles[++currentTriangleIndex];
			secondTriangle.i[0] = firstTriangle.i[1];
			secondTriangle.i[1] = (x + 1) + ((y + 1) * (gridSize + 1));
			secondTriangle.i[2] = firstTriangle.i[2];

			// Compute normal for second triangle.
			vertices[0].setXYZ(vertexArray[(secondTriangle.i[0] * 3) + 0],
							   vertexArray[(secondTriangle.i[0] * 3) + 1],
							   vertexArray[(secondTriangle.i[0] * 3) + 2]);

			vertices[1].setXYZ(vertexArray[(secondTriangle.i[1] * 3) + 0],
							   vertexArray[(secondTriangle.i[1] * 3) + 1],
							   vertexArray[(secondTriangle.i[1] * 3) + 2]);

			vertices[2].setXYZ(vertexArray[(secondTriangle.i[2] * 3) + 0],
							   vertexArray[(secondTriangle.i[2] * 3) + 1],
							   vertexArray[(secondTriangle.i[2] * 3) + 2]);

			MathLib::vector4_sub(vertices[0], vertices[1], u);
			MathLib::vector4_sub(vertices[2], vertices[1], v);
			MathLib::vector4_crossProduct(u, v, normal);
			MathLib::vector4_normalize(normal);
			MathLib::vector4_copy(triangleNormals[currentTriangleIndex], normal);

			currentTriangleIndex++;
		}
	}

	// Compute the averaged per vertex normals.
	// For each vertex, find all triangles involved in that vertex, and average their normals.
	vector<MathLib::vector4*>* perVertexNormalInfluences = new vector<MathLib::vector4*>[numVertices];

	for (unsigned int i = 0; i < numTriangles; i++)
	{
		// Store triangle normal in bins.
		for (unsigned int currentIndex = 0; currentIndex < 3; currentIndex++)
			perVertexNormalInfluences[triangles[i].i[currentIndex]].push_back(triangleNormals + i);
	}

	// Now average all of the normals and assign them to the normalArray.
	for (unsigned int i = 0; i < numVertices; i++)
	{
		MathLib::vector4 aggregateNormal(0.0f, 0.0f, 0.0f, 0.0f);

		if (perVertexNormalInfluences[i].size() <= 0)
			continue;

		const float normalInfluenceMultiplier = 1.0f / static_cast<float>(perVertexNormalInfluences[i].size());
		for (unsigned int j = 0; j < perVertexNormalInfluences[i].size(); j++)
			MathLib::vector4_addScaledVector(aggregateNormal, *perVertexNormalInfluences[i][j], normalInfluenceMultiplier, aggregateNormal);

		MathLib::vector4_normalize(aggregateNormal);
		normalArray[(i * 3) + 0] = aggregateNormal.extractX();
		normalArray[(i * 3) + 1] = aggregateNormal.extractY();
		normalArray[(i * 3) + 2] = aggregateNormal.extractZ();
	}

	delete [] perVertexNormalInfluences;

	// Next create the index array.
	GLuint* indexArray = new GLuint[numTriangles * 3];
	for (unsigned int i = 0; i < numTriangles; i++)
	{
		indexArray[(i * 3) + 0] = triangles[i].i[0];
		indexArray[(i * 3) + 1] = triangles[i].i[1];
		indexArray[(i * 3) + 2] = triangles[i].i[2];
	}

	// Texture coord array.
	GLfloat* texCoordArray = new GLfloat[numVertices * 2];
	for (unsigned int i = 0; i < numVertices; i++)
	{
		texCoordArray[(i * 2) + 0] = static_cast<GLfloat>((i % (gridSize + 1))) / (gridSize);
		texCoordArray[(i * 2) + 1] = static_cast<GLfloat>(gridSize - (i / (gridSize + 1))) / (gridSize);
	}

	const unsigned int vertexArraySizeBytes = numVertices * 3 * sizeof(GLfloat);
	const unsigned int normalArraySizeBytes = numVertices * 3 * sizeof(GLfloat);
	const unsigned int texCoordArraySizeBytes = numVertices * 2 * sizeof(GLfloat);
	const unsigned int indexArraySizeBytes = numTriangles * 3 * sizeof(GLuint);

	numIndices = numTriangles * 3;

	RendererUtils::GenerateVBO(&dataVBOHandle,
							   &indexVBOHandle,
							   vertexArraySizeBytes,
							   vertexArray,
							   texCoordArraySizeBytes,
							   texCoordArray,
							   normalArraySizeBytes,
							   normalArray,
							   indexArraySizeBytes,
							   indexArray);

	delete [] vertexArray;
	delete [] normalArray;
	delete [] texCoordArray;
	delete [] indexArray;
	delete [] triangles;
	delete [] triangleNormals;
}

std::vector<RenderToken*> RendererTerrainNaive::GetRenderTokens(const CameraLib::Camera& camera)
{
	return std::vector<RenderToken*>();
}

}
