#ifndef RENDERTOKENSTATICMESH_H_INCLUDED
#define RENDERTOKENSTATICMESH_H_INCLUDED

#include "RendererLibCommon.h"
#include "RenderToken.h"
#include "StaticMesh.h"

namespace RendererLib
{

class RenderTokenStaticMesh :
	public RenderToken
{
	public:

		const StaticMeshInstance* mesh;

		explicit RenderTokenStaticMesh(StaticMeshInstance const * const meshInstance) :
			RenderToken()
		{
			mesh = meshInstance;
		}

		explicit RenderTokenStaticMesh(const RenderTokenStaticMesh& source) :
			RenderToken()
		{
			mesh = source.mesh;
		}

		virtual ~RenderTokenStaticMesh(){}
};

}

#endif // RENDERTOKENSTATICMESH_H_INCLUDED
