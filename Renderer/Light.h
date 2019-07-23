#ifndef LIGHT_H_INCLUDED
#define LIGHT_H_INCLUDED

#include <MathLib.h>

namespace RendererLib
{

/**
* Light:
* Base type for all lights.
*/
class Light
{
	public:

		const MathLib::vector4& GetColor() const;

		virtual void SetColor(const MathLib::vector4& color);
		virtual void SetColor(float r, float g, float b);

		virtual void Update(float timeElapsed);

	protected:

		MathLib::vector4 color;

		Light();
};

}

#endif // LIGHT_H_INCLUDED
