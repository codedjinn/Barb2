#ifndef RENDERER_H_INCLUDED
#define RENDERER_H_INCLUDED

#include "DebugPrimitive.h"
#include <MathLib.h>
#include <Camera.h>
#include "RendererLibCommon.h"
#include "RenderableSet.h"
#include "GBuffer.h"
#include "GizmoMesh.h"
#include "GizmoMeshCube.h"

namespace RendererLib
{

/**
* The Renderer contains an array of matrices that enables the transformation from object
* space to NDC space. These indices are just simply conventions that allow consistent usage of matrices.
*/
#define RENDERER_MATRIX_ARRAY_SIZE		10
#define MATRIX_MVP_INDEX				0
#define MATRIX_PROJECTION_INDEX			1
#define MATRIX_WORLD_VIEW_INDEX			2
#define MATRIX_OBJECT_WORLD_INDEX		3
#define MATRIX_MODEL_VIEW_INDEX			4

class StaticMeshInstance;
class SubRendererStaticMesh;

enum RendererOutputMode
{
	RENDERER_OUTPUT_STANDARD			= 0,
	RENDERER_OUTPUT_GBUFFER_ALBEDO		= 1,
	RENDERER_OUTPUT_GBUFFER_NORMAL		= 2,
	RENDERER_OUTPUT_GBUFFER_DEPTH		= 3
};

class Renderer
{
    public:

        ~Renderer();

        MathLib::matrix4x4 matrices[RENDERER_MATRIX_ARRAY_SIZE];

        GBuffer gBuffer;

		void Initialize();

		void Resize(uint32_t width, uint32_t height);

		void Draw();

		RENDERER_INLINE unsigned int GetViewportWidth() const { return viewportWidth; }
		RENDERER_INLINE unsigned int GetViewportHeight() const { return viewportHeight; }

		RENDERER_INLINE CameraLib::Camera& GetMainCamera()
		{
			return (mainCamera);
		}

        void GenRayFromScreenCoord(CameraLib::Camera& camera, MathLib::ray* ray, int32_t screen_x, int32_t screen_y, uint32_t screenWidth, uint32_t screenHeight);

        RENDERER_INLINE void SetBoundingVolumesVisible(bool visible)
        {
            renderBoundingVolumes   = visible;
        }

        RENDERER_INLINE GLuint GetDebugSphereVBOHandle()
        {
        	return debugSphereVBO;
        }

        RENDERER_INLINE unsigned int GetDebugSphereNumVertices()
        {
			return debugSphereNumVertices;
        }

        RENDERER_INLINE GLuint GetDebugCubeVBOHandle()
        {
			return debugCubeVBO;
        }

        RENDERER_INLINE unsigned int GetDebugCubeNumVertices()
        {
        	return debugCubeNumVertices;
        }

        RENDERER_INLINE void SetRenderWireframe(bool value)
        {
            renderWireframe = value;
        }

        RENDERER_INLINE bool GetRenderWireframe()
        {
			return renderWireframe;
        }

        RENDERER_INLINE void SetFXAAEnabled(bool value)
        {
			FXAAEnabled = value;
        }

        RENDERER_INLINE bool GetFXAAEnabled()
        {
        	return FXAAEnabled;
        }

        void SetUpCamera(CameraLib::Camera& camera);

        RENDERER_INLINE void CopyRenderableSet(const RenderableSet& source)
        {
			renderableSet.CopyFrom(source);
        }

		RENDERER_INLINE unsigned int GetNumTrianglesDrawnLastFrame() const 
		{
			return numTrianglesDrawnLastFrame;
		}

        void SetRendererOutputMode(RendererOutputMode mode);
        RendererOutputMode GetRendererOutputMode() const;

        void TransferGBufferDepth() const;
		/// Transfers the depth buffer values of the GBuffer to the standard depth buffer.

        void UpdateModelViewProjectionMatrix();
		/// Takes the object-space to world-space matrix, the world-space to view-space matrix
		/// and the projection matrix and concatenates them all into the final model-view-projection
		/// matrix and stores that matrix in the slot identified by MATRIX_MVP_INDEX.
		/// Additionally, the model-view matrix is stored in the slot identified by MATRIX_MODEL_VIEW_INDEX.

        static Renderer& GetInstance();

	protected:

		unsigned int viewportWidth;
		unsigned int viewportHeight;

		CameraLib::Camera mainCamera;

		RenderableSet renderableSet;

        bool renderBoundingVolumes;
        bool renderWireframe;
        bool FXAAEnabled;

        GLuint debugSphereVBO;
        unsigned int debugSphereNumVertices;

        GLuint debugCubeVBO;
        unsigned int debugCubeNumVertices;

		GizmoMeshCube* gizmoMeshCube;

        GLuint fullscreenQuadVBO;

        static Renderer* renderer;

        RendererOutputMode outputMode;

		unsigned int numTrianglesDrawnLastFrame;

        Renderer();

        void GenerateRendererVBOs();

		void RenderShadowMaps();
		void RenderShadowCascade(unsigned int cascade);
		void RenderStaticMeshes();
		void RenderBasicShapes();
		void RenderLights();
		void RenderBoundingVolumes();
		void RenderDebugPrimitives();
		void RenderTerrains();
		void RenderGizmos();
		void RenderSky();
		void RenderHighlightStaticMeshes();

		void GenerateFogBuffer();
		void MergeFogBuffer();

		void RenderFullscreenQuad(GLuint textureID, bool gammaCorrect = false, bool useFXAA = false);
		void OutputLinearDepthPerspective(GLuint textureID, float near, float far);
		void OutputLinearDepthOrthographic(GLuint textureID);

		friend class SubRendererStaticMesh;
		friend class SubRendererStaticMeshShadow;
		friend class SubRendererBoundingVolume;
		friend class SubRendererDirectionalLight;
		friend class SubRendererGizmo;
};

}

#endif // RENDERER_H_INCLUDED
