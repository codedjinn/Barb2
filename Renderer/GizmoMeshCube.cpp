#include "GizmoMeshCube.h"

namespace RendererLib
{

GizmoMeshCube::GizmoMeshCube() :
	GizmoMesh()
{
	// Generate the vbo.
	{
		GLfloat vertices[] =
		{
			-1.0f, 1.0f, 1.0f,				// TFL
			1.0f, 1.0f, 1.0f,				// TFR
			1.0f, -1.0f, 1.0f,				// BFR
			-1.0f, -1.0f, 1.0f,				// BFL
			-1.0f, 1.0f, -1.0f,				// TBL
			1.0f, 1.0f, -1.0f,				// TBR
			1.0f, -1.0f, -1.0f,				// BBR
			-1.0f, -1.0f, -1.0f				// BBL
		};
		numVertices = 8;

		glGenBuffers(1, &vboHandle);
		glBindBuffer(GL_ARRAY_BUFFER, vboHandle);
		glBufferData(GL_ARRAY_BUFFER,
					 8 * 3 * sizeof(GLfloat),
					 vertices,
					 GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		GLuint indices[] =
		{
			// Front face.
			0, 1, 3,
			1, 2, 3,

			// Right face.
			1, 5, 2,
			2, 5, 6,

			// Back face.
			5, 4, 6,
			6, 4, 7,

			// Left face.
			4, 0, 7,
			7, 0, 3,

			// Top face.
			4, 5, 0,
			0, 5, 1,

			// Bottom face.
			3, 2, 7,
			7, 2, 6
		};
		numIndices = 6 * 6;

		glGenBuffers(1, &indicesHandle);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indicesHandle);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, numIndices * sizeof(GLuint), indices, GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
}

GizmoMeshCube::~GizmoMeshCube()
{

}

}