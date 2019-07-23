#ifndef RENDERABLESET_H_INCLUDED
#define RENDERABLESET_H_INCLUDED

#include "RendererLibCommon.h"
#include "RenderToken.h"
#include <vector>

namespace RendererLib
{

/**
* RenderableSet:
* Contains lists of RenderTokens. RenderTokens are transient data structures that encapsulate all of the
* information required by the renderer to render an instance of a specific element. These lists are built up
* by the simulation and then passed onto the renderer.
*/
class RenderableSet
{
	public:

		RenderableSet();

		void CopyFrom(const RenderableSet& source);

		void AddStaticMeshToken(RenderToken* token);
		void AddBasicShapeToken(RenderToken* token);
		void AddDebugPrimitiveToken(RenderToken* token);
		void AddTerrainToken(RenderToken* token);
		void AddGizmoToken(RenderToken* token);
		void AddSkyToken(RenderToken* token);
		void AddHighlightStaticMeshToken(RenderToken* token);

		RENDERER_INLINE const std::vector<RenderToken*>& GetStaticMeshTokens() { return staticMeshList; }
		RENDERER_INLINE const std::vector<RenderToken*>& GetBasicShapeTokens() { return basicShapeList; }
		RENDERER_INLINE const std::vector<RenderToken*>& GetDebugPrimitiveTokens() { return debugPrimitiveList; }
		RENDERER_INLINE const std::vector<RenderToken*>& GetTerrainTokens() { return terrainList; }
		RENDERER_INLINE const std::vector<RenderToken*>& GetGizmoTokens() { return gizmoList; }
		RENDERER_INLINE const std::vector<RenderToken*>& GetSkyTokens() { return skyList; }
		RENDERER_INLINE const std::vector<RenderToken*>& GetHighlightStaticMeshTokens() { return highlightStaticMeshList; }

		void Reset();

	protected:

		std::vector<RenderToken*> staticMeshList;
		std::vector<RenderToken*> basicShapeList;
		std::vector<RenderToken*> debugPrimitiveList;
		std::vector<RenderToken*> terrainList;
		std::vector<RenderToken*> gizmoList;
		std::vector<RenderToken*> skyList;
		std::vector<RenderToken*> highlightStaticMeshList;
};

}

#endif // RENDERABLESET_H_INCLUDED
