#include "SubRendererGizmo.h"
#include "Renderer.h"
#include "ShaderManager.h"
#include "RenderTokenGizmo.h"
#include <cassert>
#include <cstdlib>

namespace RendererLib
{

SubRendererGizmo::SubRendererGizmo(Renderer& renderer, const std::vector<RenderToken*>& set) : 
	SubRenderer(renderer), 
	renderableSet(set)
{
}

void SubRendererGizmo::SetContextState()
{
	glCullFace(GL_BACK);
	glDisable(GL_DEPTH_TEST);

	// Initialize shader.
	ShaderManager& shaderManager = ShaderManager::GetInstance();

	gizmoShader = shaderManager.GetShader(ShaderManager::SHADER_SINGLE_COLOR);
	assert(NULL != gizmoShader);
	gizmoShader->Use();

	glEnableClientState(GL_VERTEX_ARRAY);
}

void SubRendererGizmo::Render()
{
	for (int i = 0; i < renderableSet.size(); i++)
	{
		RenderTokenGizmo* currentToken = dynamic_cast<RenderTokenGizmo*>(renderableSet[i]);
		assert(NULL != currentToken);

		GizmoMesh* gizmoMesh = NULL;
		if (GizmoMesh::GIZMO_TYPE_CUBE == currentToken->gizmoType)
		{
			gizmoMesh = rendererHandle.gizmoMeshCube;
		}
		assert(NULL != gizmoMesh);

		gizmoShader->SetUniform3f("color", currentToken->color.extractX(), currentToken->color.extractY(), currentToken->color.extractZ());

		// Copy object space to world space matrix into the appropriate index.
		MathLib::matrix4x4 modelTransform;
		MathLib::GenerateTransformMatrix(modelTransform,
										 currentToken->position,
										 currentToken->orientation,
										 currentToken->scale);

		MathLib::matrix4x4_copy(rendererHandle.matrices[MATRIX_OBJECT_WORLD_INDEX], modelTransform);
		rendererHandle.UpdateModelViewProjectionMatrix();

		rendererHandle.matrices[MATRIX_MVP_INDEX].setOpenGLMatrix(matrixArray);
		gizmoShader->SetUniformMatrix4fv("transform", static_cast<GLfloat*>(matrixArray), false);
		
		GLuint vboHandle = gizmoMesh->GetVBOHandle();
		GLuint indicesHandle = gizmoMesh->GetIndicesHandle();

		glBindBuffer(GL_ARRAY_BUFFER, vboHandle);
		glVertexPointer(3, GL_FLOAT, 0, 0);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indicesHandle);
		unsigned int numIndices = gizmoMesh->GetNumIndices();
		glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, 0);
	}
}

void SubRendererGizmo::RestoreContextState()
{
	ShaderProgram::DeactivateShaders();

	glDisableClientState(GL_VERTEX_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}


}