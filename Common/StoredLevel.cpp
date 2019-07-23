#include "StoredLevel.h"
#include "StoredEntity.h"
#include "StoredTerrainLandscape.h"
#include "FileSystem.h"
#include <algorithm>
#include <strstream>
#include <iostream>
#include <sstream>

using namespace std;

static const char* ELEMENT_NAME_ROOT							= "Map";
static const char* ELEMENT_NAME_TERRAIN_LANDSCAPES				= "TerrainLandscapes";
static const char* ELEMENT_NAME_TERRAIN_LANDSCAPE_FILE			= "TerrainLandscapeFile";
static const char* ELEMENT_NAME_LEVEL_NAME						= "LevelName";
static const char* ELEMENT_NAME_EDITOR_PATH_TO_TEXTURE_DB		= "EditorPathToTextureDB";
static const char* ELEMENT_NAME_EDITOR_PATH_TO_STATIC_MESH_DB	= "EditorPathToStaticMeshDB";
static const char* ELEMENT_NAME_ENTITIES						= "Entities";

StoredLevel::StoredLevel()
{
	Reset();
}

void StoredLevel::AddStoredTerrainLandsacpe(StoredTerrainLandscape* landscape)
{
    auto it = find(m_TerrainList.begin(), m_TerrainList.end(), landscape);
    if (it == m_TerrainList.end())
    {
        m_TerrainList.push_back(landscape);
    }
}

void StoredLevel::RemoveStoredTerrainLandscape(StoredTerrainLandscape* landscape)
{
    auto it = find(m_TerrainList.begin(), m_TerrainList.end(), landscape);
    if (it != m_TerrainList.end())
    {
        m_TerrainList.erase(it);
    }
}

void StoredLevel::AddStoredEntity(StoredEntity* entity)
{
    auto it = find(m_EntityList.begin(), m_EntityList.end(), entity);
    if (it == m_EntityList.end())
    {
        m_EntityList.push_back(entity);
    }
}

void StoredLevel::RemoveStoredEntity(StoredEntity* entity)
{
    auto it = find(m_EntityList.begin(), m_EntityList.end(), entity);
    if (it != m_EntityList.end())
    {
        m_EntityList.erase(it);
    }
}

void StoredLevel::Reset()
{
	// Clear terrain list.
	for (int i = 0; i < m_TerrainList.size(); i++)
		delete m_TerrainList[i];
	m_TerrainList.clear();

	// Clear entity list.
	for (int i = 0; i < m_EntityList.size(); i++)
		delete m_EntityList[i];
	m_EntityList.clear();

	m_LevelName = "";
}

void StoredLevel::SetLevelName(const std::string& value)
{
	m_LevelName = value;
}

const std::string& StoredLevel::GetLevelName()
{
	return m_LevelName;
}

const vector<StoredTerrainLandscape*>& StoredLevel::GetStoredTerrainLandscapes()
{ 
	return m_TerrainList;
}

const vector<StoredEntity*>& StoredLevel::GetStoredEntities()
{
	return m_EntityList;
}

void StoredLevel::Save(const std::string& directoryName, const std::string& fileName)
{
	TiXmlDocument document;

	TiXmlDeclaration* declaration = new TiXmlDeclaration("1.0", "", "");
	document.LinkEndChild(declaration);

	TiXmlElement* root = new TiXmlElement(ELEMENT_NAME_ROOT);

	// Level name element.
	{
		TiXmlElement* levelNameElement = new TiXmlElement(ELEMENT_NAME_LEVEL_NAME);
		TiXmlText* levelNameValue = new TiXmlText(m_LevelName.c_str());
		levelNameElement->LinkEndChild(levelNameValue);
		root->LinkEndChild(levelNameElement);
	}

	SaveTerrainLandscapes(directoryName, root);
	SaveEntities(directoryName, root);

	document.LinkEndChild(root);

	document.SaveFile((directoryName + "\\" + fileName).c_str());
}

