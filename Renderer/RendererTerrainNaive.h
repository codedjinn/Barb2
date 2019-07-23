#ifndef RENDERERTERRAINNAIVE_H_INCLUDED
#define RENDERERTERRAINNAIVE_H_INCLUDED

#include "RendererLibCommon.h"
#include "RendererTerrain.h"
#include <Terrain.h>
#include <string>

namespace RendererLib
{

class RenderToken;

class RendererTerrainNaive :
	public RendererTerrain
{
	public:

		RendererTerrainNaive();

		virtual void Initialize(float const * const terrainData, unsigned int gridSize, std::string texture);

		virtual std::vector<RenderToken*> GetRenderTokens(const CameraLib::Camera& camera);

		RENDERER_INLINE GLuint GetDataVBOHandle()
		{
			return dataVBOHandle;
		}

		RENDERER_INLINE GLuint GetIndexVBOHandle()
		{
			return indexVBOHandle;
		}

		RENDERER_INLINE std::string GetTerrainTexture()
		{
			return terrainTexture;
		}

		RENDERER_INLINE unsigned int GetGridSize()
		{
			return terrainGridSize;
		}

		RENDERER_INLINE unsigned int GetNumVertices()
		{
			return (terrainGridSize + 1) * (terrainGridSize + 1);
		}

		RENDERER_INLINE unsigned int GetNumIndices()
		{
			return numIndices;
		}

	protected:

		unsigned int terrainGridSize;

		unsigned int numIndices;

		GLuint dataVBOHandle;
		GLuint indexVBOHandle;

		std::string terrainTexture;
};

}


#endif // RENDERERTERRAINNAIVE_H_INCLUDED
