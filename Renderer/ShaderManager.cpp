#include "ShaderManager.h"

namespace RendererLib
{
ShaderManager* ShaderManager::manager = NULL;

const std::string ShaderManager::SHADER_DEFAULT_MESH				= "Shaders/ShaderDefaultMesh";
const std::string ShaderManager::SHADER_STATIC_MESH_SHADOW			= "Shaders/ShaderStaticMeshShadow";
const std::string ShaderManager::SHADER_DEPTH_ONLY					= "Shaders/ShaderDepthOnly";
const std::string ShaderManager::SHADER_SINGLE_COLOR    			= "Shaders/ShaderSingleColor";
const std::string ShaderManager::SHADER_FULLSCREEN_QUAD 			= "Shaders/ShaderFullscreenQuad";
const std::string ShaderManager::SHADER_DEPTH_TRANSFER  			= "Shaders/ShaderDepthTransfer";
const std::string ShaderManager::SHADER_DIRECTIONAL_LIGHT			= "Shaders/ShaderDirectionalLight";
const std::string ShaderManager::SHADER_FXAA						= "Shaders/ShaderFXAA";
const std::string ShaderManager::SHADER_TERRAIN_NAIVE				= "Shaders/ShaderTerrainNaive";
const std::string ShaderManager::SHADER_TERRAIN_CDLOD				= "Shaders/ShaderTerrainCDLOD";
const std::string ShaderManager::SHADER_TERRAIN_CDLOD_DEPTH_ONLY	= "Shaders/ShaderTerrainCDLODDepthOnly";
const std::string ShaderManager::SHADER_SKY							= "Shaders/ShaderSky";
const std::string ShaderManager::SHADER_OUTPUT_LINEAR_DEPTH			= "Shaders/ShaderOutputLinearDepth";
const std::string ShaderManager::SHADER_GENERATE_FOG_BUFFER			= "Shaders/ShaderGenerateFogBuffer";
const std::string ShaderManager::SHADER_MERGE_FOG_BUFFER			= "Shaders/ShaderMergeFogBuffer";

static const std::string SHADER_VERTEX_EXTENSION	= ".vp";
static const std::string SHADER_FRAGMENT_EXTENSION	= ".fp";
}

using namespace RendererLib;

