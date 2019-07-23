#include "SubRendererBasicShape.h"
#include "Renderer.h"
#include "Texture.h"
#include "ShaderManager.h"
#include "RenderTokenBasicShape.h"
#include "BasicShapes.h"
#include <cassert>
#include <cstdlib>

#pragma warning(push)
#pragma warning(disable:4312)

namespace RendererLib
{

SubRendererBasicShape::SubRendererBasicShape(Renderer& renderer, const std::vector<RenderToken*>& set) :
	SubRenderer(renderer),
	renderableSet(set)
{
}

void SubRendererBasicShape::SetContextState()
{
	glCullFace(GL_BACK);
	glEnable(GL_DEPTH_TEST);

	// Initialize shader.
	ShaderManager& shaderManager = ShaderManager::GetInstance();
	shapeShader = shaderManager.GetShader(ShaderManager::SHADER_DEFAULT_MESH);
	assert(NULL != shapeShader);
	shapeShader->Use();

	GLuint programHandle = shapeShader->GetProgramHandle();
	attributeTexCoord0Index = glGetAttribLocation(programHandle, "aTexCoord0");
	attributeNormalIndex = glGetAttribLocation(programHandle, "aNormal");

	if (rendererHandle.GetRenderWireframe())
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	else
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableVertexAttribArray(attributeTexCoord0Index);
	glEnableVertexAttribArray(attributeNormalIndex);
}

void SubRendererBasicShape::Render()
{
	TextureManager& textureManager = TextureManager::GetInstance();

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
			MathLib::matrix4x4_copy(rendererHandle.matrices[MATRIX_OBJECT_WORLD_INDEX], transform);
			rendererHandle.UpdateModelViewProjectionMatrix();

			rendererHandle.matrices[MATRIX_MVP_INDEX].setOpenGLMatrix(matrixArray);
			shapeShader->SetUniformMatrix4fv("transform", static_cast<GLfloat*>(matrixArray), false);

			MathLib::matrix4x4 inverseMatrix;
			MathLib::matrix4x4_inverse(rendererHandle.matrices[MATRIX_MODEL_VIEW_INDEX], inverseMatrix);
			MathLib::matrix4x4_transpose(inverseMatrix);
			inverseMatrix.setOpenGLMatrix(matrixArray);
			shapeShader->SetUniformMatrix4fv("modelTransform", static_cast<GLfloat*>(matrixArray), false);
		}
		
		// Select appropriate meshes to render based off of the shape type.
		int shapeType = currentToken->shapeType;
		if (BasicShapes::BASIC_SHAPE_EXTERNAL_SPHERE == shapeType)
		{
			RenderExternalSphere(currentToken->faceTextures);
		}
		else if (BasicShapes::BASIC_SHAPE_INTERNAL_SPHERE == shapeType)
		{
			RenderInternalSphere(currentToken->faceTextures);
		}
		else if (BasicShapes::BASIC_SHAPE_EXTERNAL_BOX == shapeType)
		{
			RenderExternalBox(currentToken->faceTextures);
		}
		else if (BasicShapes::BASIC_SHAPE_INTERNAL_BOX == shapeType)
		{
			RenderInternalBox(currentToken->faceTextures);
		}
		else if (BasicShapes::BASIC_SHAPE_CAPSULE == shapeType)
		{
			RenderCapsule(currentToken->position, currentToken->orientation, currentToken->scale.extractX(), currentToken->scale.extractY());
		}

	}
}

