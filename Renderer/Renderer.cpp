#include <iostream>
#include <cassert>
#include <BoundingVolumes.h>
#include <stdlib.h>

#include "Renderer.h"
#include "RendererUtils.h"
#include "RenderToken.h"
#include "RenderTokenDebugPrimitive.h"
#include "RenderTokenStaticMesh.h"
#include "RenderTokenTerrain.h"
#include "RenderTokenTerrainCDLODNode.h"
#include "RenderTokenSky.h"
#include "RenderTokenHighlightStaticMesh.h"
#include "StaticMesh.h"
#include "Texture.h"
#include "ShaderManager.h"
#include "SubRendererStaticMesh.h"
#include "SubRendererBasicShape.h"
#include "SubRendererBoundingVolume.h"
#include "SubRendererDirectionalLight.h"
#include "SubRendererTerrainNaive.h"
#include "SubRendererTerrainCDLODNode.h"
#include "SubRendererGizmo.h"
#include "SubRendererSky.h"
#include "SubRendererHighlightStaticMesh.h"
#include "LightManager.h"
#include "Terrain.h"
#include "DirectionalLight.h"
#include "DirectionalLightShadowBasis.h"
#include "SubRendererBasicShapeShadow.h"
#include "SubRendererStaticMeshShadow.h"
#include "SubRendererTerrainCDLODNodeShadow.h"

namespace RendererLib
{
Renderer* Renderer::renderer = NULL;
}

using namespace RendererLib;

Renderer::~Renderer()
{

}

Renderer::Renderer()
{   
	Initialize();
}

void Renderer::Initialize()
{
    glClearColor(1.0f, 0.0f, 0.0f, 0.0f);
	glClearDepth(1.0f);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

    glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CW);

    mainCamera.SetPosition(0.0f, 1.0f, 0.0f);
    mainCamera.SetCameraYFov(70.0f);               // Humans typically have a field of view around 100 degrees, factor in screen distance.
    mainCamera.SetNearClipPlaneDistance(0.1f);
    mainCamera.SetFarClipPlaneDistance(1000.0f);

    renderBoundingVolumes = false;
    renderWireframe = false;
    FXAAEnabled = true;

    outputMode = RENDERER_OUTPUT_STANDARD;

    GenerateRendererVBOs();

	gizmoMeshCube = new GizmoMeshCube();
}

