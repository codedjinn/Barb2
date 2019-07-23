#include "StaticMesh.h"
#include "RendererUtils.h"
#include <StoredStaticMesh.h>
#include <limits>
#include <cassert>

namespace RendererLib
{
StaticMeshManager* StaticMeshManager::managerInstance   = NULL;
}

using namespace RendererLib;

StaticMesh::StaticMesh(uint32_t numVertices,
					   const float* const vertexArray,
					   const float* const texCoordArray,
					   const float* const normalArray,
					   uint32_t numIndices,
					   const uint32_t* const indexArray)
{
    GLfloat* vertexData;
    GLfloat* texCoordData;
    GLfloat* normalData;
    GLuint* indexData;

    // First copy the vertex data
	StaticMesh::numVertices	= numVertices;
    vertexData = new GLfloat[numVertices * 3];
    texCoordData = new GLfloat[numVertices * 2];
    normalData = new GLfloat[numVertices * 3];

	for (unsigned int i = 0; i < numVertices * 3; i++)
	{
		vertexData[i] = static_cast<GLfloat>(vertexArray[i]);
	}

	for (unsigned int i = 0; i < numVertices * 2; i++)
	{
		texCoordData[i]	= static_cast<GLfloat>(texCoordArray[i]);
	}

	for (unsigned int i = 0; i < numVertices * 3; i++)
	{
        normalData[i] = static_cast<GLfloat>(normalArray[i]);
	}

    // Then copy the index data
	StaticMesh::numIndices = numIndices;
	indexData = new GLuint[numIndices];

	for (unsigned int i = 0; i < numIndices; i++)
	{
		indexData[i] = indexArray[i];
	}

    vertexArraySizeBytes = numVertices * sizeof(GLfloat) * 3;
    texCoordArraySizeBytes = numVertices * sizeof(GLfloat) * 2;
    normalArraySizeBytes = numVertices * sizeof(GLfloat) * 3;
	indexArraySizeBytes = numIndices * sizeof(uint32_t);

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

    objectSpaceMeshData.GenerateBoundsInformation(vertexData, numVertices);

    delete [] vertexData;
    delete [] texCoordData;
    delete [] indexData;
}

StaticMesh::~StaticMesh()
{
    RendererUtils::DeleteVBO(&dataVBOHandle);
    RendererUtils::DeleteVBO(&indexVBOHandle);

	for (int i = 0; i < lodLevels.size(); i++)
	{
		delete lodLevels[i];
	}
}

void StaticMesh::AddLodLevel(StaticMeshLODLevel* newLevel)
{
	assert(NULL != newLevel);
	lodLevels.push_back(newLevel);
}

void StaticMesh::AddLodLevel(float distance,
							 GLuint dataVBOHandle,
							 GLuint indexVBOHandle,
							 unsigned int numVertices,
							 unsigned int numIndices,
							 unsigned int vertexArraySizeBytes,
							 unsigned int texCoordArraySizeBytes,
							 unsigned int normalArraySizeBytes,
							 unsigned int indexArraySizeBytes)
{
	StaticMeshLODLevel* newLodLevel = new StaticMeshLODLevel;
	newLodLevel->distance = distance;
	newLodLevel->dataVBOHandle = dataVBOHandle;
	newLodLevel->indexVBOHandle = indexVBOHandle;
	newLodLevel->numVertices = numVertices;
	newLodLevel->numIndices = numIndices;
	newLodLevel->vertexArraySizeBytes = vertexArraySizeBytes;
	newLodLevel->texCoordArraySizeBytes = texCoordArraySizeBytes;
	newLodLevel->normalArraySizeBytes = normalArraySizeBytes;
	newLodLevel->indexArraySizeBytes = indexArraySizeBytes;

	lodLevels.push_back(newLodLevel);
}

