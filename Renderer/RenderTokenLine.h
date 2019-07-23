#ifndef RENDERTOKENLINE_H_INCLUDED
#define RENDERTOKENLINE_H_INCLUDED

#include "RenderToken.h"
#include <MathLib.h>

namespace RendererLib
{

	class RenderTokenLine
		: public RenderToken
	{
	public:

		RenderTokenLine(const MathLib::vector4& p0, 
						const MathLib::vector4& p1, 
						const MathLib::vector4& color, 
						float lineWidth)
		{
			MathLib::vector4_copy(this->p0, p0);
			MathLib::vector4_copy(this->p1, p1);
			MathLib::vector4_copy(this->color, color);
			this->lineWidth = lineWidth;
		}

		virtual ~RenderTokenLine() {}

		MathLib::vector4 color;
		MathLib::vector4 p0;
		MathLib::vector4 p1;

		float lineWidth;
	};

}

#endif // RENDERTOKENLINE_H_INCLUDED