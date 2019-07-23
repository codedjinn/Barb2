#ifndef STOREDSTATICMESH_H_INCLUDED
#define STOREDSTATICMESH_H_INCLUDED

#include "AttributedBinaryBlob.h"
#include <inttypes.h>
#include <stdio.h>
#include <vector>
#include <string>

/**
* StoredStaticMeshLODLevel.
* Used to transfer lod levels into a stored static mesh.
*/
struct StoredStaticMeshLODLevel
{
	float distance;

	uint32_t numVertices;
	float* vertexArray;
	float* texCoordArray;
	float* normalArray;

	uint32_t numIndices;
	uint32_t* indexArray;
};

/**
* StoredStaticMesh:
* Interchange static mesh format between runtime engine and file system.
*/
class StoredStaticMesh
{
	public:

		StoredStaticMesh();

		~StoredStaticMesh();

		/**
		* Convenience method to return the amount of space that this file will
		* consume on disk, in its current format.
		*/
		uint32_t SizeOnDisk() const;

		void Save(FILE* fp) const;
		void Load(FILE* fp);

		void SetIdentifier(const std::string& identifier);

		/**
		* Convenience method to load just the identifier from a file.
		*/
		void LoadIdentifier(FILE* fp);

		void SetValues(const std::string& identifier,
					   unsigned int numVertices,
					   float* vertexArray,
					   float* texCoordArray,
					   float* normalArray,
					   unsigned int numIndices,
					   uint32_t* indexArray);

		void SetValues(const std::string& identifier,
					   unsigned int numVertices,
					   float* vertexArray,
					   float* texCoordArray,
					   float* normalArray,
					   unsigned int numIndices,
					   uint32_t* indexArray,
					   const std::vector<StoredStaticMeshLODLevel>& lodLevels);

		void AddLODLevel(const StoredStaticMeshLODLevel& lodLevel);

		void DeleteLODLevel(int index);

		std::string GetIdentifier() const;

		unsigned int GetNumVertices() const;

		float* GetVertexArray() const;

		float* GetTexCoordArray() const;

		float* GetNormalArray() const;

		unsigned int GetNumIndices() const;
		
		int GetLODAttributeIndex() const;

		std::vector<StoredStaticMeshLODLevel> GetLODLevels() const;

		uint32_t* GetIndexArray() const;

	protected:

		uint32_t identifierLength;
		char* identifier;

		std::vector<AttributedBinaryBlob*> binaryBlobs;

		void FreeMemory();
};

#endif // STOREDSTATICMESH_H_INCLUDED
