#include "Renderer.h"
#include "RenderToken.h"
#include "RendererUtils.h"
#include "RendererTerrainCDLOD.h"
#include "RendererTerrainCDLODNode.h"
#include "ShaderManager.h"
#include "RenderTokenTerrainCDLODNode.h"
#include <Terrain.h>
#include <Geometry.h>
#include <cassert>
#include <cstring>
#include <iostream>
#include <CommonDefines.h>

struct IndexedTriangle
{
	unsigned int i[3];
};

namespace RendererLib
{

bool RendererTerrainCDLOD::terrainCDLODVBOInstantiated = false;

GLuint RendererTerrainCDLOD::vertexBufferObjectHandle = 0;
GLuint RendererTerrainCDLOD::indexBufferObjectHandle = 0;

RendererTerrainCDLOD::RendererTerrainCDLOD()
{
	rootNode = NULL;

	heightmapTextureHandle = 0;
	textureLayer0Diffuse = "sand2_diffuse";
	textureLayer0Normal = "sand2_normal";

	textureRepeatFactor = 15.0f;

	CreateSplatTextures();

	if (!RendererTerrainCDLOD::terrainCDLODVBOInstantiated)
	{
		CreateTerrainCDLODVBO();
	}
}

RendererTerrainCDLOD::~RendererTerrainCDLOD()
{
	FreeResources();
}

void RendererTerrainCDLOD::Initialize(float const * const terrainData)
{
	FreeResources();

	unsigned int quadtreeDepth = RendererTerrainCDLODNode::GetNumQuadtreeLevels(TERRAIN_LANDSCAPE_SIZE, TERRAIN_PATCH_SIZE);

	lodRanges.ComputeRanges(Renderer::GetInstance().GetMainCamera().GetFarClipPlaneDistance(), quadtreeDepth);

	// Compute the root node, then generate the quadtree hierarchy off of it.
	rootNode = new RendererTerrainCDLODNode();
	rootNode->position.setXYZW(0.0f, 0.0f, 0.0f, 1.0f);
	rootNode->scale = static_cast<unsigned int>(powf(2.0f, static_cast<float>(quadtreeDepth)));

	rootNode->GenerateQuadtree();
	RendererTerrainCDLODNode::GenerateBoundingVolumeInformation(rootNode);

	CreateHeightmapTexture(terrainData);

	CreateSplatTextures();
}

RendererTerrainCDLODNode const * const RendererTerrainCDLOD::GetRootNode() const
{
	return rootNode;
}

void RendererTerrainCDLOD::FreeResources()
{
	if (NULL != rootNode)
	{
		delete rootNode;
	}

	if (glIsTexture(heightmapTextureHandle))
	{
		glDeleteTextures(1, &heightmapTextureHandle);
		heightmapTextureHandle = 0;
	}

	if (glIsTexture(splatTexture0Handle))
	{
		glDeleteTextures(1, &splatTexture0Handle);
		splatTexture0Handle = 0;
	}

	if (glIsTexture(splatTexture1Handle))
	{
		glDeleteTextures(1, &splatTexture1Handle);
		splatTexture1Handle = 0;
	}
}

void RendererTerrainCDLOD::CreateTerrainCDLODVBO()
{
	const unsigned int numVertices = TERRAIN_PATCH_SIZE * TERRAIN_PATCH_SIZE;
	const unsigned int gridSize = TERRAIN_PATCH_SIZE - 1;

	// Create vertex array.
	float* vertexArray = new float[numVertices * 3];
	for (unsigned int i = 0; i < numVertices; i++)
	{
		vertexArray[(i * 3) + 0] = static_cast<float>(i % TERRAIN_PATCH_SIZE);
		vertexArray[(i * 3) + 1] = 0.0f;
		vertexArray[(i * 3) + 2] = static_cast<float>(i / TERRAIN_PATCH_SIZE);
	}

	// Generate a list of Triangles that index the vertex array.
	const unsigned int numTriangles = gridSize * gridSize * 2;

	IndexedTriangle* triangles = new IndexedTriangle[numTriangles];
	unsigned int currentTriangleIndex = 0;
	for (unsigned int y = 0; y < gridSize; y++)
	{
		for (unsigned int x = 0; x < gridSize; x++)
		{
			// Generate first triangle.
			IndexedTriangle& firstTriangle = triangles[currentTriangleIndex];
			firstTriangle.i[0] = x + (y * (gridSize + 1));
			firstTriangle.i[1] = firstTriangle.i[0] + 1;
			firstTriangle.i[2] = x + ((y + 1) * (gridSize + 1));
			currentTriangleIndex++;

			// Generate second triangle.
			IndexedTriangle& secondTriangle = triangles[currentTriangleIndex];
			secondTriangle.i[0] = firstTriangle.i[1];
			secondTriangle.i[1] = (x + 1) + ((y + 1) * (gridSize + 1));
			secondTriangle.i[2] = firstTriangle.i[2];
			currentTriangleIndex++;
		}
	}

	// Compute texture coordinates.
	float* textureArray = new float[numVertices * 2];
	for (unsigned int i = 0; i < numVertices; i++)
	{
		textureArray[(i * 2) + 0] = static_cast<float>((i % (TERRAIN_PATCH_SIZE))) / (gridSize);
		textureArray[(i * 2) + 1] = ((gridSize) - ((int)i / (int)TERRAIN_PATCH_SIZE)) / static_cast<float>(gridSize);
	}

	// Next create the index array.
	const unsigned int numIndices = numTriangles * 3;
	uint32_t* indexArray = new uint32_t[numIndices];
	for (unsigned int i = 0; i < numTriangles; i++)
	{
		indexArray[(i * 3) + 0] = triangles[i].i[0];
		indexArray[(i * 3) + 1] = triangles[i].i[1];
		indexArray[(i * 3) + 2] = triangles[i].i[2];
	}

	const unsigned int vertexArraySizeBytes = numVertices * 3 * sizeof(float);
	const unsigned int textureArraySizeBytes = numVertices * 2 * sizeof(float);
	const unsigned int indexArraySizeBytes = numIndices * sizeof(uint32_t);

	RendererUtils::GenerateVBO(&RendererTerrainCDLOD::vertexBufferObjectHandle,
							   &RendererTerrainCDLOD::indexBufferObjectHandle,
							   vertexArraySizeBytes,
							   vertexArray,
							   textureArraySizeBytes,
							   textureArray,
							   indexArraySizeBytes,
							   indexArray);

	delete [] vertexArray;
	delete [] textureArray;
	delete [] indexArray;
	delete [] triangles;

	RendererTerrainCDLOD::terrainCDLODVBOInstantiated = true;
}

std::vector<RenderToken*> RendererTerrainCDLOD::GetRenderTokens(const CameraLib::Camera& camera)
{
	std::vector<RenderToken*> tokens;

	ProcessQuadtreeNodeForDrawing(camera, rootNode, lodRanges.rangesLength - 1, tokens);

	return tokens;
}

void RendererTerrainCDLOD::ProcessQuadtreeNodeForDrawing(const CameraLib::Camera& camera, RendererTerrainCDLODNode* node, int lodLevel, std::vector<RenderToken*>& renderTokens)
{
	Renderer& renderer = Renderer::GetInstance();
	
	// Do subdivision tests.
	// If the current node intersects the sphere of the lod range beneath it, its children can be visited.
    MathLib::vector4 cameraPosition;
    MathLib::vector4_copy(cameraPosition, camera.GetPosition());
	
	if (lodLevel >= 0)
	{
		if (GeometryLib::SphereBoxIntersection(cameraPosition, lodRanges.ranges[lodLevel].distance, node->GetPlanes()) ||
			GeometryLib::BoxInsideSphere(cameraPosition, lodRanges.ranges[lodLevel].distance, node->GetBoundingBoxVertices()))
		{
			for (unsigned int i = 0; i < 4; i++)
			{
				if (NULL != node->nodes[i])
				{
					if (lodLevel == 0)
					{
						int bob = lodLevel;
					}

					ProcessQuadtreeNodeForDrawing(camera, node->nodes[i], lodLevel - 1, renderTokens);
				}
			}

			return;
		}
	}

	// Create RendererToken.
	renderTokens.push_back(new RenderTokenTerrainCDLODNode(*this, *node));
}

static void DrawDebugQuadtree(RendererTerrainCDLODNode* rootNode, const RendererTerrainCDLODRanges& lodRanges)
{
    glDisable(GL_TEXTURE_2D);
  //  glDisable(GL_DEPTH_TEST);

    Renderer& renderer = Renderer::GetInstance();
    renderer.SetUpCamera(renderer.GetMainCamera());

    ShaderManager& shaderManager = ShaderManager::GetInstance();
    ShaderProgram* shader = shaderManager.GetShader(ShaderManager::SHADER_SINGLE_COLOR);
    assert(NULL != shader);
    shader->Use();

    GLuint programHandle = shader->GetProgramHandle();
    shader->SetUniform3f("color", 1.0f, 1.0f, 1.0f);

    float matrixArray[16];

    // Set object to world space matrix
    MathLib::matrix4x4 transform;
    transform.loadIdentity();
    MathLib::matrix4x4_copy(renderer.matrices[MATRIX_OBJECT_WORLD_INDEX], transform);

    // Get MVP matrix, send to GPU
    renderer.UpdateModelViewProjectionMatrix();
    renderer.matrices[MATRIX_MVP_INDEX].setOpenGLMatrix(matrixArray);

    shader->SetUniformMatrix4fv("transform", static_cast<GLfloat*>(matrixArray), false);

	// Do subdivision tests.
	// If the current node intersects the sphere of the lod range beneath it, its children can be visited.
	if (rootNode->scale > 1)
	{
		// Because the correct mathematical subdivision of the landscape results in a suboptimal quadtree size
		// (the terrain size is reduced to roughly half) we increase the quadtree landscape size by one level.
		// However this results in some slight hackery when selecting lod ranges.
		unsigned int lodRange = static_cast<unsigned int>(log2(rootNode->scale));
		if (lodRange >= lodRanges.rangesLength)
			lodRange = lodRanges.rangesLength - 1;

		if (GeometryLib::SphereBoxIntersection(renderer.GetMainCamera().GetPosition(), lodRanges.ranges[lodRange - 1].distance, rootNode->GetPlanes()))
		{
			for (unsigned int i = 0; i < 4; i++)
			{
				if (NULL != rootNode->nodes[i])
				{
					DrawDebugQuadtree(rootNode->nodes[i], lodRanges);
				}
			}

			return;
		}
	}

	float offsetLength = (float)((TERRAIN_PATCH_SIZE - 1) * rootNode->scale);

	MathLib::vector4 const * const boundingBoxVertices = rootNode->GetBoundingBoxVertices();

	// Front face.
	glBegin(GL_LINE_LOOP);
		glVertex3f(boundingBoxVertices[BOX_VERTICES_FTL].extractX(),
				   boundingBoxVertices[BOX_VERTICES_FTL].extractY(),
				   boundingBoxVertices[BOX_VERTICES_FTL].extractZ());

		glVertex3f(boundingBoxVertices[BOX_VERTICES_FTR].extractX(),
				   boundingBoxVertices[BOX_VERTICES_FTR].extractY(),
				   boundingBoxVertices[BOX_VERTICES_FTR].extractZ());

		glVertex3f(boundingBoxVertices[BOX_VERTICES_FBR].extractX(),
				   boundingBoxVertices[BOX_VERTICES_FBR].extractY(),
				   boundingBoxVertices[BOX_VERTICES_FBR].extractZ());

		glVertex3f(boundingBoxVertices[BOX_VERTICES_FBL].extractX(),
				   boundingBoxVertices[BOX_VERTICES_FBL].extractY(),
				   boundingBoxVertices[BOX_VERTICES_FBL].extractZ());
	glEnd();

	// Back face.
	glBegin(GL_LINE_LOOP);
		glVertex3f(boundingBoxVertices[BOX_VERTICES_BTL].extractX(),
				   boundingBoxVertices[BOX_VERTICES_BTL].extractY(),
				   boundingBoxVertices[BOX_VERTICES_BTL].extractZ());

		glVertex3f(boundingBoxVertices[BOX_VERTICES_BTR].extractX(),
				   boundingBoxVertices[BOX_VERTICES_BTR].extractY(),
				   boundingBoxVertices[BOX_VERTICES_BTR].extractZ());

		glVertex3f(boundingBoxVertices[BOX_VERTICES_BBR].extractX(),
				   boundingBoxVertices[BOX_VERTICES_BBR].extractY(),
				   boundingBoxVertices[BOX_VERTICES_BBR].extractZ());

		glVertex3f(boundingBoxVertices[BOX_VERTICES_BBL].extractX(),
				   boundingBoxVertices[BOX_VERTICES_BBL].extractY(),
				   boundingBoxVertices[BOX_VERTICES_BBL].extractZ());
	glEnd();

	// Right face.
	glBegin(GL_LINE_LOOP);
		glVertex3f(boundingBoxVertices[BOX_VERTICES_FTR].extractX(),
				   boundingBoxVertices[BOX_VERTICES_FTR].extractY(),
				   boundingBoxVertices[BOX_VERTICES_FTR].extractZ());

		glVertex3f(boundingBoxVertices[BOX_VERTICES_BTR].extractX(),
				   boundingBoxVertices[BOX_VERTICES_BTR].extractY(),
				   boundingBoxVertices[BOX_VERTICES_BTR].extractZ());

		glVertex3f(boundingBoxVertices[BOX_VERTICES_BBR].extractX(),
				   boundingBoxVertices[BOX_VERTICES_BBR].extractY(),
				   boundingBoxVertices[BOX_VERTICES_BBR].extractZ());

		glVertex3f(boundingBoxVertices[BOX_VERTICES_FBR].extractX(),
				   boundingBoxVertices[BOX_VERTICES_FBR].extractY(),
				   boundingBoxVertices[BOX_VERTICES_FBR].extractZ());
	glEnd();

	// Left face.
	glBegin(GL_LINE_LOOP);
		glVertex3f(boundingBoxVertices[BOX_VERTICES_FTL].extractX(),
				   boundingBoxVertices[BOX_VERTICES_FTL].extractY(),
				   boundingBoxVertices[BOX_VERTICES_FTL].extractZ());

		glVertex3f(boundingBoxVertices[BOX_VERTICES_BTL].extractX(),
				   boundingBoxVertices[BOX_VERTICES_BTL].extractY(),
				   boundingBoxVertices[BOX_VERTICES_BTL].extractZ());

		glVertex3f(boundingBoxVertices[BOX_VERTICES_BBL].extractX(),
				   boundingBoxVertices[BOX_VERTICES_BBL].extractY(),
				   boundingBoxVertices[BOX_VERTICES_BBL].extractZ());

		glVertex3f(boundingBoxVertices[BOX_VERTICES_FBL].extractX(),
				   boundingBoxVertices[BOX_VERTICES_FBL].extractY(),
				   boundingBoxVertices[BOX_VERTICES_FBL].extractZ());
	glEnd();
}

static void DrawDebugQuadtree(RendererTerrainCDLODNode* rootNode, const vector<TerrainCDLODOrderedNode>& orderedNodeList)
{
    glDisable(GL_TEXTURE_2D);
  //  glDisable(GL_DEPTH_TEST);

    Renderer& renderer = Renderer::GetInstance();
    renderer.SetUpCamera(renderer.GetMainCamera());

    ShaderManager& shaderManager = ShaderManager::GetInstance();
    ShaderProgram* shader = shaderManager.GetShader(ShaderManager::SHADER_SINGLE_COLOR);
    assert(NULL != shader);
    shader->Use();

    GLuint programHandle = shader->GetProgramHandle();
    shader->SetUniform3f("color", 1.0f, 1.0f, 1.0f);

    float matrixArray[16];

    // Set object to world space matrix
    MathLib::matrix4x4 transform;
    transform.loadIdentity();
    MathLib::matrix4x4_copy(renderer.matrices[MATRIX_OBJECT_WORLD_INDEX], transform);

    // Get MVP matrix, send to GPU
    renderer.UpdateModelViewProjectionMatrix();
    renderer.matrices[MATRIX_MVP_INDEX].setOpenGLMatrix(matrixArray);

    shader->SetUniformMatrix4fv("transform", static_cast<GLfloat*>(matrixArray), false);

	// Do subdivision tests.
	// If the current node intersects the sphere of the lod range beneath it, its children can be visited.
	if (rootNode->scale > 1)
	{
		for (unsigned int i = 0; i < 4; i++)
		{
			if (NULL != rootNode->nodes[i])
			{
				DrawDebugQuadtree(rootNode->nodes[i], orderedNodeList);
			}
		}

		return;
	}

	float offsetLength = (float)((TERRAIN_PATCH_SIZE - 1) * rootNode->scale);

	MathLib::vector4 const * const boundingBoxVertices = rootNode->GetBoundingBoxVertices();

	bool isInList = false;
//	for (int i = 0; i < orderedNodeList.Count(); i++)
//	{
//		if (rootNode == orderedNodeList.At(i).node)
//		{
//			isInList = true;
//		}
//	}

	if (!isInList)
		return;

	// Front face.
	glBegin(GL_LINE_LOOP);
		glVertex3f(boundingBoxVertices[BOX_VERTICES_FTL].extractX(),
				   boundingBoxVertices[BOX_VERTICES_FTL].extractY(),
				   boundingBoxVertices[BOX_VERTICES_FTL].extractZ());

		glVertex3f(boundingBoxVertices[BOX_VERTICES_FTR].extractX(),
				   boundingBoxVertices[BOX_VERTICES_FTR].extractY(),
				   boundingBoxVertices[BOX_VERTICES_FTR].extractZ());

		glVertex3f(boundingBoxVertices[BOX_VERTICES_FBR].extractX(),
				   boundingBoxVertices[BOX_VERTICES_FBR].extractY(),
				   boundingBoxVertices[BOX_VERTICES_FBR].extractZ());

		glVertex3f(boundingBoxVertices[BOX_VERTICES_FBL].extractX(),
				   boundingBoxVertices[BOX_VERTICES_FBL].extractY(),
				   boundingBoxVertices[BOX_VERTICES_FBL].extractZ());
	glEnd();

	// Back face.
	glBegin(GL_LINE_LOOP);
		glVertex3f(boundingBoxVertices[BOX_VERTICES_BTL].extractX(),
				   boundingBoxVertices[BOX_VERTICES_BTL].extractY(),
				   boundingBoxVertices[BOX_VERTICES_BTL].extractZ());

		glVertex3f(boundingBoxVertices[BOX_VERTICES_BTR].extractX(),
				   boundingBoxVertices[BOX_VERTICES_BTR].extractY(),
				   boundingBoxVertices[BOX_VERTICES_BTR].extractZ());

		glVertex3f(boundingBoxVertices[BOX_VERTICES_BBR].extractX(),
				   boundingBoxVertices[BOX_VERTICES_BBR].extractY(),
				   boundingBoxVertices[BOX_VERTICES_BBR].extractZ());

		glVertex3f(boundingBoxVertices[BOX_VERTICES_BBL].extractX(),
				   boundingBoxVertices[BOX_VERTICES_BBL].extractY(),
				   boundingBoxVertices[BOX_VERTICES_BBL].extractZ());
	glEnd();

	// Right face.
	glBegin(GL_LINE_LOOP);
		glVertex3f(boundingBoxVertices[BOX_VERTICES_FTR].extractX(),
				   boundingBoxVertices[BOX_VERTICES_FTR].extractY(),
				   boundingBoxVertices[BOX_VERTICES_FTR].extractZ());

		glVertex3f(boundingBoxVertices[BOX_VERTICES_BTR].extractX(),
				   boundingBoxVertices[BOX_VERTICES_BTR].extractY(),
				   boundingBoxVertices[BOX_VERTICES_BTR].extractZ());

		glVertex3f(boundingBoxVertices[BOX_VERTICES_BBR].extractX(),
				   boundingBoxVertices[BOX_VERTICES_BBR].extractY(),
				   boundingBoxVertices[BOX_VERTICES_BBR].extractZ());

		glVertex3f(boundingBoxVertices[BOX_VERTICES_FBR].extractX(),
				   boundingBoxVertices[BOX_VERTICES_FBR].extractY(),
				   boundingBoxVertices[BOX_VERTICES_FBR].extractZ());
	glEnd();

	// Left face.
	glBegin(GL_LINE_LOOP);
		glVertex3f(boundingBoxVertices[BOX_VERTICES_FTL].extractX(),
				   boundingBoxVertices[BOX_VERTICES_FTL].extractY(),
				   boundingBoxVertices[BOX_VERTICES_FTL].extractZ());

		glVertex3f(boundingBoxVertices[BOX_VERTICES_BTL].extractX(),
				   boundingBoxVertices[BOX_VERTICES_BTL].extractY(),
				   boundingBoxVertices[BOX_VERTICES_BTL].extractZ());

		glVertex3f(boundingBoxVertices[BOX_VERTICES_BBL].extractX(),
				   boundingBoxVertices[BOX_VERTICES_BBL].extractY(),
				   boundingBoxVertices[BOX_VERTICES_BBL].extractZ());

		glVertex3f(boundingBoxVertices[BOX_VERTICES_FBL].extractX(),
				   boundingBoxVertices[BOX_VERTICES_FBL].extractY(),
				   boundingBoxVertices[BOX_VERTICES_FBL].extractZ());
	glEnd();
}

void RendererTerrainCDLOD::DebugQuadtree()
{
	DrawDebugQuadtree(rootNode, lodRanges);
}

void RendererTerrainCDLOD::DebugQuadtree(const vector<TerrainCDLODOrderedNode>& orderedNodeList)
{
	DrawDebugQuadtree(rootNode, orderedNodeList);
}

void RendererTerrainCDLOD::CreateHeightmapTexture(float const * const heightArray)
{
	glGenTextures(1, &heightmapTextureHandle);
	glBindTexture(GL_TEXTURE_2D, heightmapTextureHandle);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D,
				 0,
				 GL_R16F,
				 TERRAIN_LANDSCAPE_SIZE,
				 TERRAIN_LANDSCAPE_SIZE,
				 0,
				 GL_RED,
				 GL_FLOAT,
				 heightArray);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void RendererTerrainCDLOD::CreateSplatTextures()
{
	unsigned int numTexels = TERRAIN_SPLAT_TEXTURE_DIMENSION * TERRAIN_SPLAT_TEXTURE_DIMENSION * 4;

	uint8_t* textureValues = new uint8_t[numTexels];
	memset(textureValues, 0x0, numTexels);
	for (unsigned int i = 0; i < numTexels; i += 4)
	{
		textureValues[i] = 255;
	}

	glGenTextures(1, &splatTexture0Handle);
	glBindTexture(GL_TEXTURE_2D, splatTexture0Handle);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D,
				 0,
				 GL_RGBA,
				 TERRAIN_SPLAT_TEXTURE_DIMENSION,
				 TERRAIN_SPLAT_TEXTURE_DIMENSION,
				 0,
				 GL_RGBA,
				 GL_UNSIGNED_BYTE,
				 textureValues);

