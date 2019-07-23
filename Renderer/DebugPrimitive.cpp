#include "DebugPrimitive.h"
#include "Renderer.h"

using namespace RendererLib;

DebugPrimitive::DebugPrimitive(const MathLib::matrix4x4& transform, const MathLib::vector4& color)
{
	MathLib::matrix4x4_copy(DebugPrimitive::transform, transform);
	MathLib::vector4_copy(DebugPrimitive::color, color);
}

DebugPrimitiveSphere::DebugPrimitiveSphere(const MathLib::matrix4x4& transform, const MathLib::vector4& color) :
	DebugPrimitive(transform, color)
{
}

DebugPrimitiveSphere::~DebugPrimitiveSphere()
{
}

GLuint DebugPrimitiveSphere::GetVBOHandle() const
{
	return Renderer::GetInstance().GetDebugSphereVBOHandle();
}

unsigned int DebugPrimitiveSphere::GetNumVertices() const
{
	return Renderer::GetInstance().GetDebugSphereNumVertices();
}

DebugPrimitiveBox::DebugPrimitiveBox(const MathLib::matrix4x4& transform, const MathLib::vector4& color) :
	DebugPrimitive(transform, color)
{
	Init(MathLib::vector4(-1.0f, -1.0f, -1.0f, 1.0f), 
		 MathLib::vector4(1.0f, 1.0f, 1.0f, 1.0f));
}

DebugPrimitiveBox::DebugPrimitiveBox(const MathLib::vector4& min, const MathLib::vector4& max, const MathLib::matrix4x4& transform, const MathLib::vector4& color) :
	DebugPrimitiveBox(transform, color)
{
	Init(min, max);
}

DebugPrimitiveBox::~DebugPrimitiveBox()
{
}

GLuint DebugPrimitiveBox::GetVBOHandle() const
{
	GLuint vboHandle = Renderer::GetInstance().GetDebugCubeVBOHandle();

	// Update the buffers contents.
	{
		glBindBuffer(GL_ARRAY_BUFFER, vboHandle);

		const unsigned int numLines = 12;
		const unsigned int numVerts = 24;
		const unsigned int arrayLength = numVerts * 3;

		float array[arrayLength];

		unsigned int offset = 0;

		float minX = minExtents.extractX();
		float minY = minExtents.extractY();
		float minZ = minExtents.extractZ();

		float maxX = maxExtents.extractX();
		float maxY = maxExtents.extractY();
		float maxZ = maxExtents.extractZ();

		// Front face
		{
			array[offset++]	= minX;
			array[offset++]	= maxY;
			array[offset++]	= maxZ;

			array[offset++]	= maxX;
			array[offset++]	= maxY;
			array[offset++]	= maxZ;

			array[offset++]	= maxX;
			array[offset++]	= maxY;
			array[offset++]	= maxZ;

			array[offset++]	= maxX;
			array[offset++]	= minY;
			array[offset++]	= maxZ;

			array[offset++]	= maxX;
			array[offset++]	= minY;
			array[offset++]	= maxZ;

			array[offset++]	= minX;
			array[offset++]	= minY;
			array[offset++]	= maxZ;

			array[offset++]	= minX;
			array[offset++]	= minY;
			array[offset++]	= maxZ;

			array[offset++]	= minX;
			array[offset++]	= maxY;
			array[offset++]	= maxZ;
		}

		// Back face
		{
			array[offset++]	= minX;
			array[offset++]	= maxY;
			array[offset++]	= minZ;

			array[offset++]	= maxX;
			array[offset++]	= maxY;
			array[offset++]	= minZ;

			array[offset++]	= maxX;
			array[offset++]	= maxY;
			array[offset++]	= minZ;

			array[offset++]	= maxX;
			array[offset++]	= minY;
			array[offset++]	= minZ;

			array[offset++]	= maxX;
			array[offset++]	= minY;
			array[offset++]	= minZ;

			array[offset++]	= minX;
			array[offset++]	= minY;
			array[offset++]	= minZ;

			array[offset++]	= minX;
			array[offset++]	= minY;
			array[offset++]	= minZ;

			array[offset++]	= minX;
			array[offset++]	= maxY;
			array[offset++]	= minZ;
		}

		// Connecting lines
		{
			array[offset++]	= minX;
			array[offset++]	= maxY;
			array[offset++]	= maxZ;

			array[offset++]	= minX;
			array[offset++]	= maxY;
			array[offset++]	= minZ;

			array[offset++]	= maxX;
			array[offset++]	= maxY;
			array[offset++]	= maxZ;

			array[offset++]	= maxX;
			array[offset++]	= maxY;
			array[offset++]	= minZ;

			array[offset++]	= maxX;
			array[offset++]	= minY;
			array[offset++]	= maxZ;

			array[offset++]	= maxX;
			array[offset++]	= minY;
			array[offset++]	= minZ;

			array[offset++]	= minX;
			array[offset++]	= minY;
			array[offset++]	= maxZ;

			array[offset++]	= minX;
			array[offset++]	= minY;
			array[offset++]	= minZ;
		}
	
		glBufferSubData(GL_ARRAY_BUFFER, 0, arrayLength * sizeof(float), array);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	return vboHandle;
}

unsigned int DebugPrimitiveBox::GetNumVertices() const
{
	return Renderer::GetInstance().GetDebugCubeNumVertices();
}

void DebugPrimitiveBox::Init(const MathLib::vector4& min, const MathLib::vector4& max)
{
	MathLib::vector4_copy(minExtents, min);
	MathLib::vector4_copy(maxExtents, max);
}