StaticMeshInstance::StaticMeshInstance(const StaticMesh* const meshData, std::string texturePath)
{
	StaticMeshInstance::texturePath	= texturePath;
	StaticMeshInstance::meshData	= meshData;

    scale = 1.0f;

    MathLib::quaternion_setToIdentity(orientation);

    scaleU = 1.0f;
    scaleV = 1.0f;

	parentTransform.loadIdentity();

    boundingVolume = new BoundingVolumes::BoundingOOBB(&meshData->GetObjectSpaceMeshData());
}

StaticMeshInstance::~StaticMeshInstance()
{
    if (NULL != boundingVolume)
    {
        delete boundingVolume;
    }
}

void StaticMeshInstance::Update()
{
    boundingVolume->GenerateInstanceBounds(MathLib::vector4(positionX, positionY, positionZ, 1.0f),
                                           orientation,
                                           scale, 
										   parentTransform);
}

StaticMeshManager::StaticMeshManager()
{
	meshDatabase = NULL;
	defaultMesh = NULL;
}

StaticMeshManager::~StaticMeshManager()
{
    // Delete all of the child mesh objects
    std::map<std::string, StaticMesh*>::iterator meshIterator = meshMap.begin();

    while (meshIterator != meshMap.end())
    {
        delete meshIterator->second;
        meshIterator++;
    }

    meshMap.clear();
}

const StaticMesh* StaticMeshManager::GetMesh(std::string meshName)
{
    std::map<std::string, StaticMesh*>::const_iterator meshIterator = meshMap.find(meshName);
    if (meshMap.end() == meshIterator)
    {
    	// Load from database if available
    	int offset = databaseIndex.OffsetOfMesh(meshName);
    	if (-1 == offset)
    	{
            if (NULL == defaultMesh)
            {
                CreateDefaultMesh();
            }

			// Mesh not found, return default mesh.
			return defaultMesh;
    	}
    	else
    	{
			// We have the offset, we can now load the mesh from the HDD, and initialize its GPU form
			fseek(meshDatabase,
				  static_cast<long>(offset),
				  SEEK_SET);

			StoredStaticMesh storedStaticMesh;
			storedStaticMesh.Load(meshDatabase);

			StaticMesh* staticMesh = RendererUtils::StaticMeshFromStoredStaticMesh(&storedStaticMesh);

			AddMesh(meshName, staticMesh);

			return staticMesh;
    	}
    }

    return meshIterator->second;
}

void StaticMeshManager::AddMesh(std::string meshName, StaticMesh* mesh)
{
    std::map<std::string, StaticMesh*>::iterator meshIterator   = meshMap.find(meshName);

    // Check if the mesh is present in the map
    if (meshMap.end() != meshIterator)
    {
        // If so, then no work needs to be done
        return;
    }

    meshMap.insert(std::pair<std::string, StaticMesh*>(meshName, mesh));
}

void StaticMeshManager::RemoveMesh(std::string meshName)
{
    std::map<std::string, StaticMesh*>::iterator meshIterator   = meshMap.find(meshName);

    // Check if the mesh is in the map
    if (meshMap.end() == meshIterator)
    {
        // No it isn't, no work needs to be done
        return;
    }

    delete meshIterator->second;

    meshMap.erase(meshIterator);
}

void StaticMeshManager::Reset()
{
    using namespace std;

    map<string, StaticMesh*>::iterator meshIterator = meshMap.begin();

    while (meshIterator != meshMap.end())
    {
        delete meshIterator->second;

        meshIterator++;
    }

    meshMap.clear();
}

uint32_t StaticMeshManager::GetSize()
{
	return (uint32_t)meshMap.size();
}

std::string StaticMeshManager::GetIndexName(uint32_t index)
{
	std::map<std::string, StaticMesh*>::iterator iterator = meshMap.begin();
	for (uint32_t i = 0; i < index; i++)
	{
		iterator++;
	}

	return iterator->first;
}

void StaticMeshManager::SetMeshDatabase(FILE* file)
{
	assert(NULL != file);

	meshDatabase = file;

	databaseIndex.Load(meshDatabase);
	databaseIndex.PrintIndex();
}

void StaticMeshManager::CloseMeshDatabase()
{
	if (NULL != meshDatabase)
	{
		fclose(meshDatabase);
	}
}

