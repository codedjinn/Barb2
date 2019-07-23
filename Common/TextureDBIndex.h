#ifndef TEXTUREDBINDEX_H_INCLUDED
#define TEXTUREDBINDEX_H_INCLUDED

#include <inttypes.h>
#include <map>
#include <string>
#include <vector>

class TextureDBIndex
{
	public:

		TextureDBIndex();

		~TextureDBIndex();

		/**
		* Returns the offset of the texture into the texture database.
		* @warning If the identifier is invalid, -1 is returned.
		*/
		int OffsetOfTexture(const std::string& identifier);

		/**
		* Load the index data from a file.
		*/
		void Load(FILE* file);

		/**
		* Debugging method, prints the contents of this index to the console.
		*/
		void PrintIndex() const;

		std::vector<std::string> GetTextureList() const;

	protected:

		std::map<std::string, uint32_t> index;
};

#endif // TEXTUREDBINDEX_H_INCLUDED