void Renderer::GenerateRendererVBOs()
{
	unsigned int offset	= 0;

	// Generate debug sphere
	const unsigned int numLinesPerAxis		= (360 / 10);
	const unsigned int numVertsPerAxis		= numLinesPerAxis * 2;
	const unsigned int numAxesPerSphere		= 3;
	const unsigned int numFloatsPerPoint	= 3;
	const unsigned int sphereArraySize		= numVertsPerAxis * numAxesPerSphere * numFloatsPerPoint;

	debugSphereNumVertices	= numVertsPerAxis * numAxesPerSphere;

	float sphereArray[sphereArraySize];

	// x axis
	for (float angle = 0.0f; angle < 360.0f; angle += 10.0f)
	{
		float angleRadians		= MATHLIB_DEG_TO_RAD(angle);
		sphereArray[offset++]	= cosf(angleRadians);
		sphereArray[offset++]	= 0.0f;
		sphereArray[offset++]	= sinf(angleRadians);

		angleRadians += MATHLIB_DEG_TO_RAD(10.0f);
		sphereArray[offset++]	= cosf(angleRadians);
		sphereArray[offset++]	= 0.0f;
		sphereArray[offset++]	= sinf(angleRadians);
	}

	// y axis
	for (float angle = 0.0f; angle < 360.0f; angle += 10.0f)
	{
		float angleRadians		= MATHLIB_DEG_TO_RAD(angle);
		sphereArray[offset++]	= cosf(angleRadians);
		sphereArray[offset++]	= sinf(angleRadians);
		sphereArray[offset++]	= 0.0f;

		angleRadians += MATHLIB_DEG_TO_RAD(10.0f);
		sphereArray[offset++]	= cosf(angleRadians);
		sphereArray[offset++]	= sinf(angleRadians);
		sphereArray[offset++]	= 0.0f;
	}

	// z axis
	for (float angle = 0.0f; angle < 360.0f; angle += 10.0f)
	{
		float angleRadians		= MATHLIB_DEG_TO_RAD(angle);
		sphereArray[offset++]	= 0.0f;
		sphereArray[offset++]	= sinf(angleRadians);
		sphereArray[offset++]	= cosf(angleRadians);

		angleRadians += MATHLIB_DEG_TO_RAD(10.0f);
		sphereArray[offset++]	= 0.0f;
		sphereArray[offset++]	= sinf(angleRadians);
		sphereArray[offset++]	= cosf(angleRadians);
	}

	glGenBuffers(1, &debugSphereVBO);
	glBindBuffer(GL_ARRAY_BUFFER, debugSphereVBO);

	// Create the data store
	glBufferData(GL_ARRAY_BUFFER,
				 sphereArraySize * sizeof(float),
				 sphereArray,
				 GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Generate debug cube
	const unsigned int cubeNumLines		= 12;
	const unsigned int cubeNumVertices	= 24;
	const unsigned int cubeArraySize	= cubeNumVertices * numFloatsPerPoint;

	debugCubeNumVertices	= cubeNumVertices;

	float cubeArray[cubeArraySize];

	offset	= 0;

	// Front face
	cubeArray[offset++]	= -1.0f;
	cubeArray[offset++]	= 1.0f;
	cubeArray[offset++]	= 1.0f;

	cubeArray[offset++]	= 1.0f;
	cubeArray[offset++]	= 1.0f;
	cubeArray[offset++]	= 1.0f;

	cubeArray[offset++]	= 1.0f;
	cubeArray[offset++]	= 1.0f;
	cubeArray[offset++]	= 1.0f;

	cubeArray[offset++]	= 1.0f;
	cubeArray[offset++]	= -1.0f;
	cubeArray[offset++]	= 1.0f;

	cubeArray[offset++]	= 1.0f;
	cubeArray[offset++]	= -1.0f;
	cubeArray[offset++]	= 1.0f;

	cubeArray[offset++]	= -1.0f;
	cubeArray[offset++]	= -1.0f;
	cubeArray[offset++]	= 1.0f;

	cubeArray[offset++]	= -1.0f;
	cubeArray[offset++]	= -1.0f;
	cubeArray[offset++]	= 1.0f;

	cubeArray[offset++]	= -1.0f;
	cubeArray[offset++]	= 1.0f;
	cubeArray[offset++]	= 1.0f;

	// Back face
	cubeArray[offset++]	= -1.0f;
	cubeArray[offset++]	= 1.0f;
	cubeArray[offset++]	= -1.0f;

	cubeArray[offset++]	= 1.0f;
	cubeArray[offset++]	= 1.0f;
	cubeArray[offset++]	= -1.0f;

	cubeArray[offset++]	= 1.0f;
	cubeArray[offset++]	= 1.0f;
	cubeArray[offset++]	= -1.0f;

	cubeArray[offset++]	= 1.0f;
	cubeArray[offset++]	= -1.0f;
	cubeArray[offset++]	= -1.0f;

	cubeArray[offset++]	= 1.0f;
	cubeArray[offset++]	= -1.0f;
	cubeArray[offset++]	= -1.0f;

	cubeArray[offset++]	= -1.0f;
	cubeArray[offset++]	= -1.0f;
	cubeArray[offset++]	= -1.0f;

	cubeArray[offset++]	= -1.0f;
	cubeArray[offset++]	= -1.0f;
	cubeArray[offset++]	= -1.0f;

	cubeArray[offset++]	= -1.0f;
	cubeArray[offset++]	= 1.0f;
	cubeArray[offset++]	= -1.0f;

	// Connecting lines
	cubeArray[offset++]	= -1.0f;
	cubeArray[offset++]	= 1.0f;
	cubeArray[offset++]	= 1.0f;

	cubeArray[offset++]	= -1.0f;
	cubeArray[offset++]	= 1.0f;
	cubeArray[offset++]	= -1.0f;

	cubeArray[offset++]	= 1.0f;
	cubeArray[offset++]	= 1.0f;
	cubeArray[offset++]	= 1.0f;

	cubeArray[offset++]	= 1.0f;
	cubeArray[offset++]	= 1.0f;
	cubeArray[offset++]	= -1.0f;

	cubeArray[offset++]	= 1.0f;
	cubeArray[offset++]	= -1.0f;
	cubeArray[offset++]	= 1.0f;

	cubeArray[offset++]	= 1.0f;
	cubeArray[offset++]	= -1.0f;
	cubeArray[offset++]	= -1.0f;

	cubeArray[offset++]	= -1.0f;
	cubeArray[offset++]	= -1.0f;
	cubeArray[offset++]	= 1.0f;

	cubeArray[offset++]	= -1.0f;
	cubeArray[offset++]	= -1.0f;
	cubeArray[offset++]	= -1.0f;

	glGenBuffers(1, &debugCubeVBO);
	glBindBuffer(GL_ARRAY_BUFFER, debugCubeVBO);

	// Create the data store
	glBufferData(GL_ARRAY_BUFFER,
				 cubeArraySize * sizeof(float),
				 cubeArray,
				 GL_STREAM_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	float fullscreenQuadPositionArray[] = {-1.0f, -1.0f,
										   -1.0f, 1.0f,
										   1.0f, 1.0f,
										   1.0f, -1.0f};

	float fullscreenQuadTextureCoordinateArray[] = {0.0f, 0.0f,
													0.0f, 1.0f,
													1.0f, 1.0f,
													1.0f, 0.0f};

	glGenBuffers(1, &fullscreenQuadVBO);
	glBindBuffer(GL_ARRAY_BUFFER, fullscreenQuadVBO);

	// Create the data store
	const int fullscreenQuadBufferSize = 8 * sizeof(float);
	const int fullscreenQuadBufferViewVectorsSize = 12 * sizeof(float);
	glBufferData(GL_ARRAY_BUFFER, (fullscreenQuadBufferSize * 2) + fullscreenQuadBufferViewVectorsSize, NULL, GL_DYNAMIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, fullscreenQuadBufferSize, fullscreenQuadPositionArray);
	glBufferSubData(GL_ARRAY_BUFFER, fullscreenQuadBufferSize, fullscreenQuadBufferSize, fullscreenQuadTextureCoordinateArray);
	
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Renderer::Resize(uint32_t width, uint32_t height)
{
	viewportWidth = width;
	viewportHeight = height;

	glViewport(0, 0, width, height);

	glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    float matrixArray[16];
    mainCamera.GetPerspectiveProjectionMatrix(width, height).setOpenGLMatrix(matrixArray);
    glLoadMatrixf(matrixArray);

	// Store the projection matrix in the appropriate index
    MathLib::matrix4x4_copy(matrices[MATRIX_PROJECTION_INDEX], mainCamera.GetPerspectiveProjectionMatrix(width, height));

    gBuffer.Resize(width, height);
}

void Renderer::GenRayFromScreenCoord(CameraLib::Camera& camera, MathLib::ray* ray, int32_t screen_x, int32_t screen_y, uint32_t screenWidth, uint32_t screenHeight)
{
    float aspectRatio = (float)screenWidth / (float)screenHeight;

    float top = camera.GetNearClipPlaneDistance() * tan(MATHLIB_DEG_TO_RAD(camera.GetYFov() * 0.5f));
    float right = top * aspectRatio;

    // First project the point onto the near clip plane (in camera space of course)
    MathLib::matrix4x4 screenCoordToNearPlaneB
    (
        right, 0.0f, 0.0f, 0.0f,
        0.0f, top, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    );

    MathLib::matrix4x4 screenCoordToNearPlaneA
    (
        2.0f / (screenWidth - 1.0f), 0.0f, 0.0f, -1.0f,
        0.0f, -2.0f  / (screenHeight - 1.0f), 0.0f, + 1.0f,
        0.0f, 0.0f, 0.0f, -camera.GetNearClipPlaneDistance(),
        0.0f, 0.0f, 0.0f, 1.0f
    );

    MathLib::matrix4x4 transform;

    MathLib::matrix4x4_mul(screenCoordToNearPlaneB, screenCoordToNearPlaneA, transform);

    MathLib::vector4 screenCoord((float)screen_x, (float)screen_y, 0.0f, 1.0f);
    MathLib::vector4 cameraSpaceDirection;

    MathLib::matrix4x4_vectorMul(transform, screenCoord, cameraSpaceDirection);
    MathLib::vector4_setToVector(cameraSpaceDirection);

    // Now we need to transform the direction into world space
    MathLib::vector4& camera_xAxis = camera.GetXAxis();
    MathLib::vector4& camera_yAxis = camera.GetYAxis();
    MathLib::vector4& camera_zAxis = camera.GetZAxis();

    MathLib::matrix4x4 rotationMatrix
    (
        camera_xAxis.extractX(), camera_yAxis.extractX(), camera_zAxis.extractX(), 0.0f,
        camera_xAxis.extractY(), camera_yAxis.extractY(), camera_zAxis.extractY(), 0.0f,
        camera_xAxis.extractZ(), camera_yAxis.extractZ(), camera_zAxis.extractZ(), 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    );

    MathLib::matrix4x4_vectorMul(rotationMatrix, cameraSpaceDirection, cameraSpaceDirection);
    MathLib::vector4_normalize(cameraSpaceDirection);

    // Transfer position and direction across to ray
    ray->setPosition(camera.GetPosition());
    ray->setDirection(cameraSpaceDirection);
}

void Renderer::Draw()
{
	// Clear OpenGL matrices (we don't use them).
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	mainCamera.Update();
	SetUpCamera(mainCamera);
	
	numTrianglesDrawnLastFrame = 0;

	// Render shadows.
	RenderShadowMaps();
	
	// Bind frame buffer.
	gBuffer.Bind(GBUFFER_MATERIAL_PASS);

	glClear(GL_DEPTH_BUFFER_BIT);

	RenderTerrains();
	RenderStaticMeshes();
	RenderBasicShapes();

	// Lighting stage.
	RenderLights();
	
	gBuffer.Bind(GBUFFER_FOG_PASS);
	GenerateFogBuffer();
	
	gBuffer.Bind(GBUFFER_MERGE_FOG_PASS);
	MergeFogBuffer();

	gBuffer.Bind(GBUFFER_LIGHTING_PASS);
	RenderFullscreenQuad(gBuffer.GetGeneralBuffer0Handle());

	switch (outputMode)
	{
        case RENDERER_OUTPUT_GBUFFER_ALBEDO:
            RenderFullscreenQuad(gBuffer.GetAlbedoBufferHandle());
            break;

		case RENDERER_OUTPUT_GBUFFER_NORMAL:
			RenderFullscreenQuad(gBuffer.GetNormalBufferHandle());
			break;

		case RENDERER_OUTPUT_GBUFFER_DEPTH:
			OutputLinearDepthPerspective(gBuffer.GetDepthBufferHandle(), mainCamera.GetNearClipPlaneDistance(), mainCamera.GetFarClipPlaneDistance());
			break;

		default:
			//OutputLinearDepthPerspective(gBuffer.GetDepthBufferHandle(), mainCamera.GetNearClipPlaneDistance(), mainCamera.GetFarClipPlaneDistance());
			break;
	}
			
    if (renderBoundingVolumes)
    {
        RenderBoundingVolumes();
    }

	RenderSky();
	RenderGizmos();
    RenderDebugPrimitives();
	RenderHighlightStaticMeshes();
	
	gBuffer.Unbind();
	
    RenderFullscreenQuad(gBuffer.GetLightingBufferHandle(), true, FXAAEnabled);

    //OutputLinearDepthOrthographic(LightManager::GetInstance().GetShadowMap0Handle());
}

void Renderer::RenderShadowMaps()
{
	using namespace CameraLib;

	LightManager& lightManager = LightManager::GetInstance();

	// Set depth buffer to 1.0f.
	glEnable(GL_DEPTH_TEST);
	glDepthMask(true);

	glClearDepth(1.0f);

	// Get the directional light.
	if (lightManager.GetDirectionalLightList().size() > 0)
	{
		for (unsigned int i = 0; i < 4; i++)
		{
			RenderShadowCascade(i);
		}
	}
	
	lightManager.Unbind();
}

void Renderer::RenderShadowCascade(unsigned int cascade)
{
	LightManager& lightManager = LightManager::GetInstance();
	DirectionalLight& light = *lightManager.GetDirectionalLightList()[0];
    DirectionalLightShadowBasis* shadowBasis = nullptr;

	mainCamera.GetYFov();

	const float slices[4] = {25.0f, 75.0f, 150.0f, 300.0f};

	MathLib::vector4 boundingSpherePosition;
	float boundingSphereRadius;

	switch (cascade)
	{
		case 0:
			lightManager.BindForCascadeMap0();
			shadowBasis = &lightManager.GetDirectionalLightShadowBasisCascade0();
			mainCamera.GetFrustumSliceBoundingSphere(mainCamera.GetNearClipPlaneDistance(), slices[0], boundingSphereRadius, boundingSpherePosition);
			shadowBasis->GenerateWvpMatrix(light.GetDirection(), boundingSphereRadius, boundingSpherePosition);
			break;

		case 1:
			lightManager.BindForCascadeMap1();
			shadowBasis = &lightManager.GetDirectionalLightShadowBasisCascade1();
			mainCamera.GetFrustumSliceBoundingSphere(slices[0], slices[1], boundingSphereRadius, boundingSpherePosition);
			shadowBasis->GenerateWvpMatrix(light.GetDirection(), boundingSphereRadius, boundingSpherePosition);
			break;

		case 2:
			lightManager.BindForCascadeMap2();
			shadowBasis = &lightManager.GetDirectionalLightShadowBasisCascade2();
			mainCamera.GetFrustumSliceBoundingSphere(slices[1], slices[2], boundingSphereRadius, boundingSpherePosition);
			shadowBasis->GenerateWvpMatrix(light.GetDirection(), boundingSphereRadius, boundingSpherePosition);
			break;

		case 3:
			lightManager.BindForCascadeMap3();
			shadowBasis = &lightManager.GetDirectionalLightShadowBasisCascade3();
			mainCamera.GetFrustumSliceBoundingSphere(slices[2], slices[3], boundingSphereRadius, boundingSpherePosition);
			shadowBasis->GenerateWvpMatrix(light.GetDirection(), boundingSphereRadius, boundingSpherePosition);
			break;
	}
	
	glClear(GL_DEPTH_BUFFER_BIT);

	{
		SubRendererTerrainCDLODNodeShadow subRenderer(*this, renderableSet.GetTerrainTokens(), *shadowBasis);
		subRenderer.SetContextState();
		subRenderer.Render();
		subRenderer.RestoreContextState();
	}

	{
		SubRendererBasicShapeShadow subRenderer(*this, renderableSet.GetBasicShapeTokens(), shadowBasis->GetWorldViewProjection());
		subRenderer.SetContextState();
		subRenderer.Render();
		subRenderer.RestoreContextState();
	}

	{
		SubRendererStaticMeshShadow subRenderer(*this, renderableSet.GetStaticMeshTokens(), *shadowBasis);
		subRenderer.SetContextState();
		subRenderer.Render();
		subRenderer.RestoreContextState();
	}
}

void Renderer::RenderStaticMeshes()
{
	SubRendererStaticMesh subRenderer(*this, renderableSet.GetStaticMeshTokens());
	subRenderer.SetContextState();
	subRenderer.Render();
	subRenderer.RestoreContextState();
}

void Renderer::RenderBasicShapes()
{
	SubRendererBasicShape subRenderer(*this, renderableSet.GetBasicShapeTokens());
	subRenderer.SetContextState();
	subRenderer.Render();
	subRenderer.RestoreContextState();
}

void Renderer::RenderLights()
{
	SubRendererDirectionalLight subRenderer(*this, LightManager::GetInstance().GetDirectionalLightList());
	subRenderer.SetContextState();
	subRenderer.Render();
	subRenderer.RestoreContextState();
}

void Renderer::RenderBoundingVolumes()
{
	SubRendererBoundingVolume subRenderer(*this, renderableSet.GetStaticMeshTokens());
	subRenderer.SetContextState();
	subRenderer.Render();
	subRenderer.RestoreContextState();
}

void Renderer::RenderTerrains()
{
	SubRendererTerrainCDLODNode subRenderer(*this, renderableSet.GetTerrainTokens());
	subRenderer.SetContextState();
	subRenderer.Render();
	subRenderer.RestoreContextState();
}

void Renderer::RenderGizmos()
{
	SubRendererGizmo subRenderer(*this, renderableSet.GetGizmoTokens());
	subRenderer.SetContextState();
	subRenderer.Render();
	subRenderer.RestoreContextState();
}

void Renderer::RenderSky()
{
	SubRendererSky subRenderer(*this, renderableSet.GetSkyTokens());
	subRenderer.SetContextState();
	subRenderer.Render();
	subRenderer.RestoreContextState();
}

void Renderer::RenderHighlightStaticMeshes()
{
	SubRendererHighlightStaticMesh subRenderer(*this, renderableSet.GetHighlightStaticMeshTokens());
	subRenderer.SetContextState();
	subRenderer.Render();
	subRenderer.RestoreContextState();
}

void Renderer::RenderDebugPrimitives()
{
	using namespace std;

	float matrixArray[16];

	ShaderProgram* shader = ShaderManager::GetInstance().GetShader(ShaderManager::SHADER_SINGLE_COLOR);
	shader->Use();

	const vector<RenderToken*>& tokens = renderableSet.GetDebugPrimitiveTokens();
	for (int i = 0; i < tokens.size(); i++)
	{
		RenderTokenDebugPrimitive const * const currentToken = reinterpret_cast<RenderTokenDebugPrimitive*>(tokens[i]);

		const DebugPrimitive* currentPrimitive = currentToken->primitive;

		shader->SetUniform3f("color", currentPrimitive->color.extractX(), currentPrimitive->color.extractY(), currentPrimitive->color.extractZ());

		MathLib::matrix4x4_copy(matrices[MATRIX_OBJECT_WORLD_INDEX], currentPrimitive->transform);
		UpdateModelViewProjectionMatrix();

		matrices[MATRIX_MVP_INDEX].setOpenGLMatrix(matrixArray);
		shader->SetUniformMatrix4fv("transform", static_cast<GLfloat*>(matrixArray), false);

		glBindBuffer(GL_ARRAY_BUFFER, currentPrimitive->GetVBOHandle());

		glEnableClientState(GL_VERTEX_ARRAY);

		glVertexPointer(3, GL_FLOAT, 0, 0);

		glDrawArrays(GL_LINES, 0, currentPrimitive->GetNumVertices());
	}

	glDisableClientState(GL_VERTEX_ARRAY);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	ShaderProgram::DeactivateShaders();
}

void Renderer::SetUpCamera(CameraLib::Camera& camera)
{
	// Store the projection matrix in the appropriate index.
    MathLib::matrix4x4_copy(matrices[MATRIX_PROJECTION_INDEX], mainCamera.GetPerspectiveProjectionMatrix(viewportWidth, viewportHeight));
	
    // Copy the world to view matrix into its appropriate index.
	MathLib::matrix4x4_copy(matrices[MATRIX_WORLD_VIEW_INDEX], mainCamera.GetWorldSpaceToCameraSpaceTransform());

	// Generate the world-space frustum planes.
	mainCamera.CalculateFrustumPlanes();
}

void Renderer::UpdateModelViewProjectionMatrix()
{
	MathLib::matrix4x4_mul(matrices[MATRIX_WORLD_VIEW_INDEX], matrices[MATRIX_OBJECT_WORLD_INDEX], matrices[MATRIX_MODEL_VIEW_INDEX]);
	MathLib::matrix4x4_mul(matrices[MATRIX_PROJECTION_INDEX], matrices[MATRIX_MODEL_VIEW_INDEX], matrices[MATRIX_MVP_INDEX]);
}

void Renderer::RenderFullscreenQuad(GLuint textureID, bool gammaCorrect, bool useFXAA)
{
	// Set state.
	{
		glDisable(GL_DEPTH_TEST);
		glDepthMask(GL_FALSE);

		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	}

	// Choose appropriate shader program.
	ShaderManager& shaderManager = ShaderManager::GetInstance();
	ShaderProgram* shader = NULL;

	float gammaFactor = 1.0f;
	if (gammaCorrect)
		gammaFactor = 1.0f / 2.2f;

	if (useFXAA)
	{
		shader = shaderManager.GetShader(ShaderManager::SHADER_FXAA);
		assert(NULL != shader);
		shader->Use();

		shader->SetUniform2f("frameBufferDimensions",
							static_cast<float>(gBuffer.GetWidth()),
							static_cast<float>(gBuffer.GetHeight()));
	}
	else
	{
		shader = shaderManager.GetShader(ShaderManager::SHADER_FULLSCREEN_QUAD);
		assert(NULL != shader);
		shader->Use();
	}

	shader->SetUniform1f("gammaFactor", gammaFactor);

	GLuint programHandle = shader->GetProgramHandle();
	GLuint attributeTexCoord0Index = glGetAttribLocation(programHandle, "aTexCoord0");

	float matrixArray[16];
	MathLib::matrix4x4 transform;
	transform.loadIdentity();
	transform.setOpenGLMatrix(matrixArray);

	shader->SetUniformMatrix4fv("transform", matrixArray, false);

	glBindBuffer(GL_ARRAY_BUFFER, fullscreenQuadVBO);

	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(2, GL_FLOAT, 0, 0);

	glEnableVertexAttribArray(attributeTexCoord0Index);
	glVertexAttribPointer(attributeTexCoord0Index, 2, GL_FLOAT, false, 0, reinterpret_cast<GLvoid*>(8 * sizeof(float)));

	glDrawArrays(GL_QUADS, 0, 4);

	// Restore state.
	{
		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableVertexAttribArray(attributeTexCoord0Index);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		ShaderProgram::DeactivateShaders();

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glBindTexture(GL_TEXTURE_2D, 0);
		glEnable(GL_DEPTH_TEST);
		glDepthMask(GL_TRUE);
	}
}

void Renderer::OutputLinearDepthPerspective(GLuint textureID, float near, float far)
{
	// Set state.
	glDisable(GL_DEPTH_TEST);
	glDepthMask(GL_FALSE);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	ShaderManager& manager = ShaderManager::GetInstance();
	ShaderProgram* shader = manager.GetShader(ShaderManager::SHADER_OUTPUT_LINEAR_DEPTH);
	assert(NULL != shader);
	shader->Use();

	// Calcuate inputs.
	float n = near;
	float f = far;
	float A = -(f + n) / (f - n);
	float B = (-2.0f * f * n) / (f - n);

	shader->SetUniform4f("ABnf", A, B, n, f);

	// Render.
	GLuint programHandle = shader->GetProgramHandle();
	GLuint attributeTexCoord0Index = glGetAttribLocation(programHandle, "aTexCoord0");

	float matrixArray[16];
	MathLib::matrix4x4 transform;
	transform.loadIdentity();
	transform.setOpenGLMatrix(matrixArray);

	shader->SetUniformMatrix4fv("transform", matrixArray, false);
	
	glBindBuffer(GL_ARRAY_BUFFER, fullscreenQuadVBO);

	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(2, GL_FLOAT, 0, 0);

	glEnableVertexAttribArray(attributeTexCoord0Index);
	glVertexAttribPointer(attributeTexCoord0Index, 2, GL_FLOAT, false, 0, reinterpret_cast<GLvoid*>(8 * sizeof(float)));

	glDrawArrays(GL_QUADS, 0, 4);

	// Restore state
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableVertexAttribArray(attributeTexCoord0Index);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	ShaderProgram::DeactivateShaders();

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glBindTexture(GL_TEXTURE_2D, 0);
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
}

void Renderer::OutputLinearDepthOrthographic(GLuint textureID)
{
	// Set state.
	glDisable(GL_DEPTH_TEST);
	glDepthMask(GL_FALSE);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE_ARB, GL_NONE);

	ShaderManager& manager = ShaderManager::GetInstance();
	ShaderProgram* shader = manager.GetShader(ShaderManager::SHADER_FULLSCREEN_QUAD);
	assert(NULL != shader);
	shader->Use();

	// Render.
	GLuint programHandle = shader->GetProgramHandle();
	GLuint attributeTexCoord0Index = glGetAttribLocation(programHandle, "aTexCoord0");

	float matrixArray[16];
	MathLib::matrix4x4 transform;
	transform.loadIdentity();

	transform._00 *= 0.5f;
	transform._11 *= 0.5f;
	transform._22 *= 0.5f;
	transform.setOpenGLMatrix(matrixArray);

	shader->SetUniformMatrix4fv("transform", matrixArray, false);
	
	glBindBuffer(GL_ARRAY_BUFFER, fullscreenQuadVBO);

	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(2, GL_FLOAT, 0, 0);

	glEnableVertexAttribArray(attributeTexCoord0Index);
	glVertexAttribPointer(attributeTexCoord0Index, 2, GL_FLOAT, false, 0, reinterpret_cast<GLvoid*>(8 * sizeof(float)));

	glDrawArrays(GL_QUADS, 0, 4);

	// Restore state
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableVertexAttribArray(attributeTexCoord0Index);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	ShaderProgram::DeactivateShaders();

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glBindTexture(GL_TEXTURE_2D, 0);
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
}

void Renderer::SetRendererOutputMode(RendererOutputMode mode)
{
	outputMode = mode;
}

RendererOutputMode Renderer::GetRendererOutputMode() const
{
	return outputMode;
}

void Renderer::TransferGBufferDepth() const
{
    // Set state
    glEnable(GL_DEPTH_TEST);        // Has to be enabled for some fucking reason.

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, gBuffer.GetDepthBufferHandle());

    glColorMask(false, false, false, false);

    ShaderManager& shaderManager = ShaderManager::GetInstance();
    ShaderProgram* shader = shaderManager.GetShader(ShaderManager::SHADER_DEPTH_TRANSFER);
    assert(NULL != shader);
    shader->Use();

    shader->SetUniform1i("textureUnit0", 0);

    GLuint programHandle = shader->GetProgramHandle();
    GLuint attributeTexCoord0Index = glGetAttribLocation(programHandle, "aTexCoord0");

    float matrixArray[16];
    MathLib::matrix4x4 transform;
    transform.loadIdentity();
    transform.setOpenGLMatrix(matrixArray);

    shader->SetUniformMatrix4fv("transform", matrixArray, false);

    glBindBuffer(GL_ARRAY_BUFFER, fullscreenQuadVBO);

    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(2, GL_FLOAT, 0, 0);

    glEnableVertexAttribArray(attributeTexCoord0Index);
    glVertexAttribPointer(attributeTexCoord0Index, 2, GL_FLOAT, false, 0, reinterpret_cast<GLvoid*>(8 * sizeof(float)));

    glDrawArrays(GL_QUADS, 0, 4);

    // Restore state
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableVertexAttribArray(attributeTexCoord0Index);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    ShaderProgram::DeactivateShaders();

    glBindTexture(GL_TEXTURE_2D, 0);
    glEnable(GL_DEPTH_TEST);

    glColorMask(true, true, true, true);
}

void Renderer::GenerateFogBuffer()
{
	// Set state.
	glDisable(GL_DEPTH_TEST);
	glDepthMask(GL_FALSE);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, gBuffer.GetDepthBufferHandle());
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	ShaderManager& manager = ShaderManager::GetInstance();
	ShaderProgram* shader = manager.GetShader(ShaderManager::SHADER_GENERATE_FOG_BUFFER);
	assert(NULL != shader);
	shader->Use();

	// Calcuate inputs.
	float n = mainCamera.GetNearClipPlaneDistance();
	float f = mainCamera.GetFarClipPlaneDistance();
	float A = -(f + n) / (f - n);
	float B = (-2.0f * f * n) / (f - n);
	shader->SetUniform4f("uABnf", A, B, n, f);

	shader->SetUniform1f("uStartDistance", 200.0f);
	//shader->SetUniform3f("uFogColor", 0.2f, 0.15f, 0.2f);
	shader->SetUniform3f("uFogColor", 0.7f, 0.7f, 0.7f);

	// Render.
	GLuint programHandle = shader->GetProgramHandle();
	GLuint attributeTexCoord0Index = glGetAttribLocation(programHandle, "aTexCoord0");

	float matrixArray[16];
	MathLib::matrix4x4 transform;
	transform.loadIdentity();
	transform.setOpenGLMatrix(matrixArray);

	shader->SetUniformMatrix4fv("transform", matrixArray, false);
	
	glBindBuffer(GL_ARRAY_BUFFER, fullscreenQuadVBO);

	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(2, GL_FLOAT, 0, 0);

	glEnableVertexAttribArray(attributeTexCoord0Index);
	glVertexAttribPointer(attributeTexCoord0Index, 2, GL_FLOAT, false, 0, reinterpret_cast<GLvoid*>(8 * sizeof(float)));

	glDrawArrays(GL_QUADS, 0, 4);

	// Restore state
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableVertexAttribArray(attributeTexCoord0Index);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	ShaderProgram::DeactivateShaders();

	glBindTexture(GL_TEXTURE_2D, 0);
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
}

