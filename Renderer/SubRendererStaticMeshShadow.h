#ifndef SUBRENDERERSTATICMESHSHADOW_H_INCLUDED
#define SUBRENDERERSTATICMESHSHADOW_H_INCLUDED

#include "SubRenderer.h"
#include "RenderToken.h"
#include "ShaderProgram.h"
#include <vector>

namespace RendererLib
{

class DirectionalLightShadowBasis;

class SubRendererStaticMeshShadow :
	public SubRenderer
{
	public:

		SubRendererStaticMeshShadow(Renderer& renderer, const std::vector<RenderToken*>& set, const DirectionalLightShadowBasis& lightShadowBasis);

		virtual void SetContextState();

		virtual void Render();

		virtual void RestoreContextState();

	protected:

		const std::vector<RenderToken*>& renderableSet;
		
		const DirectionalLightShadowBasis& shadowBasis;

		float matrixArray[16];

		ShaderProgram* shader;

		GLint attributeNormalIndex;
};

}

#endif // SUBRENDERERSTATICMESHSHADOW
