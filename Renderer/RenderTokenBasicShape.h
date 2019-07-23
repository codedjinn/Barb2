#ifndef RENDERTOKENBASICSHAPE_H_INCLUDED
#define RENDERTOKENBASICSHAPE_H_INCLUDED

#include "RenderToken.h"
#include <MathLib.h>
#include <vector>
#include <string>

namespace RendererLib
{

class RenderTokenBasicShape :
	public RenderToken
{
	public:

		RenderTokenBasicShape(int shapeType,
							  const MathLib::vector4& color,
							  const MathLib::vector4& position,
							  const MathLib::quaternion& orientation,
							  const MathLib::vector4& scale, 
							  std::vector<std::string> faceTextures)
		{
			this->shapeType = shapeType;
			MathLib::vector4_copy(this->color, color);
			MathLib::vector4_copy(this->position, position);
			MathLib::quaternion_copy(this->orientation, orientation);
			MathLib::vector4_copy(this->scale, scale);
			this->faceTextures = faceTextures;
		}

		virtual ~RenderTokenBasicShape() {}

		int shapeType;
		MathLib::vector4 color;
		MathLib::vector4 position;
		MathLib::quaternion orientation;
		MathLib::vector4 scale;

		std::vector<std::string> faceTextures;
};

}

#endif // RENDERTOKENBASICSHAPE_H_INCLUDED
