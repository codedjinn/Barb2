#ifndef DIRECTIONALLIGHT_H_INCLUDED
#define DIRECTIONALLIGHT_H_INCLUDED

#include "Light.h"

namespace RendererLib
{

class DirectionalLight :
	public Light
{
	public:

		DirectionalLight(const MathLib::vector4& color, const MathLib::vector4& direction);

		const MathLib::vector4& GetDirection() const;

	protected:

		MathLib::vector4 direction;
};

}

#endif // DIRECTIONALLIGHT_H_INCLUDED
