#ifndef DEBUGPRIMITIVE_H_INCLUDED
#define DEBUGPRIMITIVE_H_INCLUDED

#include "RendererLibCommon.h"
#include <MathLib.h>

namespace RendererLib
{

class DebugPrimitive
{
	public:

		MathLib::matrix4x4 transform;

		MathLib::vector4 color;

		virtual ~DebugPrimitive()
		{
		}

		virtual GLuint GetVBOHandle() const = 0;
		virtual unsigned int GetNumVertices() const = 0;

	protected:

		DebugPrimitive(const MathLib::matrix4x4& transform, const MathLib::vector4& color);
};

class DebugPrimitiveSphere :
	public DebugPrimitive
{
	public:

		DebugPrimitiveSphere(const MathLib::matrix4x4& transform, const MathLib::vector4& color);

		virtual ~DebugPrimitiveSphere();

		virtual GLuint GetVBOHandle() const;

		virtual unsigned int GetNumVertices() const;
};

class DebugPrimitiveBox :
	public DebugPrimitive
{
	public:

		DebugPrimitiveBox(const MathLib::matrix4x4& transform, const MathLib::vector4& color);
		DebugPrimitiveBox(const MathLib::vector4& min, const MathLib::vector4& max, const MathLib::matrix4x4& transform, const MathLib::vector4& color);

		virtual ~DebugPrimitiveBox();

		virtual GLuint GetVBOHandle() const;

		virtual unsigned int GetNumVertices() const;

	protected:
		
		MathLib::vector4 minExtents;
		MathLib::vector4 maxExtents;

		void Init(const MathLib::vector4& min, const MathLib::vector4& max);
};

}

#endif // DEBUGPRIMITIVE_H_INCLUDED
