#include "SubRendererBoundingVolume.h"
#include "ShaderManager.h"
#include "StaticMesh.h"
#include "RenderTokenStaticMesh.h"
#include <cassert>

using namespace RendererLib;

SubRendererBoundingVolume::SubRendererBoundingVolume(Renderer& renderer, const std::vector<RenderToken*>& set) :
	SubRenderer(renderer),
	renderableSet(set)
{
}

void SubRendererBoundingVolume::SetContextState()
{
	glEnable(GL_DEPTH_TEST);

	// Initialize shader
	ShaderManager& shaderManager = ShaderManager::GetInstance();

	shader = shaderManager.GetShader(ShaderManager::SHADER_SINGLE_COLOR);
	assert(NULL != shader);

	shader->Use();
	shader->SetUniform3f("color", 0.0f, 1.0f, 0.0f);
}

void SubRendererBoundingVolume::Render()
{
	for (int i = 0; i < renderableSet.size(); i++)
	{
		RenderTokenStaticMesh* currentToken = reinterpret_cast<RenderTokenStaticMesh*>(renderableSet[i]);
		assert(NULL != currentToken);

		const StaticMeshInstance& currentInstance = *currentToken->mesh;

		const StaticMesh& staticMeshData = *currentInstance.meshData;

		const BoundingVolumes::BoundingVolume* currentBoundingVolume = currentInstance.GetBoundingVolume();

		// Copy object space to world space matrix into the appropriate index]

		rendererHandle.matrices[MATRIX_OBJECT_WORLD_INDEX].loadIdentity();
		//MathLib::matrix4x4_copy(rendererHandle.matrices[MATRIX_OBJECT_W0RLD_INDEX], currentInstance.GetTransform());
		rendererHandle.UpdateModelViewProjectionMatrix();

		rendererHandle.matrices[MATRIX_MVP_INDEX].setOpenGLMatrix(matrixArray);
		shader->SetUniformMatrix4fv("transform", static_cast<GLfloat*>(matrixArray), false);

		if (BoundingVolumes::BOUNDING_VOLUME_SPHERE == currentBoundingVolume->GetBoundingVolumeType())
		{
			BoundingVolumes::BoundingSphere* sphere = (BoundingVolumes::BoundingSphere*)currentBoundingVolume;

			const MathLib::vector4& sphere_position = sphere->GetPosition();
			const float sphere_radius = sphere->GetRadius();

			glBegin(GL_LINE_LOOP);
			for (float angle = 0.0f; angle < 360.0f; angle += 10.0f)
			{
				float x = cosf(MATHLIB_DEG_TO_RAD(angle));
				float y = sinf(MATHLIB_DEG_TO_RAD(angle));

				glVertex3f(x * sphere_radius + sphere_position.extractX(),
						   y * sphere_radius + sphere_position.extractY(),
						   sphere_position.extractZ());
			}
			glEnd();

			glBegin(GL_LINE_LOOP);
			for (float angle = 0.0f; angle < 360.0f; angle += 10.0f)
			{
				float z = cosf(MATHLIB_DEG_TO_RAD(angle));
				float y = sinf(MATHLIB_DEG_TO_RAD(angle));

				glVertex3f(sphere_position.extractX(),
						   y * sphere_radius + sphere_position.extractY(),
						   z * sphere_radius + sphere_position.extractZ());
			}
			glEnd();

			glBegin(GL_LINE_LOOP);
			for (float angle = 0.0f; angle < 360.0f; angle += 10.0f)
			{
				float x = cosf(MATHLIB_DEG_TO_RAD(angle));
				float z = sinf(MATHLIB_DEG_TO_RAD(angle));

				glVertex3f(x * sphere_radius + sphere_position.extractX(),
						   sphere_position.extractY(),
						   z * sphere_radius + sphere_position.extractZ());
			}
			glEnd();
		}

		if (BoundingVolumes::BOUNDING_VOLUME_OOBB == currentBoundingVolume -> GetBoundingVolumeType())
		{
			BoundingVolumes::BoundingOOBB* oobb = (BoundingVolumes::BoundingOOBB*)currentBoundingVolume;

			const MathLib::vector4& FTL = oobb -> getWorldSpace_FTL();
			const MathLib::vector4& FTR = oobb -> getWorldSpace_FTR();
			const MathLib::vector4& FBR = oobb -> getWorldSpace_FBR();
			const MathLib::vector4& FBL = oobb -> getWorldSpace_FBL();

			const MathLib::vector4& BTL = oobb -> getWorldSpace_BTL();
			const MathLib::vector4& BTR = oobb -> getWorldSpace_BTR();
			const MathLib::vector4& BBR = oobb -> getWorldSpace_BBR();
			const MathLib::vector4& BBL = oobb -> getWorldSpace_BBL();

			glBegin(GL_LINE_LOOP);
				glVertex3f(FTL.extractX(), FTL.extractY(), FTL.extractZ());
				glVertex3f(FTR.extractX(), FTR.extractY(), FTR.extractZ());
				glVertex3f(FBR.extractX(), FBR.extractY(), FBR.extractZ());
				glVertex3f(FBL.extractX(), FBL.extractY(), FBL.extractZ());
			glEnd();

			glBegin(GL_LINE_LOOP);
				glVertex3f(BTL.extractX(), BTL.extractY(), BTL.extractZ());
				glVertex3f(BTR.extractX(), BTR.extractY(), BTR.extractZ());
				glVertex3f(BBR.extractX(), BBR.extractY(), BBR.extractZ());
				glVertex3f(BBL.extractX(), BBL.extractY(), BBL.extractZ());
			glEnd();

			glBegin(GL_LINES);
				glVertex3f(FTL.extractX(), FTL.extractY(), FTL.extractZ());
				glVertex3f(BTL.extractX(), BTL.extractY(), BTL.extractZ());

				glVertex3f(FTR.extractX(), FTR.extractY(), FTR.extractZ());
				glVertex3f(BTR.extractX(), BTR.extractY(), BTR.extractZ());

				glVertex3f(FBR.extractX(), FBR.extractY(), FBR.extractZ());
				glVertex3f(BBR.extractX(), BBR.extractY(), BBR.extractZ());

				glVertex3f(FBL.extractX(), FBL.extractY(), FBL.extractZ());
				glVertex3f(BBL.extractX(), BBL.extractY(), BBL.extractZ());
			glEnd();
		}
	}
}

void SubRendererBoundingVolume::RestoreContextState()
{
	ShaderProgram::DeactivateShaders();
}