void SubRendererBasicShape::RenderExternalSphere(std::vector<std::string>& faceTextures)
{
	BasicShapes& basicShapes = BasicShapes::GetInstance();

	TextureManager& textureManager = TextureManager::GetInstance();

	// Render front face.
	{
		glBindTexture(GL_TEXTURE_2D, textureManager.GetTexture(faceTextures[0]));
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		glBindBuffer(GL_ARRAY_BUFFER, basicShapes.externalSphereFrontFace);

		// Set attribute offsets into VBO.
		glVertexPointer(3, GL_FLOAT, 0, 0);

		glVertexAttribPointer(attributeTexCoord0Index, 2, GL_FLOAT, false, 0, reinterpret_cast<GLvoid*>(basicShapes.externalSpherePositionArraySizeBytes));
		glVertexAttribPointer(attributeNormalIndex, 3, GL_FLOAT, false, 0, reinterpret_cast<GLvoid*>(basicShapes.externalSpherePositionArraySizeBytes + basicShapes.externalSphereTexCoordArraySizeBytes));

		glDrawArrays(GL_TRIANGLES, 0, basicShapes.externalSphereFaceNumTriangles * 3);
	}
	
	// Render right face.
	{
		glBindTexture(GL_TEXTURE_2D, textureManager.GetTexture(faceTextures[1]));
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		glBindBuffer(GL_ARRAY_BUFFER, basicShapes.externalSphereRightFace);

		// Set attribute offsets into VBO.
		glVertexPointer(3, GL_FLOAT, 0, 0);

		glVertexAttribPointer(attributeTexCoord0Index, 2, GL_FLOAT, false, 0, reinterpret_cast<GLvoid*>(basicShapes.externalSpherePositionArraySizeBytes));
		glVertexAttribPointer(attributeNormalIndex, 3, GL_FLOAT, false, 0, reinterpret_cast<GLvoid*>(basicShapes.externalSpherePositionArraySizeBytes + basicShapes.externalSphereTexCoordArraySizeBytes));

		glDrawArrays(GL_TRIANGLES, 0, basicShapes.externalSphereFaceNumTriangles * 3);
	}

	// Render back face.
	{
		glBindTexture(GL_TEXTURE_2D, textureManager.GetTexture(faceTextures[2]));
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		glBindBuffer(GL_ARRAY_BUFFER, basicShapes.externalSphereBackFace);

		// Set attribute offsets into VBO.
		glVertexPointer(3, GL_FLOAT, 0, 0);

		glVertexAttribPointer(attributeTexCoord0Index, 2, GL_FLOAT, false, 0, reinterpret_cast<GLvoid*>(basicShapes.externalSpherePositionArraySizeBytes));
		glVertexAttribPointer(attributeNormalIndex, 3, GL_FLOAT, false, 0, reinterpret_cast<GLvoid*>(basicShapes.externalSpherePositionArraySizeBytes + basicShapes.externalSphereTexCoordArraySizeBytes));

		glDrawArrays(GL_TRIANGLES, 0, basicShapes.externalSphereFaceNumTriangles * 3);
	}

	// Render left face.
	{
		glBindTexture(GL_TEXTURE_2D, textureManager.GetTexture(faceTextures[3]));
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		glBindBuffer(GL_ARRAY_BUFFER, basicShapes.externalSphereLeftFace);

		// Set attribute offsets into VBO.
		glVertexPointer(3, GL_FLOAT, 0, 0);

		glVertexAttribPointer(attributeTexCoord0Index, 2, GL_FLOAT, false, 0, reinterpret_cast<GLvoid*>(basicShapes.externalSpherePositionArraySizeBytes));
		glVertexAttribPointer(attributeNormalIndex, 3, GL_FLOAT, false, 0, reinterpret_cast<GLvoid*>(basicShapes.externalSpherePositionArraySizeBytes + basicShapes.externalSphereTexCoordArraySizeBytes));

		glDrawArrays(GL_TRIANGLES, 0, basicShapes.externalSphereFaceNumTriangles * 3);
	}

	// Render top face.
	{
		glBindTexture(GL_TEXTURE_2D, textureManager.GetTexture(faceTextures[4]));
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		glBindBuffer(GL_ARRAY_BUFFER, basicShapes.externalSphereTopFace);

		// Set attribute offsets into VBO.
		glVertexPointer(3, GL_FLOAT, 0, 0);

		glVertexAttribPointer(attributeTexCoord0Index, 2, GL_FLOAT, false, 0, reinterpret_cast<GLvoid*>(basicShapes.externalSpherePositionArraySizeBytes));
		glVertexAttribPointer(attributeNormalIndex, 3, GL_FLOAT, false, 0, reinterpret_cast<GLvoid*>(basicShapes.externalSpherePositionArraySizeBytes + basicShapes.externalSphereTexCoordArraySizeBytes));

		glDrawArrays(GL_TRIANGLES, 0, basicShapes.externalSphereFaceNumTriangles * 3);
	}

	// Render bottom face.
	{
		glBindTexture(GL_TEXTURE_2D, textureManager.GetTexture(faceTextures[5]));
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		glBindBuffer(GL_ARRAY_BUFFER, basicShapes.externalSphereBottomFace);

		// Set attribute offsets into VBO.
		glVertexPointer(3, GL_FLOAT, 0, 0);
		
		glVertexAttribPointer(attributeTexCoord0Index, 2, GL_FLOAT, false, 0, reinterpret_cast<GLvoid*>(basicShapes.externalSpherePositionArraySizeBytes));
		glVertexAttribPointer(attributeNormalIndex, 3, GL_FLOAT, false, 0, reinterpret_cast<GLvoid*>(basicShapes.externalSpherePositionArraySizeBytes + basicShapes.externalSphereTexCoordArraySizeBytes));

		glDrawArrays(GL_TRIANGLES, 0, basicShapes.externalSphereFaceNumTriangles * 3);
	}
}

