#ifndef RENDERERUTILS_H_INCLUDED
#define RENDERERUTILS_H_INCLUDED

#include <MathLib.h>
#include <inttypes.h>
#include <vector>
#include <Triangle.h>
#include "RendererLibCommon.h"
#include "StaticMesh.h"


class StoredStaticMesh;

namespace RendererLib
{

class RendererUtils
{
	public:

		static void TransformMatrixFromVectors(MathLib::matrix4x4& transform,
											   const MathLib::vector4& position,
											   const MathLib::vector4& rotation,
											   const MathLib::vector4& scale);

        static void TransformMatrixFromPQS(MathLib::matrix4x4& transform,
                                           const MathLib::vector4& position,
                                           const MathLib::quaternion& orientation,
                                           const float scale);

		static void GenerateVBO(GLuint* dataVBOHandle,
								GLuint* indexVBOHandle,
								const unsigned int vertexArraySizeBytes,
								float const * const vertexArray,
								const unsigned int texCoordArraySizeBytes,
								float const * const texCoordArray,
								const unsigned int indexArraySizeBytes,
								uint32_t const * const indexArray);

		static void GenerateVBO(GLuint* dataVBOHandle,
								GLuint* indexVBOHandle,
								const unsigned int vertexArraySizeBytes,
								float const * const vertexArray,
								const unsigned int texCoordArraySizeBytes,
								float const * const texCoordArray,
								const unsigned int normalArraySizeBytes,
								float const * const normalArray,
								const unsigned int indexArraySizeBytes,
								uint32_t const * const indexArray);

		static void GenerateVBO(GLuint* vboHandle, std::vector<GeometryLib::Triangle>& triangleArray);

		static StaticMesh* StaticMeshFromStoredStaticMesh(StoredStaticMesh const * const mesh);

		static float* ExtractVertexArrayFromStoredStaticMesh(StoredStaticMesh const * const mesh);

		static float* ExtractTexCoordArrayFromStoredStaticMesh(StoredStaticMesh const * const mesh);

		static float* ExtractNormalArrayFromStoredStaticMesh(StoredStaticMesh const * const mesh);

		static uint32_t* ExtractIndexArrayFromStoredStaticMesh(StoredStaticMesh const * const mesh);

		static void ExtractAtrributeDataFromVBO(GLuint handle, unsigned int numVertices, float** vertexArrayPtr, float** texCoordArrayPtr, float** normalArrayPtr);

		static void ExtractIndexDataFromVBO(GLuint handle, unsigned int numIndices, uint32_t** indexArrayPtr);
		
        static void DeleteVBO(GLuint* vboHandle);
};

}


#endif // RENDERERUTILS_H_INCLUDED