ShaderManager::ShaderManager()
{
	using namespace std;

	ShaderProgram* currentProgram;

    currentProgram = new ShaderProgram(new Shader(SHADER_DEFAULT_MESH + SHADER_VERTEX_EXTENSION, GL_VERTEX_SHADER),
									   new Shader(SHADER_DEFAULT_MESH + SHADER_FRAGMENT_EXTENSION, GL_FRAGMENT_SHADER));
	currentProgram->Link();
	programMap.insert(pair<string, ShaderProgram*>(SHADER_DEFAULT_MESH, currentProgram));

	currentProgram = new ShaderProgram(new Shader(SHADER_STATIC_MESH_SHADOW + SHADER_VERTEX_EXTENSION, GL_VERTEX_SHADER), 
									   new Shader(SHADER_STATIC_MESH_SHADOW + SHADER_FRAGMENT_EXTENSION, GL_FRAGMENT_SHADER));
	currentProgram->Link();
	programMap.insert(pair<string, ShaderProgram*>(SHADER_STATIC_MESH_SHADOW, currentProgram));

	currentProgram = new ShaderProgram(new Shader(SHADER_DEPTH_ONLY + SHADER_VERTEX_EXTENSION, GL_VERTEX_SHADER), 
									   new Shader(SHADER_DEPTH_ONLY + SHADER_FRAGMENT_EXTENSION, GL_FRAGMENT_SHADER));
	currentProgram->Link();
	programMap.insert(pair<string, ShaderProgram*>(SHADER_DEPTH_ONLY, currentProgram));

	currentProgram = new ShaderProgram(new Shader(SHADER_SINGLE_COLOR + SHADER_VERTEX_EXTENSION, GL_VERTEX_SHADER),
									   new Shader(SHADER_SINGLE_COLOR + SHADER_FRAGMENT_EXTENSION, GL_FRAGMENT_SHADER));
	currentProgram->Link();
	programMap.insert(pair<string, ShaderProgram*>(SHADER_SINGLE_COLOR, currentProgram));

	currentProgram = new ShaderProgram(new Shader(SHADER_FULLSCREEN_QUAD + SHADER_VERTEX_EXTENSION, GL_VERTEX_SHADER),
									   new Shader(SHADER_FULLSCREEN_QUAD + SHADER_FRAGMENT_EXTENSION, GL_FRAGMENT_SHADER));
	currentProgram->Link();
	programMap.insert(pair<string, ShaderProgram*>(SHADER_FULLSCREEN_QUAD, currentProgram));

    currentProgram = new ShaderProgram(new Shader(SHADER_DEPTH_TRANSFER + SHADER_VERTEX_EXTENSION, GL_VERTEX_SHADER),
                                       new Shader(SHADER_DEPTH_TRANSFER + SHADER_FRAGMENT_EXTENSION, GL_FRAGMENT_SHADER));
    currentProgram->Link();
    programMap.insert(pair<string, ShaderProgram*>(SHADER_DEPTH_TRANSFER, currentProgram));

    currentProgram = new ShaderProgram(new Shader(SHADER_DIRECTIONAL_LIGHT + SHADER_VERTEX_EXTENSION, GL_VERTEX_SHADER),
									   new Shader(SHADER_DIRECTIONAL_LIGHT + SHADER_FRAGMENT_EXTENSION, GL_FRAGMENT_SHADER));
	currentProgram->Link();
	programMap.insert(pair<string, ShaderProgram*>(SHADER_DIRECTIONAL_LIGHT, currentProgram));

	currentProgram = new ShaderProgram(new Shader(SHADER_FXAA + SHADER_VERTEX_EXTENSION, GL_VERTEX_SHADER),
									   new Shader(SHADER_FXAA + SHADER_FRAGMENT_EXTENSION, GL_FRAGMENT_SHADER));
	currentProgram->Link();
	programMap.insert(pair<string, ShaderProgram*>(SHADER_FXAA, currentProgram));

	currentProgram = new ShaderProgram(new Shader(SHADER_TERRAIN_NAIVE + SHADER_VERTEX_EXTENSION, GL_VERTEX_SHADER),
									   new Shader(SHADER_TERRAIN_NAIVE + SHADER_FRAGMENT_EXTENSION, GL_FRAGMENT_SHADER));
	currentProgram->Link();
	programMap.insert(pair<string, ShaderProgram*>(SHADER_TERRAIN_NAIVE, currentProgram));

	currentProgram = new ShaderProgram(new Shader(SHADER_TERRAIN_CDLOD + SHADER_VERTEX_EXTENSION, GL_VERTEX_SHADER),
									   new Shader(SHADER_TERRAIN_CDLOD + SHADER_FRAGMENT_EXTENSION, GL_FRAGMENT_SHADER));
	currentProgram->Link();
	programMap.insert(pair<string, ShaderProgram*>(SHADER_TERRAIN_CDLOD, currentProgram));

	currentProgram = new ShaderProgram(new Shader(SHADER_TERRAIN_CDLOD_DEPTH_ONLY + SHADER_VERTEX_EXTENSION, GL_VERTEX_SHADER),
									   new Shader(SHADER_TERRAIN_CDLOD_DEPTH_ONLY + SHADER_FRAGMENT_EXTENSION, GL_FRAGMENT_SHADER));
	currentProgram->Link();
	programMap.insert(pair<string, ShaderProgram*>(SHADER_TERRAIN_CDLOD_DEPTH_ONLY, currentProgram));

	currentProgram = new ShaderProgram(new Shader(SHADER_SKY + SHADER_VERTEX_EXTENSION, GL_VERTEX_SHADER),
									   new Shader(SHADER_SKY + SHADER_FRAGMENT_EXTENSION, GL_FRAGMENT_SHADER));
	currentProgram->Link();
	programMap.insert(pair<string, ShaderProgram*>(SHADER_SKY, currentProgram));

	currentProgram = new ShaderProgram(new Shader(SHADER_OUTPUT_LINEAR_DEPTH + SHADER_VERTEX_EXTENSION, GL_VERTEX_SHADER),
									   new Shader(SHADER_OUTPUT_LINEAR_DEPTH + SHADER_FRAGMENT_EXTENSION, GL_FRAGMENT_SHADER));
	currentProgram->Link();
	programMap.insert(pair<string, ShaderProgram*>(SHADER_OUTPUT_LINEAR_DEPTH, currentProgram));

	currentProgram = new ShaderProgram(new Shader(SHADER_GENERATE_FOG_BUFFER + SHADER_VERTEX_EXTENSION, GL_VERTEX_SHADER),
									   new Shader(SHADER_GENERATE_FOG_BUFFER + SHADER_FRAGMENT_EXTENSION, GL_FRAGMENT_SHADER));
	currentProgram->Link();
	programMap.insert(pair<string, ShaderProgram*>(SHADER_GENERATE_FOG_BUFFER, currentProgram));
	
	currentProgram = new ShaderProgram(new Shader(SHADER_MERGE_FOG_BUFFER + SHADER_VERTEX_EXTENSION, GL_VERTEX_SHADER),
									   new Shader(SHADER_MERGE_FOG_BUFFER + SHADER_FRAGMENT_EXTENSION, GL_FRAGMENT_SHADER));
	currentProgram->Link();
	programMap.insert(pair<string, ShaderProgram*>(SHADER_MERGE_FOG_BUFFER, currentProgram));
}

ShaderProgram* ShaderManager::GetShader(std::string name)
{
	using namespace std;

	map<string, ShaderProgram*>::const_iterator program = programMap.find(name);
	if (programMap.end() != program)
	{
		return program->second;
	}

	return NULL;
}

ShaderManager::~ShaderManager()
{
	using namespace std;

	map<string, ShaderProgram*>::const_iterator currentProgram = programMap.begin();
	while (currentProgram != programMap.end())
	{
		delete currentProgram->second;

		currentProgram++;
	}
	programMap.clear();
}

ShaderManager& ShaderManager::GetInstance()
{
	if (NULL == manager)
	{
		manager = new ShaderManager();
	}

	return *manager;
}
