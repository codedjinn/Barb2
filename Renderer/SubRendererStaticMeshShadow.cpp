#include "SubRendererStaticMeshShadow.h"
#include "Renderer.h"
#include "ShaderManager.h"
#include "Texture.h"
#include "RenderTokenStaticMesh.h"
#include "DirectionalLightShadowBasis.h"
#include <cassert>
#include <cstdlib>
#include <iostream>

#pragma warning(push)
#pragma warning(disable:4312)

namespace RendererLib
{

SubRendererStaticMeshShadow::SubRendererStaticMeshShadow(Renderer& renderer, const std::vector<RenderToken*>& set, const DirectionalLightShadowBasis& lightShadowBasis) :
	SubRenderer(renderer),
	renderableSet(set), 
	shadowBasis(lightShadowBasis)
{
}

void SubRendererStaticMeshShadow::SetContextState()
{
	glCullFace(GL_BACK);
	glEnable(GL_DEPTH_TEST);

	// Initialize shader
	ShaderManager& shaderManager = ShaderManager::GetInstance();

	shader = shaderManager.GetShader(ShaderManager::SHADER_STATIC_MESH_SHADOW);
	assert(NULL != shader);
	shader->Use();

	GLuint programHandle = shader->GetProgramHandle();
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

void SubRendererStaticMeshShadow::Render()
{
	TextureManager& textureManager = TextureManager::GetInstance();

	for (int i = 0; i < renderableSet.size(); i++)
	{
		RenderTokenStaticMesh* currentToken = reinterpret_cast<RenderTokenStaticMesh*>(renderableSet[i]);

		assert(NULL != currentToken);

		const StaticMeshInstance& currentInstance = *currentToken->mesh;

		const StaticMesh& staticMeshData = *currentInstance.meshData;

		if (!currentInstance.GetBoundingVolume()->IsVisibleFromFrustum(rendererHandle.GetMainCamera().GetFrustumPlanes()) &&
			!currentInstance.GetBoundingVolume()->IsVisibleFromFrustum(shadowBasis.GetFrustumPlanes()))
		{
			continue;
		}

		MathLib::vector4 meshPosition(currentInstance.positionX, currentInstance.positionY, currentInstance.positionZ, 1.0f);

		// We need to transform the light direction and view direction into object space.
		{
			MathLib::matrix4x4 worldSpaceToObjectSpaceNormal;
			MathLib::GenerateInverseTransformMatrix(worldSpaceToObjectSpaceNormal, 
													meshPosition, 
													currentInstance.orientation, 
													currentInstance.scale);
			MathLib::matrix4x4_transpose(worldSpaceToObjectSpaceNormal);

			MathLib::vector4 lightDirectionObjectSpace;
			MathLib::matrix4x4_vectorMul(worldSpaceToObjectSpaceNormal, shadowBasis.GetLightDirection(), lightDirectionObjectSpace);
			shader->SetUniform3f("uLightDirection", lightDirectionObjectSpace.extractX(),
													lightDirectionObjectSpace.extractY(), 
													lightDirectionObjectSpace.extractZ());
		}

		// Set up transform matrix.
		{
			MathLib::matrix4x4 transform;
			MathLib::GenerateTransformMatrix(transform, 
											 meshPosition, 
											 currentInstance.orientation, 
											 currentInstance.scale);

			MathLib::matrix4x4 parentTransformConcat;
			MathLib::matrix4x4_mul(currentInstance.parentTransform, transform, parentTransformConcat);

			MathLib::matrix4x4 globalTransform;
			MathLib::matrix4x4_mul(shadowBasis.GetWorldViewProjection(), parentTransformConcat, globalTransform);
			globalTransform.setOpenGLMatrix(matrixArray);
			shader->SetUniformMatrix4fv("uTransform", static_cast<GLfloat*>(matrixArray), false);
		}

		// Find the correct lod level to use.
		GLuint dataVBOHandle = staticMeshData.dataVBOHandle;
		GLuint indexVBOHandle = staticMeshData.indexVBOHandle;

		int vertexArraySizeBytes = staticMeshData.vertexArraySizeBytes;
		int texCoordArraySizeBytes = staticMeshData.texCoordArraySizeBytes;
		int numIndices = staticMeshData.numIndices;

		MathLib::vector4 worldSpacePosition(currentInstance.positionX, currentInstance.positionY, currentInstance.positionZ, 1.0f);

		MathLib::matrix4x4_vectorMul(currentInstance.parentTransform, worldSpacePosition, worldSpacePosition);
		
		float distanceToMesh = MathLib::vector4_distance(rendererHandle.GetMainCamera().GetPosition(), worldSpacePosition);
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

		glEnableVertexAttribArray(attributeNormalIndex);
		glVertexAttribPointer(attributeNormalIndex, 3, GL_FLOAT, false, 0, reinterpret_cast<GLvoid*>(vertexArraySizeBytes + texCoordArraySizeBytes));

		glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, 0);

		rendererHandle.numTrianglesDrawnLastFrame += numIndices / 3;
	}
}

void SubRendererStaticMeshShadow::RestoreContextState()
{
	glEnable(GL_DEPTH_TEST);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	ShaderProgram::DeactivateShaders();

	glDisableClientState(GL_VERTEX_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glCullFace(GL_BACK);
}

}

#pragma warning(pop)