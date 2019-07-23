#ifndef STATICMESH_H_INCLUDED
#define STATICMESH_H_INCLUDED

#include <string>
#include <vector>
#include <map>
#include <MathLib.h>
#include <BoundingVolumes.h>
#include <StaticMeshDBIndex.h>
#include "RendererLibCommon.h"

namespace RendererLib
{

struct StaticMeshLODLevel
{
	GLuint dataVBOHandle;
	GLuint indexVBOHandle;

	unsigned int numVertices;
	unsigned int numIndices;

	unsigned int vertexArraySizeBytes;
	unsigned int texCoordArraySizeBytes;
	unsigned int normalArraySizeBytes;
	unsigned int indexArraySizeBytes;

	/// Distance at which the mesh will transition to this level.
	float distance;
};

class StaticMesh
{
	public:

        /// These fields are useful for debugging memory size information.
		unsigned int numVertices;
		unsigned int numIndices;

		unsigned int vertexArraySizeBytes;
		unsigned int texCoordArraySizeBytes;
		unsigned int normalArraySizeBytes;
		unsigned int indexArraySizeBytes;

        /// VBO Handles.
		GLuint dataVBOHandle;
		GLuint indexVBOHandle;

		/// LOD levels.
		std::vector<const StaticMeshLODLevel*> lodLevels;

		StaticMesh(uint32_t numVertices,
				   const float* const vertexArray,
				   const float* const texCoordArray,
				   const float* const normalArray,
				   uint32_t numIndices,
				   const uint32_t* const indexArray);

		~StaticMesh();

        const BoundingVolumes::ObjectSpaceMeshData& GetObjectSpaceMeshData() const
        {
            return objectSpaceMeshData;
        }

		void AddLodLevel(StaticMeshLODLevel* newLevel);

		void AddLodLevel(float distance, 
						 GLuint dataVBOHandle,
						 GLuint indexVBOHandle,
						 unsigned int numVertices,
						 unsigned int numIndices,
						 unsigned int vertexArraySizeBytes,
						 unsigned int texCoordArraySizeBytes,
						 unsigned int normalArraySizeBytes,
						 unsigned int indexArraySizeBytes);

		/// Distance at which the mesh will transition to this level.
		float distance;

    protected:

        BoundingVolumes::ObjectSpaceMeshData objectSpaceMeshData;
};

class StaticMeshManager
{
    public:

        /**
        * When the StaticMeshManager is deleted, it deletes all of its StaticMesh children, and frees any
        * video memory occupied by those meshes.
        */
        virtual ~StaticMeshManager();

        /**
        * Returns a pointer to the specified mesh.
        * If the mesh is not within the scope of the manager, the mesh database is searched.
        * If in the database, the mesh is loaded and a pointer to it returned, otherwise
        * this method returns NULL.
        */
        const StaticMesh* GetMesh(std::string meshIdentifier);

        /**
        * If not already present, this method adds the specified mesh to the scope of this mesh manager.
        */
        void AddMesh(std::string meshName, StaticMesh* mesh);

        /**
        * If present, this method removes the mesh with the given name from the scope of the mesh manager.
        * This method frees the memory occupied by the mesh, so you do not have to manually delete it.
        */
        void RemoveMesh(std::string meshName);

        /**
        * Removes all mesh data from this mesh manager and returns it to a blank state.
        */
        void Reset();

		/**
		* Returns the number of elements in the map (i.e the number of meshes in the map).
		*/
        uint32_t GetSize();

		/**
		* Sets the database file associated with this static mesh manager. If a requested mesh instance in not already available, then
		* this database is searched for the requisite data.
		*/
		void SetMeshDatabase(FILE* file);

		/**
		* Closes the current database file.
		* If no file is open, this method does nothing.
		*/
		void CloseMeshDatabase();

		const StaticMeshDBIndex& GetMeshDatabase() const { return databaseIndex; }

		/**
		* Returns the identifier of the mesh at the given index.
		*/
        std::string GetIndexName(uint32_t index);

        static StaticMeshManager& GetInstance();

    protected:

		FILE* meshDatabase;

		StaticMeshDBIndex databaseIndex;

        std::map<std::string, StaticMesh*> meshMap;

        StaticMeshManager();

		StaticMesh* defaultMesh;

        static StaticMeshManager* managerInstance;

		void CreateDefaultMesh();
};

class StaticMeshInstance
{
	public:

		const StaticMesh* meshData;

		std::string texturePath;

        MathLib::quaternion orientation;

		MathLib::matrix4x4 parentTransform;
		/// Every static mesh instance may need to be hierarchically transformed, and so this 
		/// member is to store the parent transform in that hierarchy.

        float scale;

        float positionX;
        float positionY;
        float positionZ;

        float scaleU;
        float scaleV;

		StaticMeshInstance(const StaticMesh* const meshData, std::string texturePath);

        ~StaticMeshInstance();

		void Update();
		/// Updates the internal state of this instance. Should be called once per frame.

        const BoundingVolumes::BoundingVolume* GetBoundingVolume() const
        {
            return boundingVolume;
        }

    protected:

        BoundingVolumes::BoundingVolumeObjectSpace* boundingVolume;
};

}

#endif // STATICMESH_H_INCLUDED
