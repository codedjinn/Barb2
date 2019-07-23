#ifndef SUBRENDERERDIRECTIONALLIGHT_H_INCLUDED
#define SUBRENDERERDIRECTIONALLIGHT_H_INCLUDED

#include "SubRenderer.h"
#include <vector>

namespace RendererLib
{

class DirectionalLight;
class Renderer;
class ShaderProgram;

class SubRendererDirectionalLight :
	public SubRenderer
{
	public:

		SubRendererDirectionalLight(Renderer& renderer, std::vector<DirectionalLight*>& set);

		virtual void SetContextState();

		virtual void Render();

		virtual void RestoreContextState();

	protected:

		std::vector<DirectionalLight*>& lightSet;

		ShaderProgram* directionalLightShader;
};

}

#endif // SUBRENDERERDIRECTIONALLIGHT_H_INCLUDED
