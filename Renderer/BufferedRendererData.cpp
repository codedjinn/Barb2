#include "BufferedRendererData.h"

namespace RendererLib
{

BufferedRendererData::BufferedRendererData()
{
}

void BufferedRendererData::CopyFrom(const BufferedRendererData& data)
{
	mainCamera = data.mainCamera;

	renderableSet.CopyFrom(data.renderableSet);
}

void BufferedRendererData::Reset()
{
	renderableSet.Reset();
}

}