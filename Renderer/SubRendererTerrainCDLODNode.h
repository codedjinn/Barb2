#ifndef SUBRENDERERTERRAINCDLODNODE_H_INCLUDED
#define SUBRENDERERTERRAINCDLODNODE_H_INCLUDED

#include "SubRenderer.h"
#include "RenderToken.h"
#include "ShaderProgram.h"
#include <vector>

namespace RendererLib
{

class SubRendererTerrainCDLODNode :
	public SubRenderer
{
	public:

		SubRendererTerrainCDLODNode(Renderer& renderer, const std::vector<RenderToken*>& set);

		virtual void SetContextState();
		virtual void Render();
		virtual void RestoreContextState();

	protected:

		const std::vector<RenderToken*>& renderableSet;

		float matrixArray[16];

		GLint attributeTexCoord0Index;

		ShaderProgram* shader;
};

}

#endif // SUBRENDERERTERRAINCDLODNODE_H_INCLUDED
