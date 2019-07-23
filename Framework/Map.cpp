
#include "Map.h"

#include <StoredTerrainLandscape.h>
#include <FileSystem.h>
#include <iostream>
#include <StoredLevel.h>

using namespace Framework;
using namespace RendererLib;
using namespace CommonCode;

Map::Map(float tileSize, int size, vector4 start)
{
    _size = size;
    _start = start;
    _tileSize = tileSize;
    _isLoaded = false;
}

void Map::Load(string filename)
{
    string folderPath = FileSystem::CurrentDir();
    folderPath.append("\\Levels");

    string fullPath = folderPath + "\\" + filename;

    FileInfo info(filename);

    StoredLevel* level = new StoredLevel();
    int separatorIndex = (int)filename.find("\\");

    string levelDirectory = filename.substr(0, separatorIndex);
    string levelMapFileName = filename.substr(separatorIndex + 1, filename.length());

    level->Load(folderPath + "\\" + levelDirectory, levelMapFileName);

    TiXmlDocument xmlDoc;
    xmlDoc.LoadFile(fullPath.c_str());

    TiXmlNode* node = xmlDoc.RootElement()->FirstChildElement("TerrainLandscapes");
    if (node != NULL)
    {
        TiXmlElement* terrainNode = node->FirstChildElement("TerrainLandscapeFile");

        StoredTerrainLandscape& loadedLandscape = *level->GetStoredTerrainLandscapes()[0];

        _terrainHeights = loadedLandscape.ReturnHeightData();

        _rendererTerrain = new RendererTerrainCDLOD();
        _rendererTerrain->Initialize(_terrainHeights);
        _rendererTerrain->UpdateSplatTexture0(loadedLandscape.ReturnSplatTexture0Data());
        _rendererTerrain->UpdateSplatTexture1(loadedLandscape.ReturnSplatTexture1Data());
        _rendererTerrain->SetTextureLayer0Diffuse(loadedLandscape.ReturnDiffuse0Path());
        _rendererTerrain->SetTextureLayer0Normal(loadedLandscape.ReturnNormal0Path());
        _rendererTerrain->SetTextureLayer1Diffuse(loadedLandscape.ReturnDiffuse1Path());
        _rendererTerrain->SetTextureLayer1Normal(loadedLandscape.ReturnNormal1Path());
        _rendererTerrain->SetTextureLayer2Diffuse(loadedLandscape.ReturnDiffuse2Path());
        _rendererTerrain->SetTextureLayer2Normal(loadedLandscape.ReturnNormal2Path());
        _rendererTerrain->SetTextureLayer3Diffuse(loadedLandscape.ReturnDiffuse3Path());
        _rendererTerrain->SetTextureLayer3Normal(loadedLandscape.ReturnNormal3Path());
        _rendererTerrain->SetTextureLayer4Diffuse(loadedLandscape.ReturnDiffuse4Path());
        _rendererTerrain->SetTextureLayer4Normal(loadedLandscape.ReturnNormal4Path());
        _rendererTerrain->SetTextureLayer5Diffuse(loadedLandscape.ReturnDiffuse5Path());
        _rendererTerrain->SetTextureLayer5Normal(loadedLandscape.ReturnNormal5Path());
        _rendererTerrain->SetTextureLayer6Diffuse(loadedLandscape.ReturnDiffuse6Path());
        _rendererTerrain->SetTextureLayer6Normal(loadedLandscape.ReturnNormal6Path());
        _rendererTerrain->SetTextureLayer7Diffuse(loadedLandscape.ReturnDiffuse7Path());
        _rendererTerrain->SetTextureLayer7Normal(loadedLandscape.ReturnNormal7Path());

        _terrain = new TerrainLib::Terrain(_start, _terrainHeights, (int)_rendererTerrain->GetMaxExtent());
    }

    delete level;
    level = nullptr;

    _isLoaded = true;
}


void Map::Close()
{

}