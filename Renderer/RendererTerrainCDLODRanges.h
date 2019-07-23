#ifndef RENDERERTERRAINCDLODDRANGES_H_INCLUDED
#define RENDERERTERRAINCDLODDRANGES_H_INCLUDED

#include <Camera.h>
#include <MathLib.h>
#include "RendererTerrainCommon.h"

namespace RendererLib
{

class RendererTerrainCDLODRanges
{
	struct CDLODRange
	{
		/// Distance at which this LOD range becomes active.
		float distance;

		/// Distance at which this LOD range starts morphing into the next range.
		float morphAreaStart;

		CDLODRange()
		{
		}

		CDLODRange(float distance, float morphAreaStart)
		{
			this->distance = distance;
			this->morphAreaStart = morphAreaStart;
		}
	};

	public:

		RendererTerrainCDLODRanges();
		~RendererTerrainCDLODRanges();

		CDLODRange* ranges;
		unsigned int rangesLength;

		void ComputeRanges(float viewingDistance, unsigned int quadtreeDepth);

	protected:

		void FreeResources();
};

}

#endif // RENDERERTERRAINCDLODDRANGES_H_INCLUDED
