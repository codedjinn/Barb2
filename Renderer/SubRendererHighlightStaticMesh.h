#ifndef SUBRENDERERHIGHLIGHTSTATICMESH_H_INCLUDED
#define SUBRENDERERHIGHLIGHTSTATICMESH_H_INCLUDED

#include "SubRenderer.h"
#include "RenderToken.h"
#include "ShaderProgram.h"
#include <vector>

namespace RendererLib
{

class SubRendererHighlightStaticMesh :
	public SubRenderer
{
	public:

		SubRendererHighlightStaticMesh(Renderer& renderer, const std::vector<RenderToken*>& set);

		virtual void SetContextState();

		virtual void Render();

		virtual void RestoreContextState();

	protected:

		const std::vector<RenderToken*>& renderableSet;

		float matrixArray[16];

		ShaderProgram* highlightMeshShader;
};

}

#endif // SUBRENDERERHIGHLIGHTSTATICMESH_H_INCLUDED
