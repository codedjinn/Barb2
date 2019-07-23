#include "SubRendererTerrainCDLODNode.h"
#include "ShaderManager.h"
#include "Texture.h"
#include "RenderTokenTerrainCDLODNode.h"
#include <Terrain.h>
#include <iostream>
#include <cassert>
#include <cstdlib>

#pragma warning(push)
#pragma warning(disable: 4312)
#pragma warning(disable: 4311)
#pragma warning(disable: 4302)

namespace RendererLib
{

SubRendererTerrainCDLODNode::SubRendererTerrainCDLODNode(Renderer& renderer, const std::vector<RenderToken*>& set) :
	SubRenderer(renderer),
	renderableSet(set)
{
}

void SubRendererTerrainCDLODNode::SetContextState()
{
	glCullFace(GL_BACK);

	glEnable(GL_DEPTH_TEST);

	// Initialize shader.
	ShaderManager& shaderManager = ShaderManager::GetInstance();
	shader = shaderManager.GetShader(ShaderManager::SHADER_TERRAIN_CDLOD);
	assert(NULL != shader);
	shader->Use();

	GLuint programHandle = shader->GetProgramHandle();
	attributeTexCoord0Index = glGetAttribLocation(programHandle, "aTexCoord0");

	shader->SetUniform1f("terrainLandscapeSize", static_cast<float>(TERRAIN_LANDSCAPE_SIZE));

	if (rendererHandle.GetRenderWireframe())
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	else
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	// Enable texture units.
	glActiveTexture(GL_TEXTURE0); glEnable(GL_TEXTURE_2D);
	glActiveTexture(GL_TEXTURE1); glEnable(GL_TEXTURE_2D);
	glActiveTexture(GL_TEXTURE2); glEnable(GL_TEXTURE_2D);
	glActiveTexture(GL_TEXTURE3); glEnable(GL_TEXTURE_2D);
	glActiveTexture(GL_TEXTURE4); glEnable(GL_TEXTURE_2D);
	glActiveTexture(GL_TEXTURE5); glEnable(GL_TEXTURE_2D);
	glActiveTexture(GL_TEXTURE6); glEnable(GL_TEXTURE_2D);
	glActiveTexture(GL_TEXTURE7); glEnable(GL_TEXTURE_2D);
	glActiveTexture(GL_TEXTURE8); glEnable(GL_TEXTURE_2D);
	glActiveTexture(GL_TEXTURE9); glEnable(GL_TEXTURE_2D);
	glActiveTexture(GL_TEXTURE10); glEnable(GL_TEXTURE_2D);
	glActiveTexture(GL_TEXTURE11); glEnable(GL_TEXTURE_2D);
	glActiveTexture(GL_TEXTURE12); glEnable(GL_TEXTURE_2D);
	glActiveTexture(GL_TEXTURE13); glEnable(GL_TEXTURE_2D);
	glActiveTexture(GL_TEXTURE14); glEnable(GL_TEXTURE_2D);
	glActiveTexture(GL_TEXTURE15); glEnable(GL_TEXTURE_2D);
	glActiveTexture(GL_TEXTURE16); glEnable(GL_TEXTURE_2D);
	glActiveTexture(GL_TEXTURE17); glEnable(GL_TEXTURE_2D);
	glActiveTexture(GL_TEXTURE18); glEnable(GL_TEXTURE_2D);
}

void SubRendererTerrainCDLODNode::Render()
{
	TextureManager& textureManager = TextureManager::GetInstance();

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
	/*	if (!boundingVolume.IsVisibleFromFrustum(rendererHandle.GetMainCamera().GetFrustumPlanes()))
		{
			continue;
		}
*/
		// Set texture units.
		shader->SetUniform1i("textureUnitHeightmap", 0);
		shader->SetUniform1i("textureUnitSplat0", 1);
		shader->SetUniform1i("textureUnitSplat1", 2);
		shader->SetUniform1i("textureUnitLayer0Diffuse", 3);
		shader->SetUniform1i("textureUnitLayer0Normal", 4);
		shader->SetUniform1i("textureUnitLayer1Diffuse", 5);
		shader->SetUniform1i("textureUnitLayer1Normal", 6);
		shader->SetUniform1i("textureUnitLayer2Diffuse", 7);
		shader->SetUniform1i("textureUnitLayer2Normal", 8);
		shader->SetUniform1i("textureUnitLayer3Diffuse", 9);
		shader->SetUniform1i("textureUnitLayer3Normal", 10);
		shader->SetUniform1i("textureUnitLayer4Diffuse", 11);
		shader->SetUniform1i("textureUnitLayer4Normal", 12);
		shader->SetUniform1i("textureUnitLayer5Diffuse", 13);
		shader->SetUniform1i("textureUnitLayer5Normal", 14);
		shader->SetUniform1i("textureUnitLayer6Diffuse", 15);
		shader->SetUniform1i("textureUnitLayer6Normal", 16);
		shader->SetUniform1i("textureUnitLayer7Diffuse", 17);
		shader->SetUniform1i("textureUnitLayer7Normal", 18);

		glActiveTexture(GL_TEXTURE0); glBindTexture(GL_TEXTURE_2D, currentToken->terrain.GetHeightmapTextureHandle());
		glActiveTexture(GL_TEXTURE1); glBindTexture(GL_TEXTURE_2D, currentToken->terrain.GetSplatTexture0Handle());
		glActiveTexture(GL_TEXTURE2); glBindTexture(GL_TEXTURE_2D, currentToken->terrain.GetSplatTexture1Handle());
		glActiveTexture(GL_TEXTURE3); glBindTexture(GL_TEXTURE_2D, textureManager.GetTexture(currentToken->terrain.GetTextureLayer0Diffuse()));
		glActiveTexture(GL_TEXTURE4); glBindTexture(GL_TEXTURE_2D, textureManager.GetTexture(currentToken->terrain.GetTextureLayer0Normal()));
		glActiveTexture(GL_TEXTURE5); glBindTexture(GL_TEXTURE_2D, textureManager.GetTexture(currentToken->terrain.GetTextureLayer1Diffuse()));
		glActiveTexture(GL_TEXTURE6); glBindTexture(GL_TEXTURE_2D, textureManager.GetTexture(currentToken->terrain.GetTextureLayer1Normal()));
		glActiveTexture(GL_TEXTURE7); glBindTexture(GL_TEXTURE_2D, textureManager.GetTexture(currentToken->terrain.GetTextureLayer2Diffuse()));
		glActiveTexture(GL_TEXTURE8); glBindTexture(GL_TEXTURE_2D, textureManager.GetTexture(currentToken->terrain.GetTextureLayer2Normal()));
		glActiveTexture(GL_TEXTURE9); glBindTexture(GL_TEXTURE_2D, textureManager.GetTexture(currentToken->terrain.GetTextureLayer3Diffuse()));
		glActiveTexture(GL_TEXTURE10); glBindTexture(GL_TEXTURE_2D, textureManager.GetTexture(currentToken->terrain.GetTextureLayer3Normal()));
		glActiveTexture(GL_TEXTURE11); glBindTexture(GL_TEXTURE_2D, textureManager.GetTexture(currentToken->terrain.GetTextureLayer4Diffuse()));
		glActiveTexture(GL_TEXTURE12); glBindTexture(GL_TEXTURE_2D, textureManager.GetTexture(currentToken->terrain.GetTextureLayer4Normal()));
		glActiveTexture(GL_TEXTURE13); glBindTexture(GL_TEXTURE_2D, textureManager.GetTexture(currentToken->terrain.GetTextureLayer5Diffuse()));
		glActiveTexture(GL_TEXTURE14); glBindTexture(GL_TEXTURE_2D, textureManager.GetTexture(currentToken->terrain.GetTextureLayer5Normal()));
		glActiveTexture(GL_TEXTURE15); glBindTexture(GL_TEXTURE_2D, textureManager.GetTexture(currentToken->terrain.GetTextureLayer6Diffuse()));
		glActiveTexture(GL_TEXTURE16); glBindTexture(GL_TEXTURE_2D, textureManager.GetTexture(currentToken->terrain.GetTextureLayer6Normal()));
		glActiveTexture(GL_TEXTURE17); glBindTexture(GL_TEXTURE_2D, textureManager.GetTexture(currentToken->terrain.GetTextureLayer7Diffuse()));
		glActiveTexture(GL_TEXTURE18); glBindTexture(GL_TEXTURE_2D, textureManager.GetTexture(currentToken->terrain.GetTextureLayer7Normal()));

		// Set terrain repeat factor.
		shader->SetUniform1f("terrainTextureRepeatFactor", currentToken->terrain.GetTerrainTextureFactor());

		// Set patch relative offset.
		// TODO: This needs to be adjusted for terrain landscape position.
		shader->SetUniform2f("relativePatchOffset", currentToken->position.extractX(), currentToken->position.extractZ());

		shader->SetUniform1f("terrainPatchScale", static_cast<GLfloat>(currentToken->scale));

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

        float camX = rendererHandle.GetMainCamera().GetPosition().extractX();
        float camZ = rendererHandle.GetMainCamera().GetPosition().extractZ();
		// Set camera position constant so we can compute vertex distance.
		shader->SetUniform2f("cameraPosition",
							 rendererHandle.GetMainCamera().GetPosition().extractX(),
							 rendererHandle.GetMainCamera().GetPosition().extractZ());

		// Set patch world space position.
		shader->SetUniform2f("patchPosition",
							 currentToken->position.extractX(),
							 currentToken->position.extractZ());

		// Establish transforms.
		{
			rendererHandle.matrices[MATRIX_OBJECT_WORLD_INDEX].loadIdentity();
			rendererHandle.UpdateModelViewProjectionMatrix();
			rendererHandle.matrices[MATRIX_MVP_INDEX].setOpenGLMatrix(matrixArray);
			shader->SetUniformMatrix4fv("transform", static_cast<GLfloat*>(matrixArray), false);
			
			MathLib::matrix4x4 inverseMatrix;
			MathLib::matrix4x4_inverse(rendererHandle.matrices[MATRIX_MODEL_VIEW_INDEX], inverseMatrix);
			MathLib::matrix4x4_transpose(inverseMatrix);
			inverseMatrix.setOpenGLMatrix(matrixArray);
			shader->SetUniformMatrix4fv("normalTransform", static_cast<GLfloat*>(matrixArray), false);
		}

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

		glDrawElements(GL_TRIANGLES, RendererTerrainCDLOD::GetNodeIndexVBONumIndcies(), GL_UNSIGNED_INT, 0);
	}
}

