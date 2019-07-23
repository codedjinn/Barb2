#ifndef SUBRENDERERSTATICMESH_H_INCLUDED
#define SUBRENDERERSTATICMESH_H_INCLUDED

#include "SubRenderer.h"
#include "RenderToken.h"
#include "ShaderProgram.h"
#include <vector>

namespace RendererLib
{

class SubRendererStaticMesh :
	public SubRenderer
{
	public:

		SubRendererStaticMesh(Renderer& renderer, const std::vector<RenderToken*>& set);

		virtual void SetContextState();

		virtual void Render();

		virtual void RestoreContextState();

	protected:

		const std::vector<RenderToken*>& renderableSet;

		float matrixArray[16];

		ShaderProgram* staticMeshShader;

		GLint attributeTexCoord0Index;
		GLint attributeNormalIndex;
};

}

#endif // SUBRENDERERSTATICMESH_H_INCLUDED
