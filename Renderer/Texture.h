#ifndef TEXTURE_H_INCLUDED
#define TEXTURE_H_INCLUDED

#include <string>
#include <map>
#include <stdio.h>
#include "RendererLibCommon.h"
#include "TextureDBIndex.h"

class StoredTexture;

namespace RendererLib
{

class TextureManager
{
	public:

		virtual ~TextureManager();

		/**
		* Retrieves the texture handle corresponding to the texture identifier.
		* @warning If the texture identifier is invalid, then the blank texture is returned.
		*/
		GLuint GetTexture(const std::string& textureIdentifier);

		void SetTextureDatabase(FILE* file);

		void CloseTextureDatabase();

		/**
		* Frees all resources used by this texture managers resident textures.
		*/
		void FreeTextures();

		const TextureDBIndex& GetDatabaseIndex() { return databaseIndex; }

		static TextureManager& GetInstance();

	protected:

		FILE* textureDatabase;

		TextureDBIndex databaseIndex;

		std::map<std::string, GLuint> residentTexturesIndex;

		static TextureManager* textureManager;

		TextureManager();

		GLuint StoredTextureToGPU(const StoredTexture& storedTexture);
		void BufferConversionARGB32ToRGBA32(unsigned int width, unsigned int height, uint32_t* argbBuffer, uint32_t* rgbaBuffer);
};

}

#endif // TEXTURE_H_INCLUDED