void StoredLevel::Load(const std::string& directoryName, const std::string& fileName)
{
	Reset();

	TiXmlDocument document;
	document.LoadFile((directoryName + "\\" + fileName).c_str());

	TiXmlElement* rootElement = document.FirstChildElement();
	for (TiXmlElement* currentElement = rootElement->FirstChildElement(); currentElement != NULL; currentElement = currentElement->NextSiblingElement())
	{
		string elementName = currentElement->Value();
		if (ELEMENT_NAME_LEVEL_NAME == elementName)
		{
			TiXmlText* text = currentElement->FirstChild()->ToText();

			if (NULL != text)
				SetLevelName(text->Value());
		}
		else if (ELEMENT_NAME_TERRAIN_LANDSCAPES == elementName)
		{
			for (TiXmlElement* currentTerrainElement = currentElement->FirstChildElement(); currentTerrainElement != NULL; currentTerrainElement = currentTerrainElement->NextSiblingElement())
			{
				TiXmlText* text = currentTerrainElement->FirstChild()->ToText();
				LoadTerrainLandscapes(directoryName + text->Value());
			}
		}
		else if (ELEMENT_NAME_ENTITIES == elementName)
		{
			TiXmlText* text = currentElement->FirstChild()->ToText();
			if (NULL != text)				
				LoadEntities(directoryName + text->Value());
		}
	}
}

void StoredLevel::SaveTerrainLandscapes(const std::string& directoryName, TiXmlElement* parent)
{
	TiXmlElement* landscapeElement = new TiXmlElement(ELEMENT_NAME_TERRAIN_LANDSCAPES);
	for (int i = 0; i < m_TerrainList.size(); i++)
	{
		std::ostrstream stringStream;

		string dir = directoryName;
		dir.append("\\Terrain");

		using namespace CommonCode;
		if (FileSystem::CreateDir(dir))
		{
			std::cerr << dir << std::endl;
		}

		stringStream << "\\Terrain\\terrain" << i << ".tlf" << '\0';

		char* stringStreamCharArray = stringStream.str();

		std::string fullPath = directoryName + stringStreamCharArray;

		FILE* newFile = fopen(fullPath.c_str(), "wb");
		m_TerrainList[i]->Save(newFile);
		fclose(newFile);

		TiXmlElement* currentLandscapeElement = new TiXmlElement(ELEMENT_NAME_TERRAIN_LANDSCAPE_FILE);
		TiXmlText* currentLandscapeElementText = new TiXmlText(stringStreamCharArray);
		currentLandscapeElement->LinkEndChild(currentLandscapeElementText);
		landscapeElement->LinkEndChild(currentLandscapeElement);

		stringStream.freeze(false);
	}
	parent->LinkEndChild(landscapeElement);
}

void StoredLevel::LoadTerrainLandscapes(const std::string& fileName)
{
	StoredTerrainLandscape* newLandscape = new StoredTerrainLandscape();

	FILE* fp = fopen(fileName.c_str(), "rb");
	newLandscape->Load(fp);
	fclose(fp);

	AddStoredTerrainLandsacpe(newLandscape);
}

void StoredLevel::SaveEntities(const std::string& directoryName, TiXmlElement* parent)
{
	TiXmlElement* entityElement = new TiXmlElement(ELEMENT_NAME_ENTITIES);
	TiXmlText* entityFolderText = new TiXmlText("\\Entities\\entities.ent");
	entityElement->LinkEndChild(entityFolderText);
	parent->LinkEndChild(entityElement);

	using namespace CommonCode;
	string dir = directoryName;
	dir.append("\\Entities");
	if (!FileSystem::CreateDir(dir))
	{
		std::cerr << dir << std::endl;
	}
	
	std::ostringstream stringStream(dir.c_str());

	std::string entitiesFileName = stringStream.str() + "\\entities.ent";

	FILE* newFile = fopen(entitiesFileName.c_str(), "wb");
	
	// First, write the number of entities to the saved level.
	uint32_t numEntities = m_EntityList.size();
	fwrite(&numEntities, sizeof(numEntities), 1, newFile);

	for (int i = 0; i < m_EntityList.size(); i++)
		m_EntityList[i]->Save(newFile);
}

void StoredLevel::LoadEntities(const std::string& fileName)
{
	FILE* fp;
	fopen_s(&fp, fileName.c_str(), "rb");
	
	uint32_t numEntities;
	fread(&numEntities, sizeof(numEntities), 1, fp);

	for (uint32_t i = 0; i < numEntities; i++)
	{
		StoredEntity* newEntity = new StoredEntity;
		newEntity->Load(fp);
		
		AddStoredEntity(newEntity);
	}

	fclose(fp);
}