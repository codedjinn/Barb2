#include "SubRendererTerrainNaive.h"
#include "Renderer.h"
#include "ShaderManager.h"
#include "Texture.h"
#include "RenderTokenTerrain.h"
#include "RendererTerrainNaive.h"
#include <cassert>
#include <cstdlib>

#pragma warning(push)
#pragma warning(disable:4312)

using namespace RendererLib;

SubRendererTerrainNaive::SubRendererTerrainNaive(Renderer& renderer, const std::vector<RenderToken*>& set) :
	SubRenderer(renderer),
	renderableSet(set)
{
}

void SubRendererTerrainNaive::SetContextState()
{
	glCullFace(GL_BACK);
	glEnable(GL_TEXTURE_2D);

	glEnable(GL_DEPTH_TEST);

	// Initialize shader.
	ShaderManager& shaderManager = ShaderManager::GetInstance();

	terrainShader = shaderManager.GetShader(ShaderManager::SHADER_TERRAIN_NAIVE);
	assert(NULL != terrainShader);
	terrainShader->Use();

	GLuint programHandle = terrainShader->GetProgramHandle();
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
}

void SubRendererTerrainNaive::Render()
{
	TextureManager& textureManager = TextureManager::GetInstance();

	for (int i = 0; i < renderableSet.size(); i++)
	{
		RenderTokenTerrain* currentToken = reinterpret_cast<RenderTokenTerrain*>(renderableSet[i]);
		assert(NULL != currentToken);

		RendererTerrainNaive& currentTerrain = *(reinterpret_cast<RendererTerrainNaive*>(currentToken->terrain));

		glBindTexture(GL_TEXTURE_2D, textureManager.GetTexture(currentTerrain.GetTerrainTexture()));

		// Set the object space to world space matrix for the terrain.
		rendererHandle.matrices[MATRIX_OBJECT_WORLD_INDEX].loadIdentity();
		rendererHandle.UpdateModelViewProjectionMatrix();

		rendererHandle.matrices[MATRIX_MVP_INDEX].setOpenGLMatrix(matrixArray);
		terrainShader->SetUniformMatrix4fv("transform", static_cast<GLfloat*>(matrixArray), false);

		MathLib::matrix4x4 inverseMatrix;
		MathLib::matrix4x4_inverse(rendererHandle.matrices[MATRIX_MODEL_VIEW_INDEX], inverseMatrix);
		MathLib::matrix4x4_transpose(inverseMatrix);
		inverseMatrix.setOpenGLMatrix(matrixArray);
		terrainShader->SetUniformMatrix4fv("modelTransform", static_cast<GLfloat*>(matrixArray), false);

		glBindBuffer(GL_ARRAY_BUFFER, currentTerrain.GetDataVBOHandle());
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, currentTerrain.GetIndexVBOHandle());

		const unsigned int vertexArraySizeBytes = currentTerrain.GetNumVertices() * 3 * sizeof(GLfloat);
		const unsigned int texCoordArraySizeBytes = currentTerrain.GetNumVertices() * 2 * sizeof(GLfloat);

		// Set attribute offsets into VBO.
		glEnableClientState(GL_VERTEX_ARRAY);
		glVertexPointer(3, GL_FLOAT, 0, 0);

		glEnableVertexAttribArray(attributeTexCoord0Index);
		glVertexAttribPointer(attributeTexCoord0Index, 2, GL_FLOAT, false, 0, reinterpret_cast<GLvoid*>(vertexArraySizeBytes));

		glEnableVertexAttribArray(attributeNormalIndex);
		glVertexAttribPointer(attributeNormalIndex, 3, GL_FLOAT, false, 0, reinterpret_cast<GLvoid*>(vertexArraySizeBytes + texCoordArraySizeBytes));

		glDrawElements(GL_TRIANGLES, currentTerrain.GetNumIndices(), GL_UNSIGNED_INT, 0);
	}

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableVertexAttribArray(attributeTexCoord0Index);
	glDisableVertexAttribArray(attributeNormalIndex);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void SubRendererTerrainNaive::RestoreContextState()
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	ShaderProgram::DeactivateShaders();
}

#pragma warning(pop)

