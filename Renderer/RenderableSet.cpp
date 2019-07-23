#include "RenderableSet.h"
#include "RenderTokenStaticMesh.h"
#include "RenderTokenBasicShape.h"
#include "RenderTokenDebugPrimitive.h"
#include "RenderTokenTerrainCDLODNode.h"
#include "RenderTokenGizmo.h"
#include "RenderTokenSky.h"
#include "RenderTokenHighlightStaticMesh.h"

using namespace RendererLib;

RenderableSet::RenderableSet()
{
}

void RenderableSet::CopyFrom(const RenderableSet& source)
{
	Reset();

	for (unsigned int i = 0; i < static_cast<unsigned int>(source.staticMeshList.size()); i++)
	{
		staticMeshList.push_back(new RenderTokenStaticMesh(*reinterpret_cast<RenderTokenStaticMesh*>(source.staticMeshList[i])));
	}

	for (unsigned int i = 0; i < static_cast<unsigned int>(source.basicShapeList.size()); i++)
	{
		basicShapeList.push_back(new RenderTokenBasicShape(*reinterpret_cast<RenderTokenBasicShape*>(source.basicShapeList[i])));
	}

	for (unsigned int i = 0; i < static_cast<unsigned int>(source.debugPrimitiveList.size()); i++)
	{
		debugPrimitiveList.push_back(new RenderTokenDebugPrimitive(*reinterpret_cast<RenderTokenDebugPrimitive*>(source.debugPrimitiveList[i])));
	}

	for (unsigned int i = 0; i < static_cast<unsigned int>(source.terrainList.size()); i++)
	{
		terrainList.push_back(new RenderTokenTerrainCDLODNode(*reinterpret_cast<RenderTokenTerrainCDLODNode*>(source.terrainList[i])));
	}

	for (unsigned int i = 0; i < static_cast<unsigned int>(source.gizmoList.size()); i++)
	{
		gizmoList.push_back(new RenderTokenGizmo(*reinterpret_cast<RenderTokenGizmo*>(source.gizmoList[i])));
	}

	for (unsigned int i = 0; i < static_cast<unsigned int>(source.skyList.size()); i++)
	{
		skyList.push_back(new RenderTokenSky(*reinterpret_cast<RenderTokenSky*>(source.skyList[i])));
	}

	for (unsigned int i = 0; i < static_cast<unsigned int>(source.highlightStaticMeshList.size()); i++)
	{
		highlightStaticMeshList.push_back(new RenderTokenHighlightStaticMesh(*reinterpret_cast<RenderTokenHighlightStaticMesh*>(source.highlightStaticMeshList[i])));
	}
}

void RenderableSet::AddStaticMeshToken(RenderToken* token)
{
	staticMeshList.push_back(token);
}

void RenderableSet::AddBasicShapeToken(RenderToken* token)
{
	basicShapeList.push_back(token);
}

void RenderableSet::AddDebugPrimitiveToken(RenderToken* token)
{
	debugPrimitiveList.push_back(token);
}

void RenderableSet::AddTerrainToken(RenderToken* token)
{
	terrainList.push_back(token);
}

void RenderableSet::AddGizmoToken(RenderToken* token)
{
	gizmoList.push_back(token);
}

void RenderableSet::AddSkyToken(RenderToken* token)
{
	skyList.push_back(token);
}

void RenderableSet::AddHighlightStaticMeshToken(RenderToken* token)
{
	highlightStaticMeshList.push_back(token);
}

void RenderableSet::Reset()
{
	for (unsigned int i = 0; i < static_cast<unsigned int>(debugPrimitiveList.size()); i++)
	{
		delete debugPrimitiveList[i];
	}

	for (unsigned int i = 0; i < static_cast<unsigned int>(staticMeshList.size()); i++)
	{
		delete staticMeshList[i];
	}

	for (unsigned int i = 0; i < static_cast<unsigned int>(basicShapeList.size()); i++)
	{
		delete basicShapeList[i];
	}

	for (unsigned int i = 0; i < static_cast<unsigned int>(terrainList.size()); i++)
	{
		delete terrainList[i];
	}

	for (unsigned int i = 0; i < static_cast<unsigned int>(gizmoList.size()); i++)
	{
		delete gizmoList[i];
	}

	for (unsigned int i = 0; i < static_cast<unsigned int>(skyList.size()); i++)
	{
		delete skyList[i];
	}

	for (unsigned int i = 0; i < static_cast<unsigned int>(highlightStaticMeshList.size()); i++)
	{
		delete highlightStaticMeshList[i];
	}

	staticMeshList.clear();
	basicShapeList.clear();
	debugPrimitiveList.clear();
	terrainList.clear();
	gizmoList.clear();
	skyList.clear();
	highlightStaticMeshList.clear();
}