void StaticMeshManager::CreateDefaultMesh()
{
	unsigned int numVertices = 24;

	float* vertexArray = new float[24 * 3]
	{
		// Front face.
		-1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, -1.0f, 1.0f,
		-1.0f, -1.0f, 1.0f, 

		// Right face.
		1.0f, 1.0f, 1.0f, 
		1.0f, 1.0f, -1.0f, 
		1.0f, -1.0f, -1.0f, 
		1.0f, -1.0f, 1.0f, 

		// Back face.
		1.0f, 1.0f, -1.0f, 
		-1.0f, 1.0f, -1.0f, 
		-1.0f, -1.0f, -1.0f, 
		1.0f, -1.0f, -1.0f, 

		// Left face.
		-1.0f, 1.0f, -1.0f, 
		-1.0f, 1.0f, 1.0f, 
		-1.0f, -1.0f, 1.0f, 
		-1.0f, -1.0f, -1.0f, 

		// Top face.
		-1.0f, 1.0f, -1.0f, 
		1.0f, 1.0f, -1.0f, 
		1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f, 1.0f,

		// Bottom face.
		-1.0f, -1.0f, 1.0f, 
		1.0f, -1.0f, 1.0f, 
		1.0f, -1.0f, -1.0f, 
		-1.0f, -1.0f, -1.0f
	};

	float* texCoordArray = new float[24 * 2]
	{
		// Front face.
		0.0f, 1.0f,
		1.0f, 1.0f,
		1.0f, 0.0f,
		0.0f, 0.0f,

		// Right face.
		0.0f, 1.0f,
		1.0f, 1.0f,
		1.0f, 0.0f,
		0.0f, 0.0f,

		// Back face.
		0.0f, 1.0f, 
		1.0f, 1.0f, 
		1.0f, 0.0f, 
		0.0f, 0.0f, 

		// Left face.
		0.0f, 1.0f,
		1.0f, 1.0f,
		1.0f, 0.0f,
		0.0f, 0.0f, 

		// Top face.
		0.0f, 1.0f,
		1.0f, 1.0f,
		1.0f, 0.0f,
		0.0f, 0.0f,

		// Bottom face.
		0.0f, 1.0f,
		1.0f, 1.0f,
		1.0f, 0.0f,
		0.0f, 0.0f,
	};

	float* normalArray = new float[24 * 3]
	{
		// Front face.
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,

		// Right face.
		1.0f, 0.0f, 0.0f, 
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f, 

		// Back face.
		0.0f, 0.0f, -1.0f,
		0.0f, 0.0f, -1.0f,
		0.0f, 0.0f, -1.0f,
		0.0f, 0.0f, -1.0f,

		// Left face.
		-1.0f, 0.0f, 0.0f,
		-1.0f, 0.0f, 0.0f,
		-1.0f, 0.0f, 0.0f,
		-1.0f, 0.0f, 0.0f,

		// Top face.
		0.0f, 1.0f, 0.0f, 
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,

		// Bottom face.
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f
	};

	uint32_t* indexArray = new uint32_t[36]
	{
		// Front face.
		0, 1, 3,
		1, 2, 3,

		// Right face.
		4, 5, 7, 
		5, 6, 7, 

		// Back face.
		8, 9, 11, 
		9, 10, 11,

		// Left face.
		12, 13, 15,
		13, 14, 15,

		// Top face.
		16, 17, 19, 
		17, 18, 19,

		// Bottom face.
		20, 21, 23, 
		21, 22, 23
	};

	defaultMesh = new StaticMesh(numVertices, vertexArray, texCoordArray, normalArray, 36, indexArray);

	delete[] vertexArray;
	delete[] texCoordArray;
	delete[] normalArray;
	delete[] indexArray;
}

StaticMeshManager& StaticMeshManager::GetInstance()
{
    if (NULL == managerInstance)
    {
        StaticMeshManager::managerInstance = new StaticMeshManager;
    }

    return *StaticMeshManager::managerInstance;
}