	memset(textureValues, 0x0, numTexels);
	glGenTextures(1, &splatTexture1Handle);
	glBindTexture(GL_TEXTURE_2D, splatTexture1Handle);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D,
				 0,
				 GL_RGBA,
				 TERRAIN_SPLAT_TEXTURE_DIMENSION,
				 TERRAIN_SPLAT_TEXTURE_DIMENSION,
				 0,
				 GL_RGBA,
				 GL_UNSIGNED_BYTE,
				 textureValues);

	delete [] textureValues;
}

GLuint RendererTerrainCDLOD::GetNodeVBOHandle()
{
	if (!terrainCDLODVBOInstantiated)
	{
		CreateTerrainCDLODVBO();
	}

	return vertexBufferObjectHandle;
}

GLuint RendererTerrainCDLOD::GetNodeIndexVBOHandle()
{
	if (!terrainCDLODVBOInstantiated)
	{
		CreateTerrainCDLODVBO();
	}

	return indexBufferObjectHandle;
}

GLvoid* RendererTerrainCDLOD::GetNodeVBOTextureAttribOffset()
{
	if (!terrainCDLODVBOInstantiated)
	{
		CreateTerrainCDLODVBO();
	}

	const unsigned int numVertices = (TERRAIN_PATCH_SIZE * TERRAIN_PATCH_SIZE);
	return  reinterpret_cast<GLvoid*>(numVertices * 3 * sizeof(float));
}

