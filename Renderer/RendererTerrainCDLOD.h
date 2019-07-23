#ifndef RENDERERTERRAINCDLOD_H_INCLUDED
#define RENDERERTERRAINCDLOD_H_INCLUDED

#include "RendererLibCommon.h"
#include "RendererTerrainCommon.h"
#include "RendererTerrain.h"
#include "RendererTerrainCDLODRanges.h"
#include <MathLib.h>
#include <string>
#include <vector>

namespace RendererLib
{

using namespace std;

class RenderToken;

class RendererTerrainCDLODNode;

struct TerrainCDLODOrderedNode
{
	RendererTerrainCDLODNode const * node;	/// The node that was intersected.
	float t;								/// The t value of the ray at the point of intersection with this node.

	bool operator==(const TerrainCDLODOrderedNode& right)
	{
		return (this == &right);
	}
};

class RendererTerrainCDLOD :
	public RendererTerrain
{
	public:

		RendererTerrainCDLOD();
		virtual ~RendererTerrainCDLOD();

		void Initialize(float const * const terrainData);

		void UpdateHeightmap(float const * const terrainData);
		void UpdateSplatTexture0(uint8_t const * const splatData);
		void UpdateSplatTexture1(uint8_t const * const splatData);

		const std::string& GetTextureLayer0Diffuse() const {return textureLayer0Diffuse;}
		const std::string& GetTextureLayer1Diffuse() const {return textureLayer1Diffuse;}
		const std::string& GetTextureLayer2Diffuse() const {return textureLayer2Diffuse;}
		const std::string& GetTextureLayer3Diffuse() const {return textureLayer3Diffuse;}
		const std::string& GetTextureLayer4Diffuse() const {return textureLayer4Diffuse;}
		const std::string& GetTextureLayer5Diffuse() const {return textureLayer5Diffuse;}
		const std::string& GetTextureLayer6Diffuse() const {return textureLayer6Diffuse;}
		const std::string& GetTextureLayer7Diffuse() const {return textureLayer7Diffuse;}
		const std::string& GetTextureLayer0Normal() const {return textureLayer0Normal;}
		const std::string& GetTextureLayer1Normal() const {return textureLayer1Normal;}
		const std::string& GetTextureLayer2Normal() const {return textureLayer2Normal;}
		const std::string& GetTextureLayer3Normal() const {return textureLayer3Normal;}
		const std::string& GetTextureLayer4Normal() const {return textureLayer4Normal;}
		const std::string& GetTextureLayer5Normal() const {return textureLayer5Normal;}
		const std::string& GetTextureLayer6Normal() const {return textureLayer6Normal;}
		const std::string& GetTextureLayer7Normal() const {return textureLayer7Normal;}

		void SetTextureLayer0Diffuse(const std::string& textureID) {textureLayer0Diffuse = textureID;}
		void SetTextureLayer1Diffuse(const std::string& textureID) {textureLayer1Diffuse = textureID;}
		void SetTextureLayer2Diffuse(const std::string& textureID) {textureLayer2Diffuse = textureID;}
		void SetTextureLayer3Diffuse(const std::string& textureID) {textureLayer3Diffuse = textureID;}
		void SetTextureLayer4Diffuse(const std::string& textureID) {textureLayer4Diffuse = textureID;}
		void SetTextureLayer5Diffuse(const std::string& textureID) {textureLayer5Diffuse = textureID;}
		void SetTextureLayer6Diffuse(const std::string& textureID) {textureLayer6Diffuse = textureID;}
		void SetTextureLayer7Diffuse(const std::string& textureID) {textureLayer7Diffuse = textureID;}
		void SetTextureLayer0Normal(const std::string& textureID) {textureLayer0Normal = textureID;}
		void SetTextureLayer1Normal(const std::string& textureID) {textureLayer1Normal = textureID;}
		void SetTextureLayer2Normal(const std::string& textureID) {textureLayer2Normal = textureID;}
		void SetTextureLayer3Normal(const std::string& textureID) {textureLayer3Normal = textureID;}
		void SetTextureLayer4Normal(const std::string& textureID) {textureLayer4Normal = textureID;}
		void SetTextureLayer5Normal(const std::string& textureID) {textureLayer5Normal = textureID;}
		void SetTextureLayer6Normal(const std::string& textureID) {textureLayer6Normal = textureID;}
		void SetTextureLayer7Normal(const std::string& textureID) {textureLayer7Normal = textureID;}

		float GetTerrainTextureFactor() const {return textureRepeatFactor;}

		float GetMaxExtent() const;
		/// Returns the maximum extent (relative to the position) of this terrain landscape.

		std::vector<RenderToken*> GetRenderTokens(const CameraLib::Camera& camera);

		void DebugQuadtree();
		void DebugQuadtree(const vector<TerrainCDLODOrderedNode>& orderedNodeList);

		GLuint GetHeightmapTextureHandle() const {return heightmapTextureHandle;}
		GLuint GetSplatTexture0Handle() const {return splatTexture0Handle;}
		GLuint GetSplatTexture1Handle() const {return splatTexture1Handle;}

		RendererTerrainCDLODNode const * const GetRootNode() const;

		const RendererTerrainCDLODRanges& GetLodRanges() const {return lodRanges;}

		static GLuint GetNodeVBOHandle();
		static GLuint GetNodeIndexVBOHandle();
		static GLvoid* GetNodeVBOTextureAttribOffset();
		static GLuint GetNodeIndexVBONumIndcies();

	protected:

		RendererTerrainCDLODNode* rootNode;

		RendererTerrainCDLODRanges lodRanges;

		float textureRepeatFactor;

		std::string textureLayer0Diffuse;
		std::string textureLayer0Normal;

		std::string textureLayer1Diffuse;
		std::string textureLayer1Normal;

		std::string textureLayer2Diffuse;
		std::string textureLayer2Normal;

		std::string textureLayer3Diffuse;
		std::string textureLayer3Normal;

		std::string textureLayer4Diffuse;
		std::string textureLayer4Normal;

		std::string textureLayer5Diffuse;
		std::string textureLayer5Normal;

		std::string textureLayer6Diffuse;
		std::string textureLayer6Normal;

		std::string textureLayer7Diffuse;
		std::string textureLayer7Normal;

		void FreeResources();
		/// Frees all allocated resources for this Terrain landscape.

		void CreateHeightmapTexture(float const * const heightArray);
		/// Creates the initial heightmap texture.

		void CreateSplatTextures();
		/// Creates the initial splat texture.

		void ProcessQuadtreeNodeForDrawing(const CameraLib::Camera& camera, RendererTerrainCDLODNode* node, int lodLevel, std::vector<RenderToken*>& renderTokens);

		GLuint heightmapTextureHandle;
		GLuint splatTexture0Handle;
		GLuint splatTexture1Handle;

		static bool terrainCDLODVBOInstantiated;
		static GLuint vertexBufferObjectHandle;
		static GLuint indexBufferObjectHandle;

		static void CreateTerrainCDLODVBO();
};

}


#endif // RENDERERTERRAINCDLOD_H_INCLUDED
