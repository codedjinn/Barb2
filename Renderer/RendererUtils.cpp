#include "RendererUtils.h"
#include "StaticMesh.h"
#include <cassert>
#include <StoredStaticMesh.h>
#include <Compression.h>

namespace RendererLib
{

void RendererUtils::TransformMatrixFromVectors(MathLib::matrix4x4& transform,
											   const MathLib::vector4& position,
											   const MathLib::vector4& rotation,
											   const MathLib::vector4& scale)
{
	MathLib::GenerateTransformMatrix(transform, position, rotation, scale);
}

void RendererUtils::TransformMatrixFromPQS(MathLib::matrix4x4& transform,
										   const MathLib::vector4& position,
										   const MathLib::quaternion& orientation,
										   const float scale)
{
	MathLib::GenerateTransformMatrix(transform, position, orientation, scale);
}

void RendererUtils::GenerateVBO(GLuint* dataVBOHandle,
								GLuint* indexVBOHandle,
								const unsigned int vertexArraySizeBytes,
								float const * const vertexArray,
								const unsigned int texCoordArraySizeBytes,
								float const * const texCoordArray,
								const unsigned int indexArraySizeBytes,
								uint32_t const * const indexArray)
{
	// Generate the data VBO.
	glGenBuffers(1, dataVBOHandle);
	glBindBuffer(GL_ARRAY_BUFFER, *dataVBOHandle);

	// Reserve space for the data store.
	glBufferData(GL_ARRAY_BUFFER,
				 vertexArraySizeBytes + texCoordArraySizeBytes,
				 NULL,
				 GL_STATIC_DRAW);

	// Transfer vertex data.
	glBufferSubData(GL_ARRAY_BUFFER,
					0,
					vertexArraySizeBytes,
					vertexArray);

	// Transfer texture coordinate data.
	glBufferSubData(GL_ARRAY_BUFFER,
					vertexArraySizeBytes,
					texCoordArraySizeBytes,
					texCoordArray);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Generate the index VBO.
	glGenBuffers(1, indexVBOHandle);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *indexVBOHandle);

	// Allocate and transfer the data store.
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,
				 indexArraySizeBytes,
				 indexArray,
				 GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void RendererUtils::GenerateVBO(GLuint* dataVBOHandle,
								GLuint* indexVBOHandle,
								const unsigned int vertexArraySizeBytes,
								float const * const vertexArray,
								const unsigned int texCoordArraySizeBytes,
								float const * const texCoordArray,
								const unsigned int normalArraySizeBytes,
								float const * const normalArray,
								const unsigned int indexArraySizeBytes,
								uint32_t const * const indexArray)
{
	// Generate the data VBO.
	glGenBuffers(1, dataVBOHandle);
	glBindBuffer(GL_ARRAY_BUFFER, *dataVBOHandle);

	// Reserve space for the data store.
	glBufferData(GL_ARRAY_BUFFER,
				 vertexArraySizeBytes + texCoordArraySizeBytes + normalArraySizeBytes,
				 NULL,
				 GL_STATIC_DRAW);

	// Transfer vertex data.
	glBufferSubData(GL_ARRAY_BUFFER,
					0,
					vertexArraySizeBytes,
					vertexArray);

	// Transfer texture coordinate data.
	glBufferSubData(GL_ARRAY_BUFFER,
					vertexArraySizeBytes,
					texCoordArraySizeBytes,
					texCoordArray);

	// Transform normal data.
	glBufferSubData(GL_ARRAY_BUFFER,
					vertexArraySizeBytes + texCoordArraySizeBytes,
					normalArraySizeBytes,
					normalArray);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Generate the index VBO.
	glGenBuffers(1, indexVBOHandle);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *indexVBOHandle);

