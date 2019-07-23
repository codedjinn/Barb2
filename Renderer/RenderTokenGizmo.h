#ifndef RENDERTOKENGIZMO_H_INCLUDED
#define RENDERTOKENGIZMO_H_INCLUDED

#include "RenderToken.h"
#include <MathLib.h>

namespace RendererLib
{

class RenderTokenGizmo
	: public RenderToken
{
	public:

		RenderTokenGizmo(int gizmoType,
						 const MathLib::vector4& color, 
						 const MathLib::vector4& position, 
						 const MathLib::quaternion& orientation, 
						 float scale)
		{
			this->gizmoType = gizmoType;
			MathLib::vector4_copy(this->color, color);
			MathLib::vector4_copy(this->position, position);
			MathLib::quaternion_copy(this->orientation, orientation);
			this->scale = scale;
		}

		virtual ~RenderTokenGizmo() {}

		int gizmoType;
		MathLib::vector4 color;
		MathLib::vector4 position;
		MathLib::quaternion orientation;
		float scale;
};

}

#endif // RENDERTOKENGIZMO_H_INCLUDED