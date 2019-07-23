#include "SubRendererStaticMesh.h"
#include "Renderer.h"
#include "ShaderManager.h"
#include "Texture.h"
#include "RenderTokenStaticMesh.h"
#include <cassert>
#include <cstdlib>
#include <iostream>

#pragma warning(push)
#pragma warning(disable:4312)

using namespace RendererLib;

SubRendererStaticMesh::SubRendererStaticMesh(Renderer& renderer, const std::vector<RenderToken*>& set) :
	SubRenderer(renderer),
	renderableSet(set)
{
}

void SubRendererStaticMesh::SetContextState()
{
	glCullFace(GL_BACK);

	glEnable(GL_DEPTH_TEST);

	// Initialize shader
	ShaderManager& shaderManager = ShaderManager::GetInstance();

	staticMeshShader = shaderManager.GetShader(ShaderManager::SHADER_DEFAULT_MESH);
	assert(NULL != staticMeshShader);
	staticMeshShader->Use();

	GLuint programHandle = staticMeshShader->GetProgramHandle();
	attributeTexCoord0Index = glGetAttribLocation(programHandle, "aTexCoord0");
	attributeNormalIndex = glGetAttribLocation(programHandle, "aNormal");

	if (rendererHandle.GetRenderWireframe())
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	else
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	glEnableClientState(GL_VERTEX_ARRAY);
}

void SubRendererStaticMesh::Render()
{
	TextureManager& textureManager = TextureManager::GetInstance();

	for (int i = 0; i < renderableSet.size(); i++)
	{
		RenderTokenStaticMesh* currentToken = reinterpret_cast<RenderTokenStaticMesh*>(renderableSet[i]);
		assert(NULL != currentToken);

		const StaticMeshInstance& currentInstance = *currentToken->mesh;
		const StaticMesh& staticMeshData = *currentInstance.meshData;

		if (!currentInstance.GetBoundingVolume()->IsVisibleFromFrustum(rendererHandle.GetMainCamera().GetFrustumPlanes()))
		{
			continue;
		}

		glBindTexture(GL_TEXTURE_2D, textureManager.GetTexture(currentInstance.texturePath));

		// Set up transform matrix.
		MathLib::matrix4x4 modelTransform;
		MathLib::GenerateTransformMatrix(modelTransform, 
										 MathLib::vector4(currentToken->mesh->positionX, currentToken->mesh->positionY, currentToken->mesh->positionZ, 1.0f), 
										 currentToken->mesh->orientation, 
										 currentToken->mesh->scale);

		MathLib::matrix4x4 globalTransform;
		MathLib::matrix4x4_mul(currentToken->mesh->parentTransform, modelTransform, globalTransform);
		MathLib::matrix4x4_copy(rendererHandle.matrices[MATRIX_OBJECT_WORLD_INDEX], globalTransform);
		rendererHandle.UpdateModelViewProjectionMatrix();

		rendererHandle.matrices[MATRIX_MVP_INDEX].setOpenGLMatrix(matrixArray);
		staticMeshShader->SetUniformMatrix4fv("transform", static_cast<GLfloat*>(matrixArray), false);

		MathLib::matrix4x4 inverseMatrix;
		MathLib::matrix4x4_inverse(rendererHandle.matrices[MATRIX_MODEL_VIEW_INDEX], inverseMatrix);
		MathLib::matrix4x4_transpose(inverseMatrix);
		inverseMatrix.setOpenGLMatrix(matrixArray);
		staticMeshShader->SetUniformMatrix4fv("modelTransform", static_cast<GLfloat*>(matrixArray), false);

		// Find the correct lod level to use.
		GLuint dataVBOHandle = staticMeshData.dataVBOHandle;
		GLuint indexVBOHandle = staticMeshData.indexVBOHandle;

		int vertexArraySizeBytes = staticMeshData.vertexArraySizeBytes;
		int texCoordArraySizeBytes = staticMeshData.texCoordArraySizeBytes;
		int numIndices = staticMeshData.numIndices;

		MathLib::vector4 worldSpacePosition(currentInstance.positionX, currentInstance.positionY, currentInstance.positionZ, 1.0f);

		MathLib::matrix4x4_vectorMul(globalTransform, worldSpacePosition, worldSpacePosition);

		float distanceToMesh = MathLib::vector4_distance(rendererHandle.GetMainCamera().GetPosition(),
														 worldSpacePosition);

		for (int j = 0; j < staticMeshData.lodLevels.size(); j++)
		{
			if (staticMeshData.lodLevels[j]->distance < distanceToMesh)
			{
				// Can we advance to next lod level?
				int k = j + 1;
				if (k < staticMeshData.lodLevels.size() && 
					staticMeshData.lodLevels[k]->distance < distanceToMesh)
				{
					continue;
				}

				dataVBOHandle = staticMeshData.lodLevels[j]->dataVBOHandle;
				indexVBOHandle = staticMeshData.lodLevels[j]->indexVBOHandle;

				vertexArraySizeBytes = staticMeshData.lodLevels[j]->vertexArraySizeBytes;
				texCoordArraySizeBytes = staticMeshData.lodLevels[j]->texCoordArraySizeBytes;

				numIndices = staticMeshData.lodLevels[j]->numIndices;
			}
		}

		glBindBuffer(GL_ARRAY_BUFFER, dataVBOHandle);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexVBOHandle);

		// Set attribute offsets into VBO
		glVertexPointer(3, GL_FLOAT, 0, 0);

		glEnableVertexAttribArray(attributeTexCoord0Index);
		glVertexAttribPointer(attributeTexCoord0Index, 2, GL_FLOAT, false, 0, reinterpret_cast<GLvoid*>(vertexArraySizeBytes));

		glEnableVertexAttribArray(attributeNormalIndex);
		glVertexAttribPointer(attributeNormalIndex, 3, GL_FLOAT, false, 0, reinterpret_cast<GLvoid*>(vertexArraySizeBytes + texCoordArraySizeBytes));

		glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, 0);

		rendererHandle.numTrianglesDrawnLastFrame += numIndices / 3;
	}
}

void SubRendererStaticMesh::RestoreContextState()
{
	glEnable(GL_DEPTH_TEST);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	ShaderProgram::DeactivateShaders();

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableVertexAttribArray(attributeTexCoord0Index);
	glDisableVertexAttribArray(attributeNormalIndex);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

#pragma warning(pop)