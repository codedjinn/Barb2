#ifndef BUFFEREDRENDERERDATA_H_INCLUDED
#define BUFFEREDRENDERERDATA_H_INCLUDED

#include "RendererLibCommon.h"
#include "RenderableSet.h"
#include <Camera.h>

namespace RendererLib
{

class BufferedRendererData
{
	public:

		CameraLib::Camera mainCamera;

		RenderableSet renderableSet;

		BufferedRendererData();

		void CopyFrom(const BufferedRendererData& data);
		void Reset();

};

}

#endif // BUFFEREDRENDERERDATA_H_INCLUDED