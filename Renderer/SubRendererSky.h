#ifndef SUBRENDERERSKY_H_INCLUDED
#define SUBRENDERERSKY_H_INCLUDED

#include "SubRenderer.h"
#include "RenderToken.h"
#include "ShaderProgram.h"
#include <vector>

namespace RendererLib
{

class SubRendererSky :
	public SubRenderer
{
	public:

		SubRendererSky(Renderer& renderer, const std::vector<RenderToken*>& set);

		virtual void SetContextState();

		virtual void Render();

		virtual void RestoreContextState();

	protected:

		const std::vector<RenderToken*>& renderableSet;

		float matrixArray[16];

		ShaderProgram* skyShader;

		GLint attributeTexCoord0Index;
		GLint attributeNormalIndex;
};

}

#endif // SUBRENDERERSKY_H_INCLUDED
