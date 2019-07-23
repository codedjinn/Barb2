#include "TerrainQuadtree.h"
#include <MathLib.h>
#include <cassert>
#include <iostream>

namespace TerrainLib
{

TerrainQuadtreeNode::TerrainQuadtreeNode()
{
	for (int i = 0; i < 4; i++)
	{
		nodes[i] = NULL;
	}
}

TerrainQuadtreeNode::~TerrainQuadtreeNode()
{
	FreeResources();
}

bool TerrainQuadtreeNode::IsChild() const
{
	for (int i = 0; i < 4; i++)
	{
		if (NULL != nodes[i])
		{
			return false;
		}
	}
	return true;
}

void TerrainQuadtreeNode::FreeResources()
{
	for (int i = 0; i < 4; i++)
	{
		if (NULL != nodes[i])
		{
			delete nodes[i];
			nodes[i] = NULL;
		}
	}
}

void ProcessQuadtreeNodeForCreation(TerrainQuadtreeNode* node)
{
	if (node->scale > 2)
	{
		unsigned int halfOffset = node->scale >> 1;

		// Create new level in the quadtree.
		// Top left node.
		node->nodes[TerrainQuadtreeNode::QUADTREE_NODE_TL] = new TerrainQuadtreeNode;
		node->nodes[TerrainQuadtreeNode::QUADTREE_NODE_TL]->position.setXYZW(node->position.extractX(),
																			 node->position.extractY(),
																			 node->position.extractZ(),
																			 1.0f);
		node->nodes[TerrainQuadtreeNode::QUADTREE_NODE_TL]->scale = halfOffset;
		ProcessQuadtreeNodeForCreation(node->nodes[TerrainQuadtreeNode::QUADTREE_NODE_TL]);

		// Top right node.
		node->nodes[TerrainQuadtreeNode::QUADTREE_NODE_TR] = new TerrainQuadtreeNode;
		node->nodes[TerrainQuadtreeNode::QUADTREE_NODE_TR]->position.setXYZW(node->position.extractX() + halfOffset,
																			 node->position.extractY(),
																			 node->position.extractZ(),
																			 1.0f);
		node->nodes[TerrainQuadtreeNode::QUADTREE_NODE_TR]->scale = halfOffset;
		ProcessQuadtreeNodeForCreation(node->nodes[TerrainQuadtreeNode::QUADTREE_NODE_TR]);

		// Bottom left ndoe.
		node->nodes[TerrainQuadtreeNode::QUADTREE_NODE_BL] = new TerrainQuadtreeNode;
		node->nodes[TerrainQuadtreeNode::QUADTREE_NODE_BL]->position.setXYZW(node->position.extractX(),
																			 node->position.extractY(),
																			 node->position.extractZ() + halfOffset,
																			 1.0f);
		node->nodes[TerrainQuadtreeNode::QUADTREE_NODE_BL]->scale = halfOffset;
		ProcessQuadtreeNodeForCreation(node->nodes[TerrainQuadtreeNode::QUADTREE_NODE_BL]);

		// Bottom right node.
		node->nodes[TerrainQuadtreeNode::QUADTREE_NODE_BR] = new TerrainQuadtreeNode;
		node->nodes[TerrainQuadtreeNode::QUADTREE_NODE_BR]->position.setXYZW(node->position.extractX() + halfOffset,
																			 node->position.extractY(),
																			 node->position.extractZ() + halfOffset,
																			 1.0f);
		node->nodes[TerrainQuadtreeNode::QUADTREE_NODE_BR]->scale = halfOffset;
		ProcessQuadtreeNodeForCreation(node->nodes[TerrainQuadtreeNode::QUADTREE_NODE_BR]);
	}
}

TerrainQuadtreeNode* TerrainQuadtreeNode::GenerateQuadtree(const MathLib::vector4 position, int rendererQuadtreeMaxExtent)
{
	assert(rendererQuadtreeMaxExtent > 1);

    std::cerr << "Renderer QUADTREE MAX EXTENT: " << rendererQuadtreeMaxExtent << std::endl;

	int rootNodeScale = static_cast<int>(powf(2.0f, static_cast<float>(TerrainQuadtreeNode::GetSystemQuadtreeDepth(rendererQuadtreeMaxExtent))));

    std::cerr << "System QUADTREE Scale: " << rootNodeScale << std::endl;

	// Create root node.
	TerrainQuadtreeNode* rootNode = new TerrainQuadtreeNode;
	MathLib::vector4_copy(rootNode->position, position);
	rootNode->scale = static_cast<unsigned int>(rootNodeScale);



	// Generate the hierarchy.
	ProcessQuadtreeNodeForCreation(rootNode);

	return rootNode;
}

int TerrainQuadtreeNode::GetSystemQuadtreeDepth(int maxExtent)
{
	return static_cast<int>(log2(static_cast<double>(maxExtent))) + 1;
}

void TerrainQuadtreeNode::CalculateExtents(MathLib::vector4 extentsArray[]) const
{
	float fullOffset = static_cast<float>(scale);

	MathLib::vector4_copy(extentsArray[0], position);
	extentsArray[1].setXYZW(position.extractX() + fullOffset,
							TERRAIN_MAX_HEIGHT,
							position.extractZ() + fullOffset,
							1.0f);
}

}