	// Allocate and transfer the data store.
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,
				 indexArraySizeBytes,
				 indexArray,
				 GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void RendererUtils::GenerateVBO(GLuint* vboHandle, std::vector<GeometryLib::Triangle>& triangleArray)
{
	const auto numVertices = triangleArray.size() * 3;

	float* positionArray = new float[numVertices * 3];
	float* normalArray = new float[numVertices * 3];
	float* texCoordArray = new float[numVertices * 2];

	const auto positionArraySizeBytes = numVertices * 3 * sizeof(float);
	const auto normalArraySizeBytes = numVertices * 3 * sizeof(float);
	const auto texCoordArraySizeBytes = numVertices * 2 * sizeof(float);

	// Transfer values to arrays.
	unsigned int positionIndex = 0;
	unsigned int normalIndex = 0;
	unsigned int texCoordIndex = 0;

	for (unsigned int i = 0; i < triangleArray.size(); i++)
	{
		for (unsigned int j = 0; j < 3; j++)
		{
			positionArray[positionIndex++] = triangleArray[i].points[j].position.extractX();
			positionArray[positionIndex++] = triangleArray[i].points[j].position.extractY();
			positionArray[positionIndex++] = triangleArray[i].points[j].position.extractZ();

			normalArray[normalIndex++] = triangleArray[i].points[j].normal.extractX();
			normalArray[normalIndex++] = triangleArray[i].points[j].normal.extractY();
			normalArray[normalIndex++] = triangleArray[i].points[j].normal.extractZ();

			texCoordArray[texCoordIndex++] = triangleArray[i].points[j].texCoord.extractX();
			texCoordArray[texCoordIndex++] = triangleArray[i].points[j].texCoord.extractY();
		}
	}
	
	// Generate the vertex buffer object.
	
	glGenBuffers(1, vboHandle);
	glBindBuffer(GL_ARRAY_BUFFER, *vboHandle);

	// Reserve space for the data store.
	glBufferData(GL_ARRAY_BUFFER,
				 positionArraySizeBytes + texCoordArraySizeBytes + normalArraySizeBytes,
				 NULL,
				 GL_STATIC_DRAW);

	// Transfer position data.
	glBufferSubData(GL_ARRAY_BUFFER,
					0,
					positionArraySizeBytes,
					positionArray);

	// Transfer texture coordinate data.
	glBufferSubData(GL_ARRAY_BUFFER,
					positionArraySizeBytes,
					texCoordArraySizeBytes,
					texCoordArray);

	// Transfer normal data.
	glBufferSubData(GL_ARRAY_BUFFER,
					positionArraySizeBytes + texCoordArraySizeBytes,
					normalArraySizeBytes,
					normalArray);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	
	delete[] positionArray;
	delete[] normalArray;
	delete[] texCoordArray;
}

void RendererUtils::DeleteVBO(GLuint* vboHandle)
{
	glDeleteBuffers(1, vboHandle);
}

StaticMesh* RendererUtils::StaticMeshFromStoredStaticMesh(StoredStaticMesh const * const mesh)
{
	// Uncompress and retrieve vertex array.
	float* vertexArray = mesh->GetVertexArray();

	// Uncompress and retrieve texture coordinate array.
	float* texCoord0Array = mesh->GetTexCoordArray();

	// Uncompress and retrieve normal array.
	float* normalArray = mesh->GetNormalArray();

	// Uncompress and retrieve index array.
	uint32_t* indicesArray = mesh->GetIndexArray();

	StaticMesh* newMesh = new StaticMesh(mesh->GetNumVertices(), vertexArray, texCoord0Array, normalArray, mesh->GetNumIndices(), indicesArray);

	delete[] vertexArray;
	delete[] texCoord0Array;
	delete[] indicesArray;

	std::vector<StoredStaticMeshLODLevel> lodLevels = mesh->GetLODLevels();
	for (int i = 0; i < lodLevels.size(); i++)
	{
		GLuint vboHandle;
		GLuint indexHandle;

		unsigned int vertexArraySizeBytes = lodLevels[i].numVertices * 3 * sizeof(float);
		unsigned int texCoordArraySizeBytes = lodLevels[i].numVertices * 2 * sizeof(float);
		unsigned int normalArraySizeBytes = vertexArraySizeBytes;
		unsigned int indexArraySizeBytes = lodLevels[i].numIndices * sizeof(uint32_t);

		RendererUtils::GenerateVBO(&vboHandle,
								   &indexHandle,
								   vertexArraySizeBytes,
								   lodLevels[i].vertexArray,
								   texCoordArraySizeBytes,
								   lodLevels[i].texCoordArray,
								   normalArraySizeBytes,
								   lodLevels[i].normalArray,
								   indexArraySizeBytes,
								   lodLevels[i].indexArray);

		newMesh->AddLodLevel(lodLevels[i].distance,
							 vboHandle,
							 indexHandle,
							 lodLevels[i].numVertices,
							 lodLevels[i].numIndices,
							 vertexArraySizeBytes,
							 texCoordArraySizeBytes,
							 normalArraySizeBytes,
							 indexArraySizeBytes);

		delete[] lodLevels[i].vertexArray;
		delete[] lodLevels[i].texCoordArray;
		delete[] lodLevels[i].normalArray;
		delete[] lodLevels[i].indexArray;
	}

	return newMesh;
}

void RendererUtils::ExtractAtrributeDataFromVBO(GLuint handle, unsigned int numVertices, float** vertexArrayPtr, float** texCoordArrayPtr, float** normalArrayPtr)
{
	assert(sizeof(GLfloat) == sizeof(float));

	const unsigned int vertexArraySizeBytes = numVertices * 3 * sizeof(GLfloat);
	const unsigned int texCoordArraySizeBytes = numVertices * 2 * sizeof(GLfloat);
	const unsigned int normalArraySizeBytes = vertexArraySizeBytes;

	float*& vertexArray = *vertexArrayPtr;
	float*& texCoordArray = *texCoordArrayPtr;
	float*& normalArray = *normalArrayPtr;

	vertexArray = new float[numVertices * 3];
	texCoordArray = new float[numVertices * 2];
	normalArray = new float[numVertices * 3];

	glBindBuffer(GL_ARRAY_BUFFER, handle);
	uint8_t* mappedMemory = reinterpret_cast<uint8_t*>(glMapBuffer(GL_ARRAY_BUFFER, GL_READ_ONLY));

	memcpy(vertexArray, mappedMemory, vertexArraySizeBytes);
	memcpy(texCoordArray, mappedMemory + vertexArraySizeBytes, texCoordArraySizeBytes);
	memcpy(normalArray, mappedMemory + vertexArraySizeBytes + texCoordArraySizeBytes, normalArraySizeBytes);

	assert(glUnmapBuffer(GL_ARRAY_BUFFER));
	
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void RendererUtils::ExtractIndexDataFromVBO(GLuint handle, unsigned int numIndices, uint32_t** indexArrayPtr)
{
	assert(sizeof(GLuint) == sizeof(uint32_t));

	uint32_t*& indexArray = *indexArrayPtr;

	indexArray = new uint32_t[numIndices];

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, handle);
	void* mappedMemory = glMapBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_READ_ONLY);
	
	memcpy(indexArray, mappedMemory, numIndices * sizeof(uint32_t));

	assert(glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER));
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

}
