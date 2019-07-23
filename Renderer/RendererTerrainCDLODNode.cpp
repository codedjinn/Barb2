#include "RendererTerrainCDLODNode.h"
#include "RendererTerrainCDLOD.h"
#include <Terrain.h>
#include <Geometry.h>
#include <cmath>
#include <cassert>
#include <CommonDefines.h>

namespace RendererLib
{

RendererTerrainCDLODNode::RendererTerrainCDLODNode()
{
	for (unsigned int i = 0; i < 4; i++)
	{
		nodes[i] = NULL;
	}
}

RendererTerrainCDLODNode::~RendererTerrainCDLODNode()
{
	FreeResources();
}

void RendererTerrainCDLODNode::GenerateQuadtree()
{
	if (scale > 1)
	{
		unsigned int newLevelScale = scale >> 1;
		float offset = static_cast<float>((TERRAIN_PATCH_SIZE - 1) * newLevelScale);

		// Top left node.
		if (NULL != nodes[TERRAIN_CDLOD_NODE_TL])
		{
			delete nodes[TERRAIN_CDLOD_NODE_TL];
		}
		nodes[TERRAIN_CDLOD_NODE_TL] = new RendererTerrainCDLODNode;
		nodes[TERRAIN_CDLOD_NODE_TL]->position.setXYZ(position.extractX(), 0.0f, position.extractZ());
		nodes[TERRAIN_CDLOD_NODE_TL]->scale = newLevelScale;
		nodes[TERRAIN_CDLOD_NODE_TL]->GenerateQuadtree();
		GenerateBoundingVolumeInformation(nodes[TERRAIN_CDLOD_NODE_TL]);

		// Top right node.
		if (NULL != nodes[TERRAIN_CDLOD_NODE_TR])
		{
			delete nodes[TERRAIN_CDLOD_NODE_TR];
		}
		nodes[TERRAIN_CDLOD_NODE_TR] = new RendererTerrainCDLODNode;
		nodes[TERRAIN_CDLOD_NODE_TR]->position.setXYZ(position.extractX() + offset, 0.0f, position.extractZ());
		nodes[TERRAIN_CDLOD_NODE_TR]->scale = newLevelScale;
		nodes[TERRAIN_CDLOD_NODE_TR]->GenerateQuadtree();
		GenerateBoundingVolumeInformation(nodes[TERRAIN_CDLOD_NODE_TR]);

		// Bottom left node.
		if (NULL != nodes[TERRAIN_CDLOD_NODE_BL])
		{
			delete nodes[TERRAIN_CDLOD_NODE_BL];
		}
		nodes[TERRAIN_CDLOD_NODE_BL] = new RendererTerrainCDLODNode;
		nodes[TERRAIN_CDLOD_NODE_BL]->position.setXYZ(position.extractX(), 0.0f, position.extractZ() + offset);
		nodes[TERRAIN_CDLOD_NODE_BL]->scale = newLevelScale;
		nodes[TERRAIN_CDLOD_NODE_BL]->GenerateQuadtree();
		GenerateBoundingVolumeInformation(nodes[TERRAIN_CDLOD_NODE_BL]);

		// Bottom right node.
		if (NULL != nodes[TERRAIN_CDLOD_NODE_BR])
		{
			delete nodes[TERRAIN_CDLOD_NODE_BR];
		}
		nodes[TERRAIN_CDLOD_NODE_BR] = new RendererTerrainCDLODNode;
		nodes[TERRAIN_CDLOD_NODE_BR]->position.setXYZ(position.extractX() + offset, 0.0f, position.extractZ() + offset);
		nodes[TERRAIN_CDLOD_NODE_BR]->scale = newLevelScale;
		nodes[TERRAIN_CDLOD_NODE_BR]->GenerateQuadtree();
		GenerateBoundingVolumeInformation(nodes[TERRAIN_CDLOD_NODE_BR]);
	}
}

void RendererTerrainCDLODNode::FreeResources()
{
	// Free child resources.
	for (unsigned int i = 0; i < 4; i++)
	{
		if (NULL != nodes[i])
		{
			delete nodes[i];
		}
	}

	// Free own resources.
}

bool RendererTerrainCDLODNode::IsChild() const
{
	return (NULL == nodes[TERRAIN_CDLOD_NODE_BL] &&
			NULL == nodes[TERRAIN_CDLOD_NODE_BR] &&
			NULL == nodes[TERRAIN_CDLOD_NODE_TL] &&
			NULL == nodes[TERRAIN_CDLOD_NODE_TR]);
}

unsigned int RendererTerrainCDLODNode::GetNumQuadtreeLevels(unsigned int terrainSize, unsigned int patchSize)
{
    double landscapeSize = terrainSize - 1;
    double patch = patchSize;

	double numBaseNodes = landscapeSize / patch;
	numBaseNodes = trunc(numBaseNodes);

	return static_cast<unsigned int>(log2(numBaseNodes));
}

MathLib::plane* RendererTerrainCDLODNode::GetPlanes()
{
	return planes;
}

MathLib::vector4 const * const RendererTerrainCDLODNode::GetBoundingBoxVertices() const
{
	return vertices;
}

MathLib::vector4 const * const RendererTerrainCDLODNode::GetBoundingBoxExtents() const
{
	return extents;
}

void RendererTerrainCDLODNode::GenerateBoundingVolumeInformation(RendererTerrainCDLODNode* node)
{
	assert(NULL != node);

	unsigned int nodeHalfScale = node->scale >> 1;
	float halfOffset = static_cast<float>((TERRAIN_PATCH_SIZE - 1) * nodeHalfScale);
	float fullOffset = static_cast<float>((TERRAIN_PATCH_SIZE - 1) * node->scale);

	// Compute planes.
	node->planes[BOX_FACES_FRONT].setNormal(MathLib::vector4(0.0f, 0.0f, -1.0f, 0.0f));
	node->planes[BOX_FACES_FRONT].setPointOnPlane(MathLib::vector4(node->position.extractX(), 0.0f, node->position.extractZ(), 1.0f));

	node->planes[BOX_FACES_BACK].setNormal(MathLib::vector4(0.0f, 0.0f, 1.0f, 0.0f));
	node->planes[BOX_FACES_BACK].setPointOnPlane(MathLib::vector4(node->position.extractX(), 0.0f, node->position.extractZ() + fullOffset, 1.0f));

	node->planes[BOX_FACES_RIGHT].setNormal(MathLib::vector4(1.0f, 0.0f, 0.0f, 0.0f));
	node->planes[BOX_FACES_RIGHT].setPointOnPlane(MathLib::vector4(node->position.extractX() + fullOffset, 0.0f, node->position.extractZ(), 1.0f));

	node->planes[BOX_FACES_LEFT].setNormal(MathLib::vector4(-1.0f, 0.0f, 0.0f, 0.0f));
	node->planes[BOX_FACES_LEFT].setPointOnPlane(MathLib::vector4(node->position.extractX(), 0.0f, node->position.extractZ(), 1.0f));

	node->planes[BOX_FACES_TOP].setNormal(MathLib::vector4(0.0f, 1.0f, 0.0f, 0.0f));
	node->planes[BOX_FACES_TOP].setPointOnPlane(MathLib::vector4(node->position.extractX(), TERRAIN_MAX_HEIGHT, node->position.extractZ(), 1.0f));

	node->planes[BOX_FACES_BOTTOM].setNormal(MathLib::vector4(0.0f, -1.0f, 0.0f, 0.0f));
	node->planes[BOX_FACES_BOTTOM].setPointOnPlane(MathLib::vector4(node->position.extractX(), 0.0f, node->position.extractZ(), 1.0f));

	// Compute vertices.
	node->vertices[BOX_VERTICES_FTL].setXYZW(node->position.extractX() + fullOffset, TERRAIN_MAX_HEIGHT, node->position.extractZ(), 1.0f);
	node->vertices[BOX_VERTICES_FBL].setXYZW(node->position.extractX() + fullOffset, 0.0f, node->position.extractZ(), 1.0f);

	node->vertices[BOX_VERTICES_FTR].setXYZW(node->position.extractX(), TERRAIN_MAX_HEIGHT, node->position.extractZ(), 1.0f);
	node->vertices[BOX_VERTICES_FBR].setXYZW(node->position.extractX(), 0.0f, node->position.extractZ(), 1.0f);

	node->vertices[BOX_VERTICES_BTL].setXYZW(node->position.extractX() + fullOffset, TERRAIN_MAX_HEIGHT, node->position.extractZ() + fullOffset, 1.0f);
	node->vertices[BOX_VERTICES_BBL].setXYZW(node->position.extractX() + fullOffset, 0.0f , node->position.extractZ() + fullOffset, 1.0f);

	node->vertices[BOX_VERTICES_BTR].setXYZW(node->position.extractX(), TERRAIN_MAX_HEIGHT, node->position.extractZ() + fullOffset, 1.0f);
	node->vertices[BOX_VERTICES_BBR].setXYZW(node->position.extractX(), 0.0f, node->position.extractZ() + fullOffset, 1.0f);

	// Compute extents.
	node->extents[AABB_EXTENTS_MIN].setXYZW(node->position.extractX(), 0.0f, node->position.extractZ(), 1.0f);
	node->extents[AABB_EXTENTS_MAX].setXYZW(node->position.extractX() + fullOffset, TERRAIN_MAX_HEIGHT, node->position.extractZ() + fullOffset, 1.0f);
}

}
