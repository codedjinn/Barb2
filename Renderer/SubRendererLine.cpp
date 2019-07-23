#include "SubRendererLine.h"
#include "Renderer.h"
#include "ShaderManager.h"
#include "RenderTokenLine.h"
#include <cassert>
#include <cstdlib>

namespace RendererLib
{

SubRendererLine::SubRendererLine(Renderer& renderer, const std::vector<RenderToken*>& set, bool depthTest) :
	SubRenderer(renderer), 
	renderableSet(set)
{
	performDepthTest = depthTest;
}

void SubRendererLine::SetContextState()
{
	glDisable(GL_TEXTURE_2D);
	if (!performDepthTest)
	{
		glDisable(GL_DEPTH_TEST);
	}

	ShaderManager& shaderManager = ShaderManager::GetInstance();
	lineShader = shaderManager.GetShader(ShaderManager::SHADER_SINGLE_COLOR);
	assert(NULL != lineShader);
	lineShader->Use();

	// Set object to world space matrix
	MathLib::matrix4x4 transform;
	transform.loadIdentity();
	MathLib::matrix4x4_copy(rendererHandle.matrices[MATRIX_OBJECT_WORLD_INDEX], transform);

	rendererHandle.UpdateModelViewProjectionMatrix();
	rendererHandle.matrices[MATRIX_MVP_INDEX].setOpenGLMatrix(matrixArray);
	lineShader->SetUniformMatrix4fv("transform", static_cast<GLfloat*>(matrixArray), false);
}

void SubRendererLine::Render()
{
	for (int i = 0; i < renderableSet.size(); i++)
	{
		RenderTokenLine* currentToken = dynamic_cast<RenderTokenLine*>(renderableSet[i]);
		assert(NULL != currentToken);

		glLineWidth(currentToken->lineWidth);

		lineShader->SetUniform3f("color",
								 currentToken->color.extractX(),
								 currentToken->color.extractY(),
								 currentToken->color.extractZ());

		glBegin(GL_LINES);
			glVertex3f(currentToken->p0.extractX(),
					   currentToken->p0.extractY(),
					   currentToken->p0.extractZ());

			glVertex3f(currentToken->p1.extractX(),
					   currentToken->p1.extractY(),
					   currentToken->p1.extractZ());
		glEnd();
	}
}

void SubRendererLine::RestoreContextState()
{
	glLineWidth(1.0f);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_DEPTH_TEST);
}

}
