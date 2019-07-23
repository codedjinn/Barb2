#include "Light.h"

using namespace RendererLib;

Light::Light()
{
}

const MathLib::vector4& Light::GetColor() const
{
	return color;
}

void Light::SetColor(const MathLib::vector4& color)
{
	MathLib::vector4_copy(Light::color, color);
}

void Light::SetColor(float r, float g, float b)
{
	color.setXYZ(r, g, b);
}

void Light::Update(float timeElapsed)
{
}