void Renderer::MergeFogBuffer()
{
	ShaderManager& manager = ShaderManager::GetInstance();
	ShaderProgram* shader = manager.GetShader(ShaderManager::SHADER_MERGE_FOG_BUFFER);
	assert(NULL != shader);
	shader->Use();

	// Set state.
	glDisable(GL_DEPTH_TEST);
	glDepthMask(GL_FALSE);

	glActiveTexture(GL_TEXTURE0); glEnable(GL_TEXTURE_2D);
	shader->SetUniform1i("textureUnit0", 0);
	glBindTexture(GL_TEXTURE_2D, gBuffer.GetLightingBufferHandle());
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glActiveTexture(GL_TEXTURE1); glEnable(GL_TEXTURE_2D);
	shader->SetUniform1i("textureUnit1", 1);
	glBindTexture(GL_TEXTURE_2D, gBuffer.GetFogBufferHandle());
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	// Submit geometry for rendering.
	GLuint programHandle = shader->GetProgramHandle();
	GLuint attributeTexCoord0Index = glGetAttribLocation(programHandle, "aTexCoord0");

	float matrixArray[16];
	MathLib::matrix4x4 transform;
	transform.loadIdentity();
	transform.setOpenGLMatrix(matrixArray);

	shader->SetUniformMatrix4fv("transform", matrixArray, false);
	
	glBindBuffer(GL_ARRAY_BUFFER, fullscreenQuadVBO);

	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(2, GL_FLOAT, 0, 0);

	glEnableVertexAttribArray(attributeTexCoord0Index);
	glVertexAttribPointer(attributeTexCoord0Index, 2, GL_FLOAT, false, 0, reinterpret_cast<GLvoid*>(8 * sizeof(float)));

	glDrawArrays(GL_QUADS, 0, 4);

	// Restore state
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableVertexAttribArray(attributeTexCoord0Index);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	ShaderProgram::DeactivateShaders();

	glActiveTexture(GL_TEXTURE1); 
	glBindTexture(GL_TEXTURE_2D, 0);
	
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, 0);

	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
}

Renderer& Renderer::GetInstance()
{
    if (NULL == renderer)
    {
        renderer = new Renderer();

        assert(NULL != renderer);
    }

    return *renderer;
}
