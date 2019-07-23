#include "SubRendererDirectionalLight.h"
#include "LightManager.h"
#include "DirectionalLight.h"
#include "Renderer.h"
#include "ShaderManager.h"
#include <Camera.h>
#include <MathLib.h>
#include <iostream>
#include <cassert>

#pragma warning(push)
#pragma warning(disable:4312)


using namespace RendererLib;

SubRendererDirectionalLight::SubRendererDirectionalLight(Renderer& renderer, std::vector<DirectionalLight*>& set) :
	SubRenderer(renderer),
	lightSet(set)
{
}

void SubRendererDirectionalLight::SetContextState()
{
	LightManager& lightManager = LightManager::GetInstance();

	// Lighting buffer for result.
	rendererHandle.gBuffer.Bind(GBUFFER_LIGHTING_PASS);

	// Disable depth test and write.
	glDisable(GL_DEPTH_TEST);
	glDepthMask(GL_FALSE);

	// Activate additive blend mode.
	glBlendFunc(GL_ONE, GL_ONE);
	glEnable(GL_BLEND);

	// Cear lighting buffer.
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	glClear(GL_COLOR_BUFFER_BIT);

	glActiveTexture(GL_TEXTURE0);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, rendererHandle.gBuffer.GetAlbedoBufferHandle());
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glActiveTexture(GL_TEXTURE1);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, rendererHandle.gBuffer.GetNormalBufferHandle());
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glActiveTexture(GL_TEXTURE2);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, rendererHandle.gBuffer.GetDepthBufferHandle());
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glActiveTexture(GL_TEXTURE3);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, lightManager.GetShadowMap0Handle());
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE_ARB, GL_COMPARE_R_TO_TEXTURE_ARB);

	glActiveTexture(GL_TEXTURE4);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, lightManager.GetShadowMap1Handle());
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE_ARB, GL_COMPARE_R_TO_TEXTURE_ARB);

	glActiveTexture(GL_TEXTURE5);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, lightManager.GetShadowMap2Handle());
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE_ARB, GL_COMPARE_R_TO_TEXTURE_ARB);

	glActiveTexture(GL_TEXTURE6);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, lightManager.GetShadowMap3Handle());
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE_ARB, GL_COMPARE_R_TO_TEXTURE_ARB);

	// Set shader
	ShaderManager& shaderManager = ShaderManager::GetInstance();
	directionalLightShader = shaderManager.GetShader(ShaderManager::SHADER_DIRECTIONAL_LIGHT);
	assert(NULL != directionalLightShader);
	directionalLightShader->Use();
	directionalLightShader->SetUniform1i("albedoBuffer", 0);
	directionalLightShader->SetUniform1i("normalBuffer", 1);
	directionalLightShader->SetUniform1i("depthBuffer", 2);
	directionalLightShader->SetUniform1i("shadowCascade0", 3);
	directionalLightShader->SetUniform1i("shadowCascade1", 4);
	directionalLightShader->SetUniform1i("shadowCascade2", 5);
	directionalLightShader->SetUniform1i("shadowCascade3", 6);

	float n = rendererHandle.GetMainCamera().GetNearClipPlaneDistance();
	float f = rendererHandle.GetMainCamera().GetFarClipPlaneDistance();
	float A = -(f + n) / (f - n);
	float B = (-2.0f * f * n) / (f - n);
	directionalLightShader->SetUniform4f("ABnf", A, B, n, f);

	float aspectRatio = (float)rendererHandle.GetViewportWidth() / (float)rendererHandle.GetViewportHeight();
	float halfYFov = rendererHandle.mainCamera.GetYFov() * 0.5f;
	float nearClipPlaneExtentsX = n * tan(MATHLIB_DEG_TO_RAD(halfYFov)) * aspectRatio;
	float nearClipPlaneExtentsY = n * tan(MATHLIB_DEG_TO_RAD(halfYFov));
	directionalLightShader->SetUniform2f("nearClipPlaneExtents", nearClipPlaneExtentsX, nearClipPlaneExtentsY);

	float matrixArray[16];
	lightManager.GetDirectionalLightShadowBasisCascade0().GetWorldViewProjection().setOpenGLMatrix(matrixArray);
	directionalLightShader->SetUniformMatrix4fv("cascade0WVP", matrixArray, false);
	
	lightManager.GetDirectionalLightShadowBasisCascade1().GetWorldViewProjection().setOpenGLMatrix(matrixArray);
	directionalLightShader->SetUniformMatrix4fv("cascade1WVP", matrixArray, false);

	lightManager.GetDirectionalLightShadowBasisCascade2().GetWorldViewProjection().setOpenGLMatrix(matrixArray);
	directionalLightShader->SetUniformMatrix4fv("cascade2WVP", matrixArray, false);

	lightManager.GetDirectionalLightShadowBasisCascade3().GetWorldViewProjection().setOpenGLMatrix(matrixArray);
	directionalLightShader->SetUniformMatrix4fv("cascade3WVP", matrixArray, false);

	directionalLightShader->SetUniform1f("shadowMapResolution", (float)lightManager.GetShadowMapsResolution());

	MathLib::matrix4x4 transform;
	transform.loadIdentity();
	transform.setOpenGLMatrix(matrixArray);

	directionalLightShader->SetUniformMatrix4fv("transform", matrixArray, false);

	glBindBuffer(GL_ARRAY_BUFFER, rendererHandle.fullscreenQuadVBO);
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(2, GL_FLOAT, 0, 0);
	
	const int fullscreenQuadBufferSize = 8 * sizeof(float);
	const int fullscreenQuadBufferViewVectorsSize = 12 * sizeof(float);
	GLuint attributeTexCoord0Index = glGetAttribLocation(directionalLightShader->GetProgramHandle(), "aTexCoord0");
	glEnableVertexAttribArray(attributeTexCoord0Index);
	glVertexAttribPointer(attributeTexCoord0Index, 2, GL_FLOAT, false, 0, reinterpret_cast<GLvoid*>(fullscreenQuadBufferSize));

	GLuint attributeFrustumVectorIndex = glGetAttribLocation(directionalLightShader->GetProgramHandle(), "aFrustumVector");
	glEnableVertexAttribArray(attributeFrustumVectorIndex);
	glVertexAttribPointer(attributeFrustumVectorIndex, 3, GL_FLOAT, false, 0, reinterpret_cast<GLvoid*>(fullscreenQuadBufferSize * 2));
}