void SubRendererBasicShape::RenderInternalSphere(std::vector<std::string>& faceTextures)
{
	BasicShapes& basicShapes = BasicShapes::GetInstance();

	TextureManager& textureManager = TextureManager::GetInstance();

	// Render front face.
	{
		glBindBuffer(GL_ARRAY_BUFFER, basicShapes.internalSphereFrontFace);

		glBindTexture(GL_TEXTURE_2D, textureManager.GetTexture(faceTextures[0]));
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		// Set attribute offsets into VBO.
		glVertexPointer(3, GL_FLOAT, 0, 0);

		glVertexAttribPointer(attributeTexCoord0Index, 2, GL_FLOAT, false, 0, reinterpret_cast<GLvoid*>(basicShapes.internalSpherePositionArraySizeBytes));
		glVertexAttribPointer(attributeNormalIndex, 3, GL_FLOAT, false, 0, reinterpret_cast<GLvoid*>(basicShapes.internalSpherePositionArraySizeBytes + basicShapes.internalSphereTexCoordArraySizeBytes));

		glDrawArrays(GL_TRIANGLES, 0, basicShapes.internalSphereFaceNumTriangles * 3);
	}
	
	// Render right face.
	{
		glBindBuffer(GL_ARRAY_BUFFER, basicShapes.internalSphereRightFace);

		glBindTexture(GL_TEXTURE_2D, textureManager.GetTexture(faceTextures[1]));
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		// Set attribute offsets into VBO.
		glVertexPointer(3, GL_FLOAT, 0, 0);

		glVertexAttribPointer(attributeTexCoord0Index, 2, GL_FLOAT, false, 0, reinterpret_cast<GLvoid*>(basicShapes.internalSpherePositionArraySizeBytes));
		glVertexAttribPointer(attributeNormalIndex, 3, GL_FLOAT, false, 0, reinterpret_cast<GLvoid*>(basicShapes.internalSpherePositionArraySizeBytes + basicShapes.internalSphereTexCoordArraySizeBytes));

		glDrawArrays(GL_TRIANGLES, 0, basicShapes.internalSphereFaceNumTriangles * 3);
	}

	// Render back face.
	{
		glBindBuffer(GL_ARRAY_BUFFER, basicShapes.internalSphereBackFace);

		glBindTexture(GL_TEXTURE_2D, textureManager.GetTexture(faceTextures[2]));
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		// Set attribute offsets into VBO.
		glVertexPointer(3, GL_FLOAT, 0, 0);

		glVertexAttribPointer(attributeTexCoord0Index, 2, GL_FLOAT, false, 0, reinterpret_cast<GLvoid*>(basicShapes.internalSpherePositionArraySizeBytes));
		glVertexAttribPointer(attributeNormalIndex, 3, GL_FLOAT, false, 0, reinterpret_cast<GLvoid*>(basicShapes.internalSpherePositionArraySizeBytes + basicShapes.internalSphereTexCoordArraySizeBytes));

		glDrawArrays(GL_TRIANGLES, 0, basicShapes.internalSphereFaceNumTriangles * 3);
	}

	// 	// Render left face.
	{
		glBindBuffer(GL_ARRAY_BUFFER, basicShapes.internalSphereLeftFace);

		glBindTexture(GL_TEXTURE_2D, textureManager.GetTexture(faceTextures[3]));
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		// Set attribute offsets into VBO.
		glVertexPointer(3, GL_FLOAT, 0, 0);

		glVertexAttribPointer(attributeTexCoord0Index, 2, GL_FLOAT, false, 0, reinterpret_cast<GLvoid*>(basicShapes.internalSpherePositionArraySizeBytes));
		glVertexAttribPointer(attributeNormalIndex, 3, GL_FLOAT, false, 0, reinterpret_cast<GLvoid*>(basicShapes.internalSpherePositionArraySizeBytes + basicShapes.internalSphereTexCoordArraySizeBytes));

		glDrawArrays(GL_TRIANGLES, 0, basicShapes.internalSphereFaceNumTriangles * 3);
	}

	// Render top face.
	{
		glBindBuffer(GL_ARRAY_BUFFER, basicShapes.internalSphereTopFace);

		glBindTexture(GL_TEXTURE_2D, textureManager.GetTexture(faceTextures[4]));
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		// Set attribute offsets into VBO.
		glVertexPointer(3, GL_FLOAT, 0, 0);

		glVertexAttribPointer(attributeTexCoord0Index, 2, GL_FLOAT, false, 0, reinterpret_cast<GLvoid*>(basicShapes.internalSpherePositionArraySizeBytes));
		glVertexAttribPointer(attributeNormalIndex, 3, GL_FLOAT, false, 0, reinterpret_cast<GLvoid*>(basicShapes.internalSpherePositionArraySizeBytes + basicShapes.internalSphereTexCoordArraySizeBytes));

		glDrawArrays(GL_TRIANGLES, 0, basicShapes.internalSphereFaceNumTriangles * 3);
	}

	// Render bottom face.
	{
		glBindBuffer(GL_ARRAY_BUFFER, basicShapes.internalSphereBottomFace);

		glBindTexture(GL_TEXTURE_2D, textureManager.GetTexture(faceTextures[5]));
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		// Set attribute offsets into VBO.
		glVertexPointer(3, GL_FLOAT, 0, 0);

		glVertexAttribPointer(attributeTexCoord0Index, 2, GL_FLOAT, false, 0, reinterpret_cast<GLvoid*>(basicShapes.internalSpherePositionArraySizeBytes));
		glVertexAttribPointer(attributeNormalIndex, 3, GL_FLOAT, false, 0, reinterpret_cast<GLvoid*>(basicShapes.internalSpherePositionArraySizeBytes + basicShapes.internalSphereTexCoordArraySizeBytes));

		glDrawArrays(GL_TRIANGLES, 0, basicShapes.internalSphereFaceNumTriangles * 3);
	}
}

