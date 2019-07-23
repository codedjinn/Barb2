#ifndef SUBRENDERERTERRAINCDLODNODESHADOW_H_INCLUDED
#define SUBRENDERERTERRAINCDLODNODESHADOW_H_INCLUDED

#include "SubRenderer.h"
#include "RenderToken.h"
#include "ShaderProgram.h"
#include <vector>

namespace RendererLib
{

class DirectionalLightShadowBasis;

class SubRendererTerrainCDLODNodeShadow :
	public SubRenderer
{
	public:

		SubRendererTerrainCDLODNodeShadow(Renderer& renderer, const std::vector<RenderToken*>& set, const DirectionalLightShadowBasis& directionalLightBasis);

		virtual void SetContextState();
		virtual void Render();
		virtual void RestoreContextState();

	protected:

		const std::vector<RenderToken*>& renderableSet;

		float matrixArray[16];

		GLint attributeTexCoord0Index;

		const DirectionalLightShadowBasis& lightBasis;

		ShaderProgram* shader;
};

}

#endif // SUBRENDERERTERRAINCDLODNODESHADOW_H_INCLUDED
