#ifndef SUBRENDERERTERRAINNAIVE_H_INCLUDED
#define SUBRENDERERTERRAINNAIVE_H_INCLUDED

#include "SubRenderer.h"
#include "RenderToken.h"
#include "ShaderProgram.h"
#include <vector>

namespace RendererLib
{

class SubRendererTerrainNaive :
	public SubRenderer
{
	public:

		SubRendererTerrainNaive(Renderer& renderer, const std::vector<RenderToken*>& set);

		virtual void SetContextState();

		virtual void Render();

		virtual void RestoreContextState();

	protected:

		const std::vector<RenderToken*> renderableSet;

		float matrixArray[16];

		ShaderProgram* terrainShader;

		GLint attributeTexCoord0Index;
		GLint attributeNormalIndex;
};

}


#endif // SUBRENDERERTERRAINNAIVE_H_INCLUDED
