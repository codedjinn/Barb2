#ifndef STATICMESHDBINDEX_H_INCLUDED
#define STATICMESHDBINDEX_H_INCLUDED

#include <inttypes.h>
#include <vector>
#include <map>
#include <string>

class StaticMeshDBIndex
{
	public:

		StaticMeshDBIndex();

		~StaticMeshDBIndex();

		/**
		* Returns the offset of the specified mesh into the mesh database file.
		* @warning If the identifier is invalid, -1 is returned.
		*/
		int OffsetOfMesh(const std::string& identifier) const;

		/**
		* Load the index data from a file.
		*/
		void Load(FILE* file);

		/**
		* Returns a list of all the meshes in the database.
		*/
		std::vector<std::string> GetDatabaseContents() const;

		/**
		* Debugging method, prints the contents of the index to the console.
		*/
		void PrintIndex() const;

	protected:

		std::map<std::string, uint32_t> index;
};

#endif // STATICMESHDBINDEX_H_INCLUDED