void SubRendererBasicShape::RenderExternalBox(std::vector<std::string>& faceTextures)
{
	BasicShapes& basicShapes = BasicShapes::GetInstance();

	TextureManager& textureManager = TextureManager::GetInstance();

	// Render front face.
	{
		glBindTexture(GL_TEXTURE_2D, textureManager.GetTexture(faceTextures[0]));
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		glBindBuffer(GL_ARRAY_BUFFER, basicShapes.externalBoxFrontFace);

		// Set attribute offsets into VBO.
		glVertexPointer(3, GL_FLOAT, 0, 0);

		glVertexAttribPointer(attributeTexCoord0Index, 2, GL_FLOAT, false, 0, reinterpret_cast<GLvoid*>(basicShapes.externalBoxPositionArraySizeBytes));
		glVertexAttribPointer(attributeNormalIndex, 3, GL_FLOAT, false, 0, reinterpret_cast<GLvoid*>(basicShapes.externalBoxPositionArraySizeBytes + basicShapes.externalBoxTexCoordArraySizeBytes));

		glDrawArrays(GL_TRIANGLES, 0, basicShapes.externalBoxFaceNumTriangles * 3);
	}

	// Render right face.
	{
		glBindTexture(GL_TEXTURE_2D, textureManager.GetTexture(faceTextures[1]));
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		glBindBuffer(GL_ARRAY_BUFFER, basicShapes.externalBoxRightFace);

		// Set attribute offsets into VBO.
		glVertexPointer(3, GL_FLOAT, 0, 0);

		glVertexAttribPointer(attributeTexCoord0Index, 2, GL_FLOAT, false, 0, reinterpret_cast<GLvoid*>(basicShapes.externalBoxPositionArraySizeBytes));
		glVertexAttribPointer(attributeNormalIndex, 3, GL_FLOAT, false, 0, reinterpret_cast<GLvoid*>(basicShapes.externalBoxPositionArraySizeBytes + basicShapes.externalBoxTexCoordArraySizeBytes));

		glDrawArrays(GL_TRIANGLES, 0, basicShapes.externalBoxFaceNumTriangles * 3);
	}

	// Render back face.
	{
		glBindTexture(GL_TEXTURE_2D, textureManager.GetTexture(faceTextures[2]));
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		glBindBuffer(GL_ARRAY_BUFFER, basicShapes.externalBoxBackFace);

		// Set attribute offsets into VBO.
		glVertexPointer(3, GL_FLOAT, 0, 0);

		glVertexAttribPointer(attributeTexCoord0Index, 2, GL_FLOAT, false, 0, reinterpret_cast<GLvoid*>(basicShapes.externalBoxPositionArraySizeBytes));
		glVertexAttribPointer(attributeNormalIndex, 3, GL_FLOAT, false, 0, reinterpret_cast<GLvoid*>(basicShapes.externalBoxPositionArraySizeBytes + basicShapes.externalBoxTexCoordArraySizeBytes));

		glDrawArrays(GL_TRIANGLES, 0, basicShapes.externalBoxFaceNumTriangles * 3);
	}

	// Render left face.
	{
		glBindTexture(GL_TEXTURE_2D, textureManager.GetTexture(faceTextures[3]));
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		glBindBuffer(GL_ARRAY_BUFFER, basicShapes.externalBoxLeftFace);

		// Set attribute offsets into VBO.
		glVertexPointer(3, GL_FLOAT, 0, 0);

		glVertexAttribPointer(attributeTexCoord0Index, 2, GL_FLOAT, false, 0, reinterpret_cast<GLvoid*>(basicShapes.externalBoxPositionArraySizeBytes));
		glVertexAttribPointer(attributeNormalIndex, 3, GL_FLOAT, false, 0, reinterpret_cast<GLvoid*>(basicShapes.externalBoxPositionArraySizeBytes + basicShapes.externalBoxTexCoordArraySizeBytes));

		glDrawArrays(GL_TRIANGLES, 0, basicShapes.externalBoxFaceNumTriangles * 3);
	}

	// Render top face.
	{
		glBindTexture(GL_TEXTURE_2D, textureManager.GetTexture(faceTextures[4]));
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		glBindBuffer(GL_ARRAY_BUFFER, basicShapes.externalBoxTopFace);

		// Set attribute offsets into VBO.
		glVertexPointer(3, GL_FLOAT, 0, 0);

		glVertexAttribPointer(attributeTexCoord0Index, 2, GL_FLOAT, false, 0, reinterpret_cast<GLvoid*>(basicShapes.externalBoxPositionArraySizeBytes));
		glVertexAttribPointer(attributeNormalIndex, 3, GL_FLOAT, false, 0, reinterpret_cast<GLvoid*>(basicShapes.externalBoxPositionArraySizeBytes + basicShapes.externalBoxTexCoordArraySizeBytes));

		glDrawArrays(GL_TRIANGLES, 0, basicShapes.externalBoxFaceNumTriangles * 3);
	}

	// Render bottom face.
	{
		glBindTexture(GL_TEXTURE_2D, textureManager.GetTexture(faceTextures[5]));
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		glBindBuffer(GL_ARRAY_BUFFER, basicShapes.externalBoxBottomFace);

		// Set attribute offsets into VBO.
		glVertexPointer(3, GL_FLOAT, 0, 0);

		glVertexAttribPointer(attributeTexCoord0Index, 2, GL_FLOAT, false, 0, reinterpret_cast<GLvoid*>(basicShapes.externalBoxPositionArraySizeBytes));
		glVertexAttribPointer(attributeNormalIndex, 3, GL_FLOAT, false, 0, reinterpret_cast<GLvoid*>(basicShapes.externalBoxPositionArraySizeBytes + basicShapes.externalBoxTexCoordArraySizeBytes));

		glDrawArrays(GL_TRIANGLES, 0, basicShapes.externalBoxFaceNumTriangles * 3);
	}
}

