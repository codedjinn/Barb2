#ifndef SUBRENDERERBOUNDINGVOLUME_H_INCLUDED
#define SUBRENDERERBOUNDINGVOLUME_H_INCLUDED

#include "SubRenderer.h"
#include "RenderToken.h"
#include "ShaderProgram.h"
#include <vector>

namespace RendererLib
{

class SubRendererBoundingVolume :
	public SubRenderer
{
	public:

		SubRendererBoundingVolume(Renderer& renderer, const std::vector<RenderToken*>& set);

		virtual void SetContextState();

		virtual void Render();

		virtual void RestoreContextState();

	protected:

		const std::vector<RenderToken*>& renderableSet;

		float matrixArray[16];

		ShaderProgram* shader;
};

}

#endif // SUBRENDERERBOUNDINGVOLUME_H_INCLUDED