void SubRendererTerrainCDLODNode::RestoreContextState()
{
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableVertexAttribArray(attributeTexCoord0Index);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	// Disable active multi texture units (except 0).
	glActiveTexture(GL_TEXTURE1); glDisable(GL_TEXTURE_2D);
	glActiveTexture(GL_TEXTURE2); glDisable(GL_TEXTURE_2D);
	glActiveTexture(GL_TEXTURE3); glDisable(GL_TEXTURE_2D);
	glActiveTexture(GL_TEXTURE4); glDisable(GL_TEXTURE_2D);
	glActiveTexture(GL_TEXTURE5); glDisable(GL_TEXTURE_2D);
	glActiveTexture(GL_TEXTURE6); glDisable(GL_TEXTURE_2D);
	glActiveTexture(GL_TEXTURE7); glDisable(GL_TEXTURE_2D);
	glActiveTexture(GL_TEXTURE8); glDisable(GL_TEXTURE_2D);
	glActiveTexture(GL_TEXTURE9); glDisable(GL_TEXTURE_2D);
	glActiveTexture(GL_TEXTURE10); glDisable(GL_TEXTURE_2D);
	glActiveTexture(GL_TEXTURE11); glDisable(GL_TEXTURE_2D);
	glActiveTexture(GL_TEXTURE12); glDisable(GL_TEXTURE_2D);
	glActiveTexture(GL_TEXTURE13); glDisable(GL_TEXTURE_2D);
	glActiveTexture(GL_TEXTURE14); glDisable(GL_TEXTURE_2D);
	glActiveTexture(GL_TEXTURE15); glDisable(GL_TEXTURE_2D);
	glActiveTexture(GL_TEXTURE16); glDisable(GL_TEXTURE_2D);
	glActiveTexture(GL_TEXTURE17); glDisable(GL_TEXTURE_2D);
	glActiveTexture(GL_TEXTURE18); glDisable(GL_TEXTURE_2D);
	glActiveTexture(GL_TEXTURE0);

	ShaderProgram::DeactivateShaders();
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

}

#pragma warning(pop)
