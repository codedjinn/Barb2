#pragma once

#include <string>
#include <RendererTerrain.h>
#include <RendererTerrainCDLOD.h>
#include <MathLib_Vector4.h>
#include <Terrain.h>

namespace Framework
{

    using namespace MathLib;
    using namespace std;

    class Map
    {
    public:

        Map(float tileSize, int size, vector4 start);

        void Load(string filename);

        void Close();

        TerrainLib::Terrain* GetTerrain() 
        {
            return _terrain;
        }

        RendererLib::RendererTerrainCDLOD* GetRendererTerrain()
        {
            return _rendererTerrain;
        }

    private:

        // system terrain
        TerrainLib::Terrain* _terrain;

        // visual terrain
        RendererLib::RendererTerrainCDLOD* _rendererTerrain;

        int _size;
        vector4 _start;
        bool _isLoaded = true;

        float* _terrainHeights;
        

        float _tileSize;
    };
}