void SubRendererBasicShape::RenderInternalBox(std::vector<std::string>& faceTextures)
{
	BasicShapes& basicShapes = BasicShapes::GetInstance();

	TextureManager& textureManager = TextureManager::GetInstance();

	// Render front face.
	{
		glBindTexture(GL_TEXTURE_2D, textureManager.GetTexture(faceTextures[0]));
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		glBindBuffer(GL_ARRAY_BUFFER, basicShapes.internalBoxFrontFace);

		// Set attribute offsets into VBO.
		glVertexPointer(3, GL_FLOAT, 0, 0);

		glVertexAttribPointer(attributeTexCoord0Index, 2, GL_FLOAT, false, 0, reinterpret_cast<GLvoid*>(basicShapes.internalBoxPositionArraySizeBytes));
		glVertexAttribPointer(attributeNormalIndex, 3, GL_FLOAT, false, 0, reinterpret_cast<GLvoid*>(basicShapes.internalBoxPositionArraySizeBytes + basicShapes.internalBoxTexCoordArraySizeBytes));

		glDrawArrays(GL_TRIANGLES, 0, basicShapes.internalBoxFaceNumTriangles * 3);
	}

	// Render right face.
	{
		glBindTexture(GL_TEXTURE_2D, textureManager.GetTexture(faceTextures[1]));
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		glBindBuffer(GL_ARRAY_BUFFER, basicShapes.internalBoxRightFace);

		// Set attribute offsets into VBO.
		glVertexPointer(3, GL_FLOAT, 0, 0);

		glVertexAttribPointer(attributeTexCoord0Index, 2, GL_FLOAT, false, 0, reinterpret_cast<GLvoid*>(basicShapes.internalBoxPositionArraySizeBytes));
		glVertexAttribPointer(attributeNormalIndex, 3, GL_FLOAT, false, 0, reinterpret_cast<GLvoid*>(basicShapes.internalBoxPositionArraySizeBytes + basicShapes.internalBoxTexCoordArraySizeBytes));

		glDrawArrays(GL_TRIANGLES, 0, basicShapes.internalBoxFaceNumTriangles * 3);
	}

	// Render back face.
	{
		glBindTexture(GL_TEXTURE_2D, textureManager.GetTexture(faceTextures[2]));
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		glBindBuffer(GL_ARRAY_BUFFER, basicShapes.internalBoxBackFace);

		// Set attribute offsets into VBO.
		glVertexPointer(3, GL_FLOAT, 0, 0);

		glVertexAttribPointer(attributeTexCoord0Index, 2, GL_FLOAT, false, 0, reinterpret_cast<GLvoid*>(basicShapes.internalBoxPositionArraySizeBytes));
		glVertexAttribPointer(attributeNormalIndex, 3, GL_FLOAT, false, 0, reinterpret_cast<GLvoid*>(basicShapes.internalBoxPositionArraySizeBytes + basicShapes.internalBoxTexCoordArraySizeBytes));

		glDrawArrays(GL_TRIANGLES, 0, basicShapes.internalBoxFaceNumTriangles * 3);
	}

	// Render left face.
	{
		glBindTexture(GL_TEXTURE_2D, textureManager.GetTexture(faceTextures[3]));
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		glBindBuffer(GL_ARRAY_BUFFER, basicShapes.internalBoxLeftFace);

		// Set attribute offsets into VBO.
		glVertexPointer(3, GL_FLOAT, 0, 0);

		glVertexAttribPointer(attributeTexCoord0Index, 2, GL_FLOAT, false, 0, reinterpret_cast<GLvoid*>(basicShapes.internalBoxPositionArraySizeBytes));
		glVertexAttribPointer(attributeNormalIndex, 3, GL_FLOAT, false, 0, reinterpret_cast<GLvoid*>(basicShapes.internalBoxPositionArraySizeBytes + basicShapes.internalBoxTexCoordArraySizeBytes));

		glDrawArrays(GL_TRIANGLES, 0, basicShapes.internalBoxFaceNumTriangles * 3);
	}

	// Render top face.
	{
		glBindTexture(GL_TEXTURE_2D, textureManager.GetTexture(faceTextures[4]));
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		glBindBuffer(GL_ARRAY_BUFFER, basicShapes.internalBoxTopFace);

		// Set attribute offsets into VBO.
		glVertexPointer(3, GL_FLOAT, 0, 0);

		glVertexAttribPointer(attributeTexCoord0Index, 2, GL_FLOAT, false, 0, reinterpret_cast<GLvoid*>(basicShapes.internalBoxPositionArraySizeBytes));
		glVertexAttribPointer(attributeNormalIndex, 3, GL_FLOAT, false, 0, reinterpret_cast<GLvoid*>(basicShapes.internalBoxPositionArraySizeBytes + basicShapes.internalBoxTexCoordArraySizeBytes));

		glDrawArrays(GL_TRIANGLES, 0, basicShapes.internalBoxFaceNumTriangles * 3);
	}

	// Render bottom face.
	{
		glBindTexture(GL_TEXTURE_2D, textureManager.GetTexture(faceTextures[5]));
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		glBindBuffer(GL_ARRAY_BUFFER, basicShapes.internalBoxBottomFace);

		// Set attribute offsets into VBO.
		glVertexPointer(3, GL_FLOAT, 0, 0);

		glVertexAttribPointer(attributeTexCoord0Index, 2, GL_FLOAT, false, 0, reinterpret_cast<GLvoid*>(basicShapes.internalBoxPositionArraySizeBytes));
		glVertexAttribPointer(attributeNormalIndex, 3, GL_FLOAT, false, 0, reinterpret_cast<GLvoid*>(basicShapes.internalBoxPositionArraySizeBytes + basicShapes.internalBoxTexCoordArraySizeBytes));

		glDrawArrays(GL_TRIANGLES, 0, basicShapes.internalBoxFaceNumTriangles * 3);
	}
}

