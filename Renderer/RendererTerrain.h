#ifndef RENDERERTERRAIN_H_INCLUDED
#define RENDERERTERRAIN_H_INCLUDED

#include "RendererLibCommon.h"
#include <string>
#include <vector>
#include <Camera.h>

namespace RendererLib
{

class RenderToken;

class RendererTerrain
{
	public:

		virtual std::vector<RenderToken*> GetRenderTokens(const CameraLib::Camera& camera) = 0;

		static float* ReadHeightMapValuesFromFile(const std::string& fileName, unsigned int landscapeSize, float scaleFactor);
};

}

#endif // RENDERERTERRAIN_H_INCLUDED
