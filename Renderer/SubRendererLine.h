#ifndef SUBRENDERERLINE_H_INCLUDED
#define SUBRENDERERLINE_H_INCLUDED

#include "SubRenderer.h"
#include "RenderToken.h"
#include "ShaderProgram.h"
#include <vector>

namespace RendererLib
{

class SubRendererLine :
	public SubRenderer
{
	public:

		SubRendererLine(Renderer& renderer, const std::vector<RenderToken*>& set, bool depthTest);

		virtual void SetContextState();
		
		virtual void Render();

		virtual void RestoreContextState();

	private:

		bool performDepthTest;

		const std::vector<RenderToken*>& renderableSet;

		float matrixArray[16];

		ShaderProgram* lineShader;
};

}

#endif // SUBRENDERERLINE_H_INCLUDED