#include "DirectionalLight.h"

using namespace RendererLib;


DirectionalLight::DirectionalLight(const MathLib::vector4& color, const MathLib::vector4& direction)
{
	Light::SetColor(color);
	MathLib::vector4_copy(DirectionalLight::direction, direction);
	MathLib::vector4_normalize(DirectionalLight::direction);
}

const MathLib::vector4& DirectionalLight::GetDirection() const
{
	return direction;
}
