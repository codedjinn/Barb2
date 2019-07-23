#include "SubRendererSky.h"
#include "Renderer.h"
#include "ShaderManager.h"
#include "Texture.h"
#include "RenderTokenSky.h"
#include "BasicShapes.h"
#include <cassert>
#include <cstdlib>
#include <iostream>

#pragma warning(push)
#pragma warning(disable:4312)

namespace RendererLib
{

SubRendererSky::SubRendererSky(Renderer& renderer, const std::vector<RenderToken*>& set) :
	SubRenderer(renderer),
	renderableSet(set)
{
}

void SubRendererSky::SetContextState()
{
	glCullFace(GL_BACK);
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_FALSE);

	glActiveTexture(GL_TEXTURE0);
	glEnable(GL_TEXTURE_2D);

	// Initialize shader.
	ShaderManager& shaderManager = ShaderManager::GetInstance();

	skyShader = shaderManager.GetShader(ShaderManager::SHADER_SKY);
	assert(NULL != skyShader);
	skyShader->Use();

	skyShader->SetUniform1i("textureUnit0", 0);

	GLuint programHandle = skyShader->GetProgramHandle();
	attributeTexCoord0Index = glGetAttribLocation(programHandle, "aTexCoord0");

	if (rendererHandle.GetRenderWireframe())
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINES);
	}
	else
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableVertexAttribArray(attributeTexCoord0Index);
}

void SubRendererSky::Render()
{
	TextureManager& textureManager = TextureManager::GetInstance();

	for (unsigned int i = 0; i < renderableSet.size(); i++)
	{
		RenderTokenSky* currentToken = reinterpret_cast<RenderTokenSky*>(renderableSet[i]);
		assert(NULL != currentToken);

		// Establish transform matrices.
		{
			MathLib::quaternion orientation;
			MathLib::quaternion_setToIdentity(orientation);

			MathLib::matrix4x4 transform;
			MathLib::GenerateTransformMatrix(transform,
											 MathLib::vector4(2048.0f, 0.0f, 2048.0f, 1.0f), 
											 orientation,
											 10000.0f);
			MathLib::matrix4x4_copy(rendererHandle.matrices[MATRIX_OBJECT_WORLD_INDEX], transform);
			rendererHandle.UpdateModelViewProjectionMatrix();

			rendererHandle.matrices[MATRIX_MVP_INDEX].setOpenGLMatrix(matrixArray);
			skyShader->SetUniformMatrix4fv("transform", static_cast<GLfloat*>(matrixArray), false);
		}

		BasicShapes& basicShapes = BasicShapes::GetInstance();

		TextureManager& textureManager = TextureManager::GetInstance();

		// Render front face.
		{
			glBindBuffer(GL_ARRAY_BUFFER, basicShapes.internalSphereFrontFace);

			glBindTexture(GL_TEXTURE_2D, textureManager.GetTexture(currentToken->faceTextures[0]));
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

			// Set attribute offsets into VBO.
			glVertexPointer(3, GL_FLOAT, 0, 0);
			glVertexAttribPointer(attributeTexCoord0Index, 2, GL_FLOAT, false, 0, reinterpret_cast<GLvoid*>(basicShapes.internalSpherePositionArraySizeBytes));
			
			glDrawArrays(GL_TRIANGLES, 0, basicShapes.internalSphereFaceNumTriangles * 3);
		}

		// Render right face.
		{
			glBindBuffer(GL_ARRAY_BUFFER, basicShapes.internalSphereRightFace);

			glBindTexture(GL_TEXTURE_2D, textureManager.GetTexture(currentToken->faceTextures[1]));
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

			// Set attribute offsets into VBO.
			glVertexPointer(3, GL_FLOAT, 0, 0);
			glVertexAttribPointer(attributeTexCoord0Index, 2, GL_FLOAT, false, 0, reinterpret_cast<GLvoid*>(basicShapes.internalSpherePositionArraySizeBytes));
			
			glDrawArrays(GL_TRIANGLES, 0, basicShapes.internalSphereFaceNumTriangles * 3);
		}

		// Render back face.
		{
			glBindBuffer(GL_ARRAY_BUFFER, basicShapes.internalSphereBackFace);

			glBindTexture(GL_TEXTURE_2D, textureManager.GetTexture(currentToken->faceTextures[2]));
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

			// Set attribute offsets into VBO.
			glVertexPointer(3, GL_FLOAT, 0, 0);
			glVertexAttribPointer(attributeTexCoord0Index, 2, GL_FLOAT, false, 0, reinterpret_cast<GLvoid*>(basicShapes.internalSpherePositionArraySizeBytes));

			glDrawArrays(GL_TRIANGLES, 0, basicShapes.internalSphereFaceNumTriangles * 3);
		}

		// 	// Render left face.
		{
			glBindBuffer(GL_ARRAY_BUFFER, basicShapes.internalSphereLeftFace);

			glBindTexture(GL_TEXTURE_2D, textureManager.GetTexture(currentToken->faceTextures[3]));
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

			// Set attribute offsets into VBO.
			glVertexPointer(3, GL_FLOAT, 0, 0);
			glVertexAttribPointer(attributeTexCoord0Index, 2, GL_FLOAT, false, 0, reinterpret_cast<GLvoid*>(basicShapes.internalSpherePositionArraySizeBytes));

			glDrawArrays(GL_TRIANGLES, 0, basicShapes.internalSphereFaceNumTriangles * 3);
		}

		// Render top face.
		{
			glBindBuffer(GL_ARRAY_BUFFER, basicShapes.internalSphereTopFace);

			glBindTexture(GL_TEXTURE_2D, textureManager.GetTexture(currentToken->faceTextures[4]));
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

			// Set attribute offsets into VBO.
			glVertexPointer(3, GL_FLOAT, 0, 0);
			glVertexAttribPointer(attributeTexCoord0Index, 2, GL_FLOAT, false, 0, reinterpret_cast<GLvoid*>(basicShapes.internalSpherePositionArraySizeBytes));

			glDrawArrays(GL_TRIANGLES, 0, basicShapes.internalSphereFaceNumTriangles * 3);
		}

		// Render bottom face.
		{
			glBindBuffer(GL_ARRAY_BUFFER, basicShapes.internalSphereBottomFace);

			glBindTexture(GL_TEXTURE_2D, textureManager.GetTexture(currentToken->faceTextures[5]));
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

			// Set attribute offsets into VBO.
			glVertexPointer(3, GL_FLOAT, 0, 0);
			glVertexAttribPointer(attributeTexCoord0Index, 2, GL_FLOAT, false, 0, reinterpret_cast<GLvoid*>(basicShapes.internalSpherePositionArraySizeBytes));

			glDrawArrays(GL_TRIANGLES, 0, basicShapes.internalSphereFaceNumTriangles * 3);
		}
	}
}

void SubRendererSky::RestoreContextState()
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glDepthMask(GL_TRUE);

	ShaderProgram::DeactivateShaders();

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableVertexAttribArray(attributeTexCoord0Index);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}


}

#pragma warning(pop)