#ifndef SHADERMANAGER_H_INCLUDED
#define SHADERMANAGER_H_INCLUDED

#include "RendererLibCommon.h"
#include "ShaderProgram.h"
#include <string>
#include <map>

namespace RendererLib
{

class ShaderManager
{
	public:

		/// <summary>
		/// Returns the ShaderProgram specified by @param name.
		/// If the program does not exist, NULL is returned.
		/// </summary>
		ShaderProgram* GetShader(std::string name);

		/// <summary>
		/// On destruction, the shader manager will delete all of the resident ShaderProgram objects.
		/// </summary>
		~ShaderManager();

		static ShaderManager& GetInstance();

		static const std::string SHADER_DEFAULT_MESH;
		static const std::string SHADER_STATIC_MESH_SHADOW;
		static const std::string SHADER_DEPTH_ONLY;
		static const std::string SHADER_SINGLE_COLOR;
		static const std::string SHADER_FULLSCREEN_QUAD;
        static const std::string SHADER_DEPTH_TRANSFER;
        static const std::string SHADER_DIRECTIONAL_LIGHT;
        static const std::string SHADER_FXAA;
        static const std::string SHADER_TERRAIN_NAIVE;
        static const std::string SHADER_TERRAIN_CDLOD;
		static const std::string SHADER_TERRAIN_CDLOD_DEPTH_ONLY;
		static const std::string SHADER_SKY;
		static const std::string SHADER_OUTPUT_LINEAR_DEPTH;
		static const std::string SHADER_GENERATE_FOG_BUFFER;
		static const std::string SHADER_MERGE_FOG_BUFFER;

	protected:

		std::map<std::string, ShaderProgram*> programMap;

		ShaderManager();

		static ShaderManager* manager;
};

}

#endif // SHADERMANAGER_H_INCLUDED