GLuint RendererTerrainCDLOD::GetNodeIndexVBONumIndcies()
{
	if (!terrainCDLODVBOInstantiated)
	{
		CreateTerrainCDLODVBO();
	}

	const unsigned int gridSize = TERRAIN_PATCH_SIZE - 1;
	const unsigned int numTriangles = gridSize * gridSize * 2;
	const unsigned int numIndices = numTriangles * 3;

	return static_cast<GLuint>(numIndices);
}

void RendererTerrainCDLOD::UpdateHeightmap(float const * const terrainData)
{
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, heightmapTextureHandle);

	glTexSubImage2D(GL_TEXTURE_2D,
					0,
					0,
					0,
					TERRAIN_LANDSCAPE_SIZE,
					TERRAIN_LANDSCAPE_SIZE,
					GL_RED,
					GL_FLOAT,
					terrainData);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void RendererTerrainCDLOD::UpdateSplatTexture0(uint8_t const * const splatData)
{
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, splatTexture0Handle);

	glTexSubImage2D(GL_TEXTURE_2D,
					0,
					0,
					0,
					TERRAIN_SPLAT_TEXTURE_DIMENSION,
					TERRAIN_SPLAT_TEXTURE_DIMENSION,
					GL_RGBA,
					GL_UNSIGNED_BYTE,
					splatData);

	glBindTexture(GL_TEXTURE_2D, 0);
}

void RendererTerrainCDLOD::UpdateSplatTexture1(uint8_t const * const splatData)
{
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, splatTexture1Handle);

	glTexSubImage2D(GL_TEXTURE_2D,
					0,
					0,
					0,
					TERRAIN_SPLAT_TEXTURE_DIMENSION,
					TERRAIN_SPLAT_TEXTURE_DIMENSION,
					GL_RGBA,
					GL_UNSIGNED_BYTE,
					splatData);

	glBindTexture(GL_TEXTURE_2D, 0);
}

float RendererTerrainCDLOD::GetMaxExtent() const
{
    int returnValue = (TERRAIN_PATCH_SIZE - 1) * rootNode->scale;
    if (returnValue >= TERRAIN_LANDSCAPE_SIZE)
        returnValue = TERRAIN_LANDSCAPE_SIZE - 1;

	return (float)returnValue;
}

}
