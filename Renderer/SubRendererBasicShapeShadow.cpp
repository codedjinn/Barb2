#include "SubRendererBasicShapeShadow.h"
#include "Renderer.h"
#include "Texture.h"
#include "ShaderManager.h"
#include "RenderTokenBasicShape.h"
#include "BasicShapes.h"
#include <cassert>
#include <cstdlib>

namespace RendererLib
{

SubRendererBasicShapeShadow::SubRendererBasicShapeShadow(Renderer& renderer, const std::vector<RenderToken*>& set, const MathLib::matrix4x4& lightMatrix) :
	SubRenderer(renderer),
	renderableSet(set)
{
	MathLib::matrix4x4_copy(shadowBasisWorldProjectionMatrix, lightMatrix);
}

void SubRendererBasicShapeShadow::SetContextState()
{
	glCullFace(GL_FRONT);
	glEnable(GL_DEPTH_TEST);

	// Initialize shader.
	ShaderManager& shaderManager = ShaderManager::GetInstance();
	shader = shaderManager.GetShader(ShaderManager::SHADER_DEPTH_ONLY);
	assert(NULL != shader);
	shader->Use();

	glEnableClientState(GL_VERTEX_ARRAY);
}

void SubRendererBasicShapeShadow::Render()
{
	for (unsigned int i = 0; i < renderableSet.size(); i++)
	{
		RenderTokenBasicShape* currentToken = dynamic_cast<RenderTokenBasicShape*>(renderableSet[i]);
		assert(NULL != currentToken);

		// Establish transform matrices.
		{
			MathLib::matrix4x4 transform;

			MathLib::GenerateTransformMatrix(transform,
											 currentToken->position,
											 currentToken->orientation,
											 currentToken->scale);
			
			MathLib::matrix4x4 mvpMatrix;
			MathLib::matrix4x4_mul(shadowBasisWorldProjectionMatrix, transform, mvpMatrix);
			mvpMatrix.setOpenGLMatrix(matrixArray);
			shader->SetUniformMatrix4fv("transform", static_cast<GLfloat*>(matrixArray), false);
		}
		
		// Select appropriate meshes to render based off of the shape type.
		int shapeType = currentToken->shapeType;
		if (BasicShapes::BASIC_SHAPE_EXTERNAL_SPHERE == shapeType)
		{
			RenderExternalSphere();
		}
		else if (BasicShapes::BASIC_SHAPE_INTERNAL_SPHERE == shapeType)
		{
			RenderInternalSphere();
		}
		else if (BasicShapes::BASIC_SHAPE_EXTERNAL_BOX == shapeType)
		{
			RenderExternalBox();
		}
		else if (BasicShapes::BASIC_SHAPE_INTERNAL_BOX == shapeType)
		{
			RenderInternalBox();
		}
		else if (BasicShapes::BASIC_SHAPE_CAPSULE == shapeType)
		{
			RenderCapsule(currentToken->position, currentToken->orientation, currentToken->scale.extractX(), currentToken->scale.extractY());
		}
	}
}

void SubRendererBasicShapeShadow::RenderExternalSphere()
{
	BasicShapes& basicShapes = BasicShapes::GetInstance();

	TextureManager& textureManager = TextureManager::GetInstance();

	// Render front face.
	{
		glBindBuffer(GL_ARRAY_BUFFER, basicShapes.externalSphereFrontFace);

		// Set attribute offsets into VBO.
		glVertexPointer(3, GL_FLOAT, 0, 0);
		glDrawArrays(GL_TRIANGLES, 0, basicShapes.externalSphereFaceNumTriangles * 3);
	}
	
	// Render right face.
	{
		glBindBuffer(GL_ARRAY_BUFFER, basicShapes.externalSphereRightFace);

		// Set attribute offsets into VBO.
		glVertexPointer(3, GL_FLOAT, 0, 0);
		glDrawArrays(GL_TRIANGLES, 0, basicShapes.externalSphereFaceNumTriangles * 3);
	}

	// Render back face.
	{
		glBindBuffer(GL_ARRAY_BUFFER, basicShapes.externalSphereBackFace);

		// Set attribute offsets into VBO.
		glVertexPointer(3, GL_FLOAT, 0, 0);
		glDrawArrays(GL_TRIANGLES, 0, basicShapes.externalSphereFaceNumTriangles * 3);
	}

	// Render left face.
	{
		glBindBuffer(GL_ARRAY_BUFFER, basicShapes.externalSphereLeftFace);

		// Set attribute offsets into VBO.
		glVertexPointer(3, GL_FLOAT, 0, 0);
		glDrawArrays(GL_TRIANGLES, 0, basicShapes.externalSphereFaceNumTriangles * 3);
	}

	// Render top face.
	{
		glBindBuffer(GL_ARRAY_BUFFER, basicShapes.externalSphereTopFace);

		// Set attribute offsets into VBO.
		glVertexPointer(3, GL_FLOAT, 0, 0);
		glDrawArrays(GL_TRIANGLES, 0, basicShapes.externalSphereFaceNumTriangles * 3);
	}

	// Render bottom face.
	{
		glBindBuffer(GL_ARRAY_BUFFER, basicShapes.externalSphereBottomFace);

		// Set attribute offsets into VBO.
		glVertexPointer(3, GL_FLOAT, 0, 0);
		glDrawArrays(GL_TRIANGLES, 0, basicShapes.externalSphereFaceNumTriangles * 3);
	}
}

void SubRendererBasicShapeShadow::RenderInternalSphere()
{
	BasicShapes& basicShapes = BasicShapes::GetInstance();

	// Render front face.
	{
		glBindBuffer(GL_ARRAY_BUFFER, basicShapes.internalSphereFrontFace);

		// Set attribute offsets into VBO.
		glVertexPointer(3, GL_FLOAT, 0, 0);
		glDrawArrays(GL_TRIANGLES, 0, basicShapes.internalSphereFaceNumTriangles * 3);
	}
	
	// Render right face.
	{
		glBindBuffer(GL_ARRAY_BUFFER, basicShapes.internalSphereRightFace);

		// Set attribute offsets into VBO.
		glVertexPointer(3, GL_FLOAT, 0, 0);
		glDrawArrays(GL_TRIANGLES, 0, basicShapes.internalSphereFaceNumTriangles * 3);
	}

	// Render back face.
	{
		glBindBuffer(GL_ARRAY_BUFFER, basicShapes.internalSphereBackFace);

		// Set attribute offsets into VBO.
		glVertexPointer(3, GL_FLOAT, 0, 0);
		glDrawArrays(GL_TRIANGLES, 0, basicShapes.internalSphereFaceNumTriangles * 3);
	}

	// 	// Render left face.
	{
		glBindBuffer(GL_ARRAY_BUFFER, basicShapes.internalSphereLeftFace);

		// Set attribute offsets into VBO.
		glVertexPointer(3, GL_FLOAT, 0, 0);
		glDrawArrays(GL_TRIANGLES, 0, basicShapes.internalSphereFaceNumTriangles * 3);
	}

	// Render top face.
	{
		glBindBuffer(GL_ARRAY_BUFFER, basicShapes.internalSphereTopFace);

		// Set attribute offsets into VBO.
		glVertexPointer(3, GL_FLOAT, 0, 0);
		glDrawArrays(GL_TRIANGLES, 0, basicShapes.internalSphereFaceNumTriangles * 3);
	}

	// Render bottom face.
	{
		glBindBuffer(GL_ARRAY_BUFFER, basicShapes.internalSphereBottomFace);

		// Set attribute offsets into VBO.
		glVertexPointer(3, GL_FLOAT, 0, 0);
		glDrawArrays(GL_TRIANGLES, 0, basicShapes.internalSphereFaceNumTriangles * 3);
	}
}

void SubRendererBasicShapeShadow::RenderExternalBox()
{
	BasicShapes& basicShapes = BasicShapes::GetInstance();

	TextureManager& textureManager = TextureManager::GetInstance();

	// Render front face.
	{
		glBindBuffer(GL_ARRAY_BUFFER, basicShapes.externalBoxFrontFace);

		// Set attribute offsets into VBO.
		glVertexPointer(3, GL_FLOAT, 0, 0);
		glDrawArrays(GL_TRIANGLES, 0, basicShapes.externalBoxFaceNumTriangles * 3);
	}

	// Render right face.
	{
		glBindBuffer(GL_ARRAY_BUFFER, basicShapes.externalBoxRightFace);

		// Set attribute offsets into VBO.
		glVertexPointer(3, GL_FLOAT, 0, 0);
		glDrawArrays(GL_TRIANGLES, 0, basicShapes.externalBoxFaceNumTriangles * 3);
	}

	// Render back face.
	{
		glBindBuffer(GL_ARRAY_BUFFER, basicShapes.externalBoxBackFace);

		// Set attribute offsets into VBO.
		glVertexPointer(3, GL_FLOAT, 0, 0);
		glDrawArrays(GL_TRIANGLES, 0, basicShapes.externalBoxFaceNumTriangles * 3);
	}

	// Render left face.
	{
		glBindBuffer(GL_ARRAY_BUFFER, basicShapes.externalBoxLeftFace);

		// Set attribute offsets into VBO.
		glVertexPointer(3, GL_FLOAT, 0, 0);
		glDrawArrays(GL_TRIANGLES, 0, basicShapes.externalBoxFaceNumTriangles * 3);
	}

	// Render top face.
	{
		glBindBuffer(GL_ARRAY_BUFFER, basicShapes.externalBoxTopFace);

		// Set attribute offsets into VBO.
		glVertexPointer(3, GL_FLOAT, 0, 0);
		glDrawArrays(GL_TRIANGLES, 0, basicShapes.externalBoxFaceNumTriangles * 3);
	}

	// Render bottom face.
	{
		glBindBuffer(GL_ARRAY_BUFFER, basicShapes.externalBoxBottomFace);

		// Set attribute offsets into VBO.
		glVertexPointer(3, GL_FLOAT, 0, 0);
		glDrawArrays(GL_TRIANGLES, 0, basicShapes.externalBoxFaceNumTriangles * 3);
	}
}

void SubRendererBasicShapeShadow::RenderInternalBox()
{
	BasicShapes& basicShapes = BasicShapes::GetInstance();

	TextureManager& textureManager = TextureManager::GetInstance();

	// Render front face.
	{
		glBindBuffer(GL_ARRAY_BUFFER, basicShapes.internalBoxFrontFace);

		// Set attribute offsets into VBO.
		glVertexPointer(3, GL_FLOAT, 0, 0);
		glDrawArrays(GL_TRIANGLES, 0, basicShapes.internalBoxFaceNumTriangles * 3);
	}

	// Render right face.
	{
		glBindBuffer(GL_ARRAY_BUFFER, basicShapes.internalBoxRightFace);

		// Set attribute offsets into VBO.
		glVertexPointer(3, GL_FLOAT, 0, 0);
		glDrawArrays(GL_TRIANGLES, 0, basicShapes.internalBoxFaceNumTriangles * 3);
	}

	// Render back face.
	{
		glBindBuffer(GL_ARRAY_BUFFER, basicShapes.internalBoxBackFace);

		// Set attribute offsets into VBO.
		glVertexPointer(3, GL_FLOAT, 0, 0);
		glDrawArrays(GL_TRIANGLES, 0, basicShapes.internalBoxFaceNumTriangles * 3);
	}

	// Render left face.
	{
		glBindBuffer(GL_ARRAY_BUFFER, basicShapes.internalBoxLeftFace);

		// Set attribute offsets into VBO.
		glVertexPointer(3, GL_FLOAT, 0, 0);
		glDrawArrays(GL_TRIANGLES, 0, basicShapes.internalBoxFaceNumTriangles * 3);
	}

	// Render top face.
	{
		glBindBuffer(GL_ARRAY_BUFFER, basicShapes.internalBoxTopFace);

		// Set attribute offsets into VBO.
		glVertexPointer(3, GL_FLOAT, 0, 0);
		glDrawArrays(GL_TRIANGLES, 0, basicShapes.internalBoxFaceNumTriangles * 3);
	}

	// Render bottom face.
	{
		glBindBuffer(GL_ARRAY_BUFFER, basicShapes.internalBoxBottomFace);

		// Set attribute offsets into VBO.
		glVertexPointer(3, GL_FLOAT, 0, 0);
		glDrawArrays(GL_TRIANGLES, 0, basicShapes.internalBoxFaceNumTriangles * 3);
	}
}

void SubRendererBasicShapeShadow::RenderCapsule(const MathLib::vector4& position, const MathLib::quaternion& orientation, float halfHeight, float radius) 
{
	BasicShapes& basicShapes = BasicShapes::GetInstance();

	// Establish tube transform.
	{
		MathLib::matrix4x4 transform;
		MathLib::GenerateTransformMatrix(transform,
										 position,
										 orientation,
										 MathLib::vector4(halfHeight, radius, radius, 0.0f));

		MathLib::matrix4x4 mvpMatrix;
		MathLib::matrix4x4_mul(shadowBasisWorldProjectionMatrix, transform, mvpMatrix);
		mvpMatrix.setOpenGLMatrix(matrixArray);
		shader->SetUniformMatrix4fv("transform", static_cast<GLfloat*>(matrixArray), false);
	}
	
	// Render front face.
	{
		glBindBuffer(GL_ARRAY_BUFFER, basicShapes.capsuleFrontFace);

		// Set attribute offsets into VBO.
		glVertexPointer(3, GL_FLOAT, 0, 0);
		glDrawArrays(GL_TRIANGLES, 0, basicShapes.capsuleFaceNumTriangles * 3);
	}

	// Render top face.
	{
		glBindBuffer(GL_ARRAY_BUFFER, basicShapes.capsuleTopFace);

		// Set attribute offsets into VBO.
		glVertexPointer(3, GL_FLOAT, 0, 0);
		glDrawArrays(GL_TRIANGLES, 0, basicShapes.capsuleFaceNumTriangles * 3);
	}

	// Render back face.
	{
		glBindBuffer(GL_ARRAY_BUFFER, basicShapes.capsuleBackFace);

		// Set attribute offsets into VBO.
		glVertexPointer(3, GL_FLOAT, 0, 0);
		glDrawArrays(GL_TRIANGLES, 0, basicShapes.capsuleFaceNumTriangles * 3);
	}

	// Render bottom face.
	{
		glBindBuffer(GL_ARRAY_BUFFER, basicShapes.capsuleBottomFace);

		// Set attribute offsets into VBO.
		glVertexPointer(3, GL_FLOAT, 0, 0);
		glDrawArrays(GL_TRIANGLES, 0, basicShapes.capsuleFaceNumTriangles * 3);
	}

	// Top sphere transform.
	{
		MathLib::matrix4x4 transform;

		MathLib::vector4 topPosition(position);
		topPosition.setX(topPosition.extractX() + halfHeight);
		MathLib::GenerateTransformMatrix(transform,
										 topPosition,
										 orientation,
										 radius);

		MathLib::matrix4x4 mvpMatrix;
		MathLib::matrix4x4_mul(shadowBasisWorldProjectionMatrix, transform, mvpMatrix);
		mvpMatrix.setOpenGLMatrix(matrixArray);
		shader->SetUniformMatrix4fv("transform", static_cast<GLfloat*>(matrixArray), false);
	}
	RenderExternalSphere();

	// Bottom sphere transform.
	{
		MathLib::matrix4x4 transform;

		MathLib::vector4 bottomPosition(position);
		bottomPosition.setX(bottomPosition.extractX() - halfHeight);
		MathLib::GenerateTransformMatrix(transform,
										 bottomPosition,
										 orientation,
										 radius);
	
		MathLib::matrix4x4 mvpMatrix;
		MathLib::matrix4x4_mul(shadowBasisWorldProjectionMatrix, transform, mvpMatrix);
		mvpMatrix.setOpenGLMatrix(matrixArray);
		shader->SetUniformMatrix4fv("transform", static_cast<GLfloat*>(matrixArray), false);
	}
	RenderExternalSphere();
}

void SubRendererBasicShapeShadow::RestoreContextState()
{
	ShaderProgram::DeactivateShaders();

	glDisableClientState(GL_VERTEX_ARRAY);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	
	glCullFace(GL_BACK);
}

}