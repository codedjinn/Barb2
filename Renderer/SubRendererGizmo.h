#ifndef SUBRENDERERGIZMO_H_INCLUDED
#define SUBRENDERERGIZMO_H_INCLUDED

#include "SubRenderer.h"
#include "RenderToken.h"
#include "ShaderProgram.h"
#include <vector>

namespace RendererLib
{

class SubRendererGizmo :
	public SubRenderer
{
	public:

		SubRendererGizmo(Renderer& renderer, const std::vector<RenderToken*>& set);

		virtual void SetContextState();

		virtual void Render();

		virtual void RestoreContextState();

	private:

		const std::vector<RenderToken*>& renderableSet;

		float matrixArray[16];

		ShaderProgram* gizmoShader;
};

}

#endif // SUBRENDERERGIZMO_H_INCLUDED