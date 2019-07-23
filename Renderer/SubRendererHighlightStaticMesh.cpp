#include "SubRendererHighlightStaticMesh.h"
#include "Renderer.h"
#include "ShaderManager.h"
#include "RenderTokenHighlightStaticMesh.h"
#include <cassert>
#include <cstdlib>
#include <iostream>

namespace RendererLib
{

SubRendererHighlightStaticMesh::SubRendererHighlightStaticMesh(Renderer& renderer, const std::vector<RenderToken*>& set) :
	SubRenderer(renderer), 
	renderableSet(set)
{
}

void SubRendererHighlightStaticMesh::SetContextState()
{
	glCullFace(GL_BACK);

	glDisable(GL_DEPTH_TEST);
	glDepthMask(false);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Initialize shader.
	ShaderManager& shaderManager = ShaderManager::GetInstance();
	highlightMeshShader = shaderManager.GetShader(ShaderManager::SHADER_SINGLE_COLOR);
	assert(NULL != highlightMeshShader);
	highlightMeshShader->Use();
	
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

void SubRendererHighlightStaticMesh::Render()
{
	for (unsigned int i = 0; i < renderableSet.size(); i++)
	{
		RenderTokenHighlightStaticMesh* currentToken = reinterpret_cast<RenderTokenHighlightStaticMesh*>(renderableSet[i]);
		assert(NULL != currentToken);

		const StaticMeshInstance& staticMeshInstance = *currentToken->mesh;
		const StaticMesh& staticMeshData = *staticMeshInstance.meshData;

		highlightMeshShader->SetUniform3f("color", 
										  currentToken->color.extractX(), 
										  currentToken->color.extractY(), 
										  currentToken->color.extractZ());

		// Cull if not visible.
		if (!staticMeshInstance.GetBoundingVolume()->IsVisibleFromFrustum(rendererHandle.GetMainCamera().GetFrustumPlanes()))
		{
			continue;
		}

		// Set up transform matrix.
		MathLib::vector4 meshPosition(currentToken->mesh->positionX, currentToken->mesh->positionY, currentToken->mesh->positionZ, 1.0f);

		MathLib::matrix4x4 modelTransform;
		MathLib::GenerateTransformMatrix(modelTransform, 
										 meshPosition, 
										 currentToken->mesh->orientation, 
										 currentToken->mesh->scale);

		MathLib::matrix4x4 globalTransform;
		MathLib::matrix4x4_mul(currentToken->mesh->parentTransform, modelTransform, globalTransform);
		MathLib::matrix4x4_copy(rendererHandle.matrices[MATRIX_OBJECT_WORLD_INDEX], globalTransform);
		
		// Extract MVP matrix, set shader constant.
		rendererHandle.UpdateModelViewProjectionMatrix();
		rendererHandle.matrices[MATRIX_MVP_INDEX].setOpenGLMatrix(matrixArray);
		highlightMeshShader->SetUniformMatrix4fv("transform", reinterpret_cast<GLfloat*>(matrixArray), false);

		// Find the correct LOD level to use.
		GLuint dataVBOHandle = staticMeshData.dataVBOHandle;
		GLuint indexVBOHandle = staticMeshData.indexVBOHandle;

		int vertexArraySizeBytes = staticMeshData.vertexArraySizeBytes;
		int numIndices = staticMeshData.numIndices;

		MathLib::vector4 worldSpacePosition;
		MathLib::matrix4x4_vectorMul(currentToken->mesh->parentTransform, meshPosition, worldSpacePosition);

		const float distanceToMesh = MathLib::vector4_distance(rendererHandle.GetMainCamera().GetPosition(), 
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

				numIndices = staticMeshData.lodLevels[j]->numIndices;
			}
		}

		glBindBuffer(GL_ARRAY_BUFFER, dataVBOHandle);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexVBOHandle);

		// Set attribute offsets into VBO.
		glVertexPointer(3, GL_FLOAT, 0, 0);

		glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, 0);
	}
}

void SubRendererHighlightStaticMesh::RestoreContextState()
{
	glEnable(GL_DEPTH_TEST);
	glDepthMask(true);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	ShaderProgram::DeactivateShaders();

	glDisableClientState(GL_VERTEX_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glDisable(GL_BLEND);
}

}