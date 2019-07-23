#ifndef RENDERTOKENHIGHLIGHTSTATICMESH_H_INCLUDED
#define RENDERTOKENHIGHLIGHTSTATICMESH_H_INCLUDED

#include "RenderToken.h"
#include <MathLib.h>
#include "StaticMesh.h"

namespace RendererLib
{

class RenderTokenHighlightStaticMesh :
	public RenderToken
{
	public:

		const StaticMeshInstance* mesh;

		MathLib::vector4 color;

		explicit RenderTokenHighlightStaticMesh(StaticMeshInstance const * const meshInstance, const MathLib::vector4& color) :
			RenderToken()
		{
			mesh = meshInstance;

			MathLib::vector4_copy(this->color, color);
		}

		explicit RenderTokenHighlightStaticMesh(const RenderTokenHighlightStaticMesh& source) : 
			RenderToken()
		{
			mesh = source.mesh;

			MathLib::vector4_copy(color, source.color);
		}

		virtual ~RenderTokenHighlightStaticMesh()
		{
		}
};

}

#endif // RENDERTOKENHIGHLIGHTSTATICMESH_H_INCLUDED