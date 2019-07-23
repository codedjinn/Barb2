#ifndef RENDERTOKENTERRAINCDLODNODE_H_INCLUDED
#define RENDERTOKENTERRAINCDLODNODE_H_INCLUDED

#include "RendererLibCommon.h"
#include "RenderToken.h"
#include "RendererTerrainCDLOD.h"
#include "RendererTerrainCDLODNode.h"

namespace RendererLib
{

class RenderTokenTerrainCDLODNode :
	public RenderToken
{
	public:

		const RendererTerrainCDLOD& terrain;

		MathLib::vector4 position;
		unsigned int scale;

		explicit RenderTokenTerrainCDLODNode(const RendererTerrainCDLOD& terrainLandscape, const RendererTerrainCDLODNode& node) :
			RenderToken(),
			terrain(terrainLandscape)
		{
			MathLib::vector4_copy(position, node.position);

			scale = node.scale;
		}

		explicit RenderTokenTerrainCDLODNode(const RenderTokenTerrainCDLODNode& source) :
			RenderToken(),
			terrain(source.terrain)
		{
			MathLib::vector4_copy(position, source.position);

			scale = source.scale;
		}

		virtual ~RenderTokenTerrainCDLODNode(){}
};

}

#endif // RENDERTOKENTERRAINCDLODNODE_H_INCLUDED