void SubRendererDirectionalLight::Render()
{
	for (int i = 0; i < lightSet.size(); i++)
	{
		CameraLib::Camera& camera = rendererHandle.GetMainCamera();

		MathLib::matrix4x4 inverseMatrix;
		MathLib::matrix4x4_inverse(camera.GetWorldSpaceToCameraSpaceTransform(), inverseMatrix);
		MathLib::matrix4x4_transpose(inverseMatrix);

		// Copy light direction into view space.
		MathLib::vector4 lightDirectionViewSpace;
		MathLib::matrix4x4_vectorMul(inverseMatrix, lightSet[i]->GetDirection(), lightDirectionViewSpace);

		// Need to upload the opposite direction of the directional light as the viewspace direction for the dot product.
		directionalLightShader->SetUniform3f("viewspaceDirection", -lightDirectionViewSpace.extractX(), -lightDirectionViewSpace.extractY(), -lightDirectionViewSpace.extractZ());
		
		// Set the frustum vectors.
		const int fullscreenQuadBufferSize = 8 * sizeof(float);
		const int fullscreenQuadBufferViewVectorsSize = 12 * sizeof(float);
		{
			float width = (float)rendererHandle.GetViewportWidth();
			float height = (float)rendererHandle.GetViewportHeight();

			float aspectRatio = width / height;
			float farDistance = rendererHandle.mainCamera.GetFarClipPlaneDistance();
			float xfov = rendererHandle.mainCamera.GetYFov() * 0.5f;
			float yfov = rendererHandle.mainCamera.GetYFov() * 0.5f;

			float yOppositeFar = farDistance * tanf(MATHLIB_DEG_TO_RAD(yfov));
			float xOppositeFar = farDistance * tanf(MATHLIB_DEG_TO_RAD(xfov)) * aspectRatio;
	
			MathLib::vector4 bottomLeft(-xOppositeFar, -yOppositeFar, -farDistance, 0.0f);
			MathLib::vector4 topLeft(-xOppositeFar, yOppositeFar, -farDistance, 0.0f);
			MathLib::vector4 topRight(xOppositeFar, yOppositeFar, -farDistance, 0.0f);
			MathLib::vector4 bottomRight(xOppositeFar, -yOppositeFar, -farDistance, 0.0f);

			// Transform into world space.
			MathLib::matrix4x4 cameraWorldNormal;
			MathLib::matrix4x4_copy(cameraWorldNormal, rendererHandle.mainCamera.GetCameraToWorldSpaceTransform());
			//MathLib::matrix4x4_transpose(cameraWorldNormal);

			MathLib::matrix4x4_vectorMul(cameraWorldNormal, bottomLeft, bottomLeft);
			MathLib::matrix4x4_vectorMul(cameraWorldNormal, topLeft, topLeft);
			MathLib::matrix4x4_vectorMul(cameraWorldNormal, topRight, topRight);
			MathLib::matrix4x4_vectorMul(cameraWorldNormal, bottomRight, bottomRight);

			float frustumVectors[12] = 
			{
				bottomLeft.extractX(), bottomLeft.extractY(), bottomLeft.extractZ(), 
				topLeft.extractX(), topLeft.extractY(), topLeft.extractZ(), 
				topRight.extractX(), topRight.extractY(), topRight.extractZ(), 
				bottomRight.extractX(), bottomRight.extractY(), bottomRight.extractZ()
			};

			glBufferSubData(GL_ARRAY_BUFFER, fullscreenQuadBufferSize * 2, fullscreenQuadBufferViewVectorsSize, frustumVectors);
		}

		const MathLib::vector4& cameraPosition = rendererHandle.mainCamera.GetPosition();
		directionalLightShader->SetUniform3f("cameraPosition", cameraPosition.extractX(), cameraPosition.extractY(), cameraPosition.extractZ());

		// Set light color
		MathLib::vector4 lightColor = lightSet[i]->GetColor();
		directionalLightShader->SetUniform3f("lightColor", lightColor.extractX(), lightColor.extractY(), lightColor.extractZ());

		glDrawArrays(GL_QUADS, 0, 4);
	}
}

void SubRendererDirectionalLight::RestoreContextState()
{
	// Reset gbuffer binding.
	rendererHandle.gBuffer.Unbind();

	// Enable depth test and write.
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);

	// Disable blending.
	glDisable(GL_BLEND);

	glActiveTexture(GL_TEXTURE1);
	glDisable(GL_TEXTURE_2D);

	glActiveTexture(GL_TEXTURE2);
	glDisable(GL_TEXTURE_2D);

	glActiveTexture(GL_TEXTURE3);
	glDisable(GL_TEXTURE_2D);
	
	glActiveTexture(GL_TEXTURE4);
	glDisable(GL_TEXTURE_2D);
	
	glActiveTexture(GL_TEXTURE5);
	glDisable(GL_TEXTURE_2D);

	glActiveTexture(GL_TEXTURE6);
	glDisable(GL_TEXTURE_2D);

	glActiveTexture(GL_TEXTURE0);		// !! This needs to always be reset !!

	// Disable shaders.
	ShaderProgram::DeactivateShaders();
}

#pragma warning(pop)