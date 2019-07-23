#ifndef RENDERTOKENDEBUGPRIMITIVE_H_INCLUDED
#define RENDERTOKENDEBUGPRIMITIVE_H_INCLUDED

#include "RendererLibCommon.h"
#include "RenderToken.h"
#include "DebugPrimitive.h"

namespace RendererLib
{

class RenderTokenDebugPrimitive :
	public RenderToken
{
	public:

		const DebugPrimitive* primitive;

		explicit RenderTokenDebugPrimitive(DebugPrimitive const * const debugPrimitive) :
			RenderToken()
		{
			primitive = debugPrimitive;
		}

		explicit RenderTokenDebugPrimitive(const RenderTokenDebugPrimitive& source) :
			RenderToken()
		{
			primitive = source.primitive;
		}

		virtual ~RenderTokenDebugPrimitive(){}
};

}
#endif // RENDERTOKENDEBUGPRIMITIVE_H_INCLUDED
