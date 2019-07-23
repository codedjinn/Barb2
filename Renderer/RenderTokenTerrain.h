#ifndef RENDERTOKENTERRAIN_H_INCLUDED
#define RENDERTOKENTERRAIN_H_INCLUDED

#include "RendererLibCommon.h"
#include "RenderToken.h"
#include <cassert>

namespace RendererLib
{

class RendererTerrain;

class RenderTokenTerrain :
	public RenderToken
{
	public:

		explicit RenderTokenTerrain(RendererTerrain* terrain)
		{
			RenderTokenTerrain::terrain = terrain;
			assert(NULL != RenderTokenTerrain::terrain);
		}

		RendererTerrain* terrain;
};

}

#endif // RENDERTOKENTERRAIN_H_INCLUDED