void SubRendererBasicShape::RenderCapsule(const MathLib::vector4& position, const MathLib::quaternion& orientation, float halfHeight, float radius) 
{
	BasicShapes& basicShapes = BasicShapes::GetInstance();

	// Establish tube transform.
	{
		MathLib::matrix4x4 transform;

		MathLib::GenerateTransformMatrix(transform,
										 position,
										 orientation,
										 MathLib::vector4(halfHeight, radius, radius, 0.0f));
		MathLib::matrix4x4_copy(rendererHandle.matrices[MATRIX_OBJECT_WORLD_INDEX], transform);
		rendererHandle.UpdateModelViewProjectionMatrix();

		rendererHandle.matrices[MATRIX_MVP_INDEX].setOpenGLMatrix(matrixArray);
		shapeShader->SetUniformMatrix4fv("transform", static_cast<GLfloat*>(matrixArray), false);

		MathLib::matrix4x4 inverseMatrix;
		MathLib::matrix4x4_inverse(rendererHandle.matrices[MATRIX_MODEL_VIEW_INDEX], inverseMatrix);
		MathLib::matrix4x4_transpose(inverseMatrix);
		inverseMatrix.setOpenGLMatrix(matrixArray);
		shapeShader->SetUniformMatrix4fv("modelTransform", static_cast<GLfloat*>(matrixArray), false);
	}

	TextureManager& textureManager = TextureManager::GetInstance();
	GLuint blankTexture = textureManager.GetTexture("");

	// Render front face.
	{
		glBindTexture(GL_TEXTURE_2D, blankTexture);

		glBindBuffer(GL_ARRAY_BUFFER, basicShapes.capsuleFrontFace);

		// Set attribute offsets into VBO.
		glVertexPointer(3, GL_FLOAT, 0, 0);

		glVertexAttribPointer(attributeTexCoord0Index, 2, GL_FLOAT, false, 0, reinterpret_cast<GLvoid*>(basicShapes.capsulePositionArraySizeBytes));
		glVertexAttribPointer(attributeNormalIndex, 3, GL_FLOAT, false, 0, reinterpret_cast<GLvoid*>(basicShapes.capsulePositionArraySizeBytes + basicShapes.capsuleTexCoordArraySizeBytes));

		glDrawArrays(GL_TRIANGLES, 0, basicShapes.capsuleFaceNumTriangles * 3);
	}

	// Render top face.
	{
		glBindTexture(GL_TEXTURE_2D, blankTexture);

		glBindBuffer(GL_ARRAY_BUFFER, basicShapes.capsuleTopFace);

		// Set attribute offsets into VBO.
		glVertexPointer(3, GL_FLOAT, 0, 0);

		glVertexAttribPointer(attributeTexCoord0Index, 2, GL_FLOAT, false, 0, reinterpret_cast<GLvoid*>(basicShapes.capsulePositionArraySizeBytes));
		glVertexAttribPointer(attributeNormalIndex, 3, GL_FLOAT, false, 0, reinterpret_cast<GLvoid*>(basicShapes.capsulePositionArraySizeBytes + basicShapes.capsuleTexCoordArraySizeBytes));

		glDrawArrays(GL_TRIANGLES, 0, basicShapes.capsuleFaceNumTriangles * 3);
	}

	// Render back face.
	{
		glBindTexture(GL_TEXTURE_2D, blankTexture);

		glBindBuffer(GL_ARRAY_BUFFER, basicShapes.capsuleBackFace);

		// Set attribute offsets into VBO.
		glVertexPointer(3, GL_FLOAT, 0, 0);

		glVertexAttribPointer(attributeTexCoord0Index, 2, GL_FLOAT, false, 0, reinterpret_cast<GLvoid*>(basicShapes.capsulePositionArraySizeBytes));
		glVertexAttribPointer(attributeNormalIndex, 3, GL_FLOAT, false, 0, reinterpret_cast<GLvoid*>(basicShapes.capsulePositionArraySizeBytes + basicShapes.capsuleTexCoordArraySizeBytes));

		glDrawArrays(GL_TRIANGLES, 0, basicShapes.capsuleFaceNumTriangles * 3);
	}

	// Render bottom face.
	{
		glBindTexture(GL_TEXTURE_2D, blankTexture);

		glBindBuffer(GL_ARRAY_BUFFER, basicShapes.capsuleBottomFace);

		// Set attribute offsets into VBO.
		glVertexPointer(3, GL_FLOAT, 0, 0);

		glVertexAttribPointer(attributeTexCoord0Index, 2, GL_FLOAT, false, 0, reinterpret_cast<GLvoid*>(basicShapes.capsulePositionArraySizeBytes));
		glVertexAttribPointer(attributeNormalIndex, 3, GL_FLOAT, false, 0, reinterpret_cast<GLvoid*>(basicShapes.capsulePositionArraySizeBytes + basicShapes.capsuleTexCoordArraySizeBytes));

		glDrawArrays(GL_TRIANGLES, 0, basicShapes.capsuleFaceNumTriangles * 3);
	}

	std::vector<std::string> faceTextures;
	for (int i = 0; i < 6; i++)
	{
		faceTextures.push_back("");
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
		MathLib::matrix4x4_copy(rendererHandle.matrices[MATRIX_OBJECT_WORLD_INDEX], transform);
		rendererHandle.UpdateModelViewProjectionMatrix();

		rendererHandle.matrices[MATRIX_MVP_INDEX].setOpenGLMatrix(matrixArray);
		shapeShader->SetUniformMatrix4fv("transform", static_cast<GLfloat*>(matrixArray), false);

		MathLib::matrix4x4 inverseMatrix;
		MathLib::matrix4x4_inverse(rendererHandle.matrices[MATRIX_MODEL_VIEW_INDEX], inverseMatrix);
		MathLib::matrix4x4_transpose(inverseMatrix);
		inverseMatrix.setOpenGLMatrix(matrixArray);
		shapeShader->SetUniformMatrix4fv("modelTransform", static_cast<GLfloat*>(matrixArray), false);
	}
	RenderExternalSphere(faceTextures);

	// Bottom sphere transform.
	{
		MathLib::matrix4x4 transform;

		MathLib::vector4 bottomPosition(position);
		bottomPosition.setX(bottomPosition.extractX() - halfHeight);
		MathLib::GenerateTransformMatrix(transform,
										 bottomPosition,
										 orientation,
										 radius);
		MathLib::matrix4x4_copy(rendererHandle.matrices[MATRIX_OBJECT_WORLD_INDEX], transform);
		rendererHandle.UpdateModelViewProjectionMatrix();

		rendererHandle.matrices[MATRIX_MVP_INDEX].setOpenGLMatrix(matrixArray);
		shapeShader->SetUniformMatrix4fv("transform", static_cast<GLfloat*>(matrixArray), false);

		MathLib::matrix4x4 inverseMatrix;
		MathLib::matrix4x4_inverse(rendererHandle.matrices[MATRIX_MODEL_VIEW_INDEX], inverseMatrix);
		MathLib::matrix4x4_transpose(inverseMatrix);
		inverseMatrix.setOpenGLMatrix(matrixArray);
		shapeShader->SetUniformMatrix4fv("modelTransform", static_cast<GLfloat*>(matrixArray), false);
	}
	RenderExternalSphere(faceTextures);
}

void SubRendererBasicShape::RestoreContextState()
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	ShaderProgram::DeactivateShaders();

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableVertexAttribArray(attributeTexCoord0Index);
	glDisableVertexAttribArray(attributeNormalIndex);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

}

#pragma warning(pop)