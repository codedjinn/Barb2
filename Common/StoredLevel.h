#ifndef STOREDLEVEL_H_INCLUDED
#define STOREDLEVEL_H_INCLUDED

#include "CommonTypes.h"
#include <string>
#include <tinyxml.h>
#include <stdio.h>
#include <vector>

class StoredEntity;
class StoredTerrainLandscape;

class StoredLevel
{
	public:

		/// <summary>
		/// Default constructor.
		/// </summary>
		StoredLevel();

		/// <summary>
		/// Adds a specified terrain landscape to this level.
		/// </summary>
		void AddStoredTerrainLandsacpe(StoredTerrainLandscape* landscape);

		/// <summary>
		/// Removes a specified terrain landscape from this level.
		/// </summary>
		void RemoveStoredTerrainLandscape(StoredTerrainLandscape* landscape);

		/// <summary>
		/// Adds a stored entity to the domain of this level.
		/// </summary.
		void AddStoredEntity(StoredEntity* entity);

		/// <summary>
		/// Removes a stored entity from the domain of this level.
		/// </summary>
		void RemoveStoredEntity(StoredEntity* entity);

		/// <summary>
		/// Saves the current stored level to a specified directory.
		/// </summary>
		void Save(const std::string& directoryName, const std::string& fileName);

		/// <summary>
		/// Loads a stored level from a specified directory.
		/// </summary>
		void Load(const std::string& directoryName, const std::string& fileName);

		/// <summary>
		/// Set the level name.
		/// </summary>
		void SetLevelName(const std::string& value);

		/// <summary>
		/// Returns the level name.
		/// </summary>
		const std::string& GetLevelName();

		/// <summary>
		/// Returns a list of stored terrain landscapes that comprise this level.
		/// </summary>
		const std::vector<StoredTerrainLandscape*>& GetStoredTerrainLandscapes();

		/// <summary>
		/// Returns a list of stored entities that comprise this level.
		/// </summary>
        const std::vector<StoredEntity*>& GetStoredEntities();

		/// <summary>
		/// Resets the stored level to a blank state.
		/// </summary.
        void Reset();

	protected:

        std::vector<StoredTerrainLandscape*> m_TerrainList;
        std::vector<StoredEntity*> m_EntityList;

		std::string m_LevelName;

		/// <summary>
		/// Sub method that saves the terrain landscapes.
		/// </summary>
		void SaveTerrainLandscapes(const std::string& directoryName, TiXmlElement* parent);

		/// <summary>
		/// Loads the terrain landscapes.
		/// </summary>
		void LoadTerrainLandscapes(const std::string& fileName);

		/// <summary>
		/// Sub method that saves the entities.
		/// </summary.
		void SaveEntities(const std::string& directoryName, TiXmlElement* parent);

		/// <summary>
		/// Loads the entities.
		/// </summary>
		void LoadEntities(const std::string& fileName);
};

#endif // STOREDLEVEL_H_INCLUDED
