#include "SubRendererTerrainCDLODNodeShadow.h"
#include "ShaderManager.h"
#include "Texture.h"
#include "RenderTokenTerrainCDLODNode.h"
#include "DirectionalLightShadowBasis.h"
#include <Terrain.h>
#include <iostream>
#include <cassert>
#include <cstdlib>

#pragma warning(push)
#pragma warning(disable:4311)
#pragma warning(disable:4302)

namespace RendererLib
{

SubRendererTerrainCDLODNodeShadow::SubRendererTerrainCDLODNodeShadow(Renderer& renderer, const std::vector<RenderToken*>& set, const DirectionalLightShadowBasis& directionalLightBasis) :
	SubRenderer(renderer),
	renderableSet(set), 
	lightBasis(directionalLightBasis)
{
}

void SubRendererTerrainCDLODNodeShadow::SetContextState()
{
	glCullFace(GL_BACK);
	glEnable(GL_DEPTH_TEST);

	// Initialize shader.
	ShaderManager& shaderManager = ShaderManager::GetInstance();
	shader = shaderManager.GetShader(ShaderManager::SHADER_TERRAIN_CDLOD_DEPTH_ONLY);
	assert(NULL != shader);
	shader->Use();

	GLuint programHandle = shader->GetProgramHandle();
	attributeTexCoord0Index = glGetAttribLocation(programHandle, "aTexCoord0");

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	// Enable texture units.
	glActiveTexture(GL_TEXTURE0);
	glEnable(GL_TEXTURE_2D);

	// Initialize VBOs.
	glBindBuffer(GL_ARRAY_BUFFER, RendererTerrainCDLOD::GetNodeVBOHandle());
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, RendererTerrainCDLOD::GetNodeIndexVBOHandle());

	// Set attribute offsets into VBO.
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, 0);

	const unsigned int vboHandle = RendererTerrainCDLOD::GetNodeVBOHandle();
	const unsigned int indHandle = RendererTerrainCDLOD::GetNodeIndexVBOHandle();
	const unsigned int attribOffset = reinterpret_cast<unsigned int>(RendererTerrainCDLOD::GetNodeVBOTextureAttribOffset());
	const unsigned int numIndices = RendererTerrainCDLOD::GetNodeIndexVBONumIndcies();

	glEnableVertexAttribArray(attributeTexCoord0Index);
	glVertexAttribPointer(attributeTexCoord0Index, 2, GL_FLOAT, false, 0, RendererTerrainCDLOD::GetNodeVBOTextureAttribOffset());
}

void SubRendererTerrainCDLODNodeShadow::Render()
{
	for (int i = 0; i < renderableSet.size(); i++)
	{
		RenderTokenTerrainCDLODNode* currentToken = reinterpret_cast<RenderTokenTerrainCDLODNode*>(renderableSet[i]);
		assert(NULL != currentToken);

		// Cull node if possible.
		float nodeExtent = (float)(currentToken->scale * (TERRAIN_PATCH_SIZE - 1));

		BoundingVolumes::BoundingAABB boundingVolume(currentToken->position.extractX() + nodeExtent,
													 TERRAIN_MAX_HEIGHT,
													 currentToken->position.extractZ() + nodeExtent,
													 currentToken->position.extractX(),
													 0.0f,
													 currentToken->position.extractZ());
		if (!boundingVolume.IsVisibleFromFrustum(rendererHandle.GetMainCamera().GetFrustumPlanes()) && 
			!boundingVolume.IsVisibleFromFrustum(lightBasis.GetFrustumPlanes()))
		{
			continue;
		}

		// Set texture units.
		shader->SetUniform1i("textureUnitHeightmap", 0);

		glActiveTexture(GL_TEXTURE0); glBindTexture(GL_TEXTURE_2D, currentToken->terrain.GetHeightmapTextureHandle());

		shader->SetUniform1f("terrainPatchScale", static_cast<GLfloat>(currentToken->scale));

		// Set patch world space position.
		shader->SetUniform2f("patchPosition",
							 currentToken->position.extractX(),
							 currentToken->position.extractZ());

		// Copy object space to world space matrix into appropriate index.
		lightBasis.GetWorldViewProjection().setOpenGLMatrix(matrixArray);
		shader->SetUniformMatrix4fv("transform", static_cast<GLfloat*>(matrixArray), false);

		const float terrainLandscapeMaxValue = TERRAIN_LANDSCAPE_SIZE - 1;
		const float terrainPatchMaxValue = TERRAIN_PATCH_SIZE - 1;

		const float uStep = 1.0f / terrainLandscapeMaxValue;
		const float vStep = uStep;

		shader->SetUniform2f("uvStep",
							 uStep * currentToken->scale,
							 vStep * currentToken->scale);

		shader->SetUniform2f("uvOffset",
							 currentToken->position.extractX() / terrainLandscapeMaxValue,
							 (terrainLandscapeMaxValue - (currentToken->position.extractZ() + terrainPatchMaxValue * currentToken->scale)) / terrainLandscapeMaxValue);

		shader->SetUniform1f("textureCoordScale", terrainPatchMaxValue * currentToken->scale / terrainLandscapeMaxValue);

		// Need to get the range factors for this lod level.
		{
			float morphStartDistance;
			float morphEndDistance;

			const RendererTerrainCDLODRanges& lodRanges = currentToken->terrain.GetLodRanges();

			const unsigned int maxRange = lodRanges.rangesLength - 1;

			// Get the current range for this node.
			unsigned int lodRange = static_cast<unsigned int>(log2(currentToken->scale));
			if (lodRange >= lodRanges.rangesLength)
				lodRange = maxRange;

            morphStartDistance = lodRanges.ranges[lodRange].morphAreaStart;
            morphEndDistance = lodRanges.ranges[lodRange].distance;

			// Set shader constants.
			shader->SetUniform1f("morphStartDistance", morphStartDistance);
			shader->SetUniform1f("morphEndDistance", morphEndDistance);
		}

		// Set camera position constant so we can compute vertex distance.
		shader->SetUniform2f("cameraPosition",
							 rendererHandle.GetMainCamera().GetPosition().extractX(),
							 rendererHandle.GetMainCamera().GetPosition().extractZ());

		// Set patch world space position.
		shader->SetUniform2f("patchPosition",
							 currentToken->position.extractX(),
							 currentToken->position.extractZ());

		glDrawElements(GL_TRIANGLES, RendererTerrainCDLOD::GetNodeIndexVBONumIndcies(), GL_UNSIGNED_INT, 0);
	}
}

void SubRendererTerrainCDLODNodeShadow::RestoreContextState()
{
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableVertexAttribArray(attributeTexCoord0Index);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glActiveTexture(GL_TEXTURE0);

	ShaderProgram::DeactivateShaders();
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	
	glCullFace(GL_BACK);
}

}

#pragma warning(pop)