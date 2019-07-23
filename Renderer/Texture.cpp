#include "Texture.h"
#include <assert.h>
#include <inttypes.h>
#include <vector>
#include <StoredTexture.h>
#include <cstring>
#include <stddef.h>
#include <Compression.h>
#include <iostream>

namespace RendererLib
{
TextureManager* TextureManager::textureManager = NULL;
}

using namespace RendererLib;

TextureManager::TextureManager()
{
	textureDatabase = NULL;
}

TextureManager::~TextureManager()
{
	FreeTextures();

	if (NULL != textureDatabase)
	{
		fclose(textureDatabase);
	}
}

void TextureManager::FreeTextures()
{
	using namespace std;

	// Free all allocated resources
	map<string, uint32_t>::const_iterator currentTexture = residentTexturesIndex.begin();
	while (currentTexture != residentTexturesIndex.end())
	{
		GLuint currentTextureHandle = currentTexture->second;
		glDeleteTextures(1, &currentTextureHandle);

		currentTexture++;
	}
	residentTexturesIndex.clear();
}

void TextureManager::CloseTextureDatabase()
{
	if (NULL != textureDatabase)
	{
		fclose(textureDatabase);
		textureDatabase = NULL;
	}
}

void TextureManager::SetTextureDatabase(FILE* file)
{
	assert(NULL != file);

	textureDatabase = file;

	databaseIndex.Load(textureDatabase);
	databaseIndex.PrintIndex();
}

static GLuint GenAndReturnNullTexture()
{
	static GLuint textureHandle = 0;

	if (!glIsTexture(textureHandle))
	{
		// Generate new texture.
		glGenTextures(1, &textureHandle);
		glBindTexture(GL_TEXTURE_2D, textureHandle);

		glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP_SGIS, GL_TRUE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

		uint32_t textureBuffer[] = {0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff};
		glTexImage2D(GL_TEXTURE_2D,
					 0,
					 GL_COMPRESSED_RGBA,
					 2, 2,
					 0,
					 GL_RGBA,
					 GL_UNSIGNED_BYTE,
					 textureBuffer);
	}

	assert(glIsTexture(textureHandle));
	return textureHandle;
}

GLuint TextureManager::GetTexture(const std::string& textureIdentifier)
{
	using namespace std;

    if (NULL == textureDatabase)
    {
		return GenAndReturnNullTexture();
    }

	// First, determine if the texture is resident.
	map<string, GLuint>::const_iterator residentIndexIterator = residentTexturesIndex.find(textureIdentifier);
	if (residentTexturesIndex.end() != residentIndexIterator)
	{
		// The texture is resident, return it.
		return residentIndexIterator->second;
	}

	int offset = databaseIndex.OffsetOfTexture(textureIdentifier);
	if (-1 == offset)
	{
		return GenAndReturnNullTexture();
	}
	else
	{
		// We have the offset, we can now load the texture from HDD, and transfer it to the GPU.
		// First, set the file offset and load the texture.
		fseek(textureDatabase,
			  static_cast<long>(offset),
			  SEEK_SET);

		StoredTexture storedTexture;
		storedTexture.Load(textureDatabase);

		// Transfer texture to the GPU, get the returned handle.
		GLuint handle = StoredTextureToGPU(storedTexture);

		// Now add the identifier and the handle to the resident textures list.
		pair<string, GLuint> newResidentTexture(storedTexture.identifier, handle);
		residentTexturesIndex.insert(newResidentTexture);

		return handle;
	}
}

GLuint TextureManager::StoredTextureToGPU(const StoredTexture& storedTexture)
{
	GLuint textureHandle;

	glGenTextures(1, &textureHandle);

	glBindTexture(GL_TEXTURE_2D, textureHandle);

	glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP_SGIS, GL_TRUE);				// For compressed mipmaps.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	uint32_t* rgbaBuffer = new uint32_t[storedTexture.width * storedTexture.height];
	if (storedTexture.formatFlag & ARGB32_RAW)
	{
		BufferConversionARGB32ToRGBA32(storedTexture.width, storedTexture.height, reinterpret_cast<uint32_t*>(storedTexture.data), rgbaBuffer);
	}
	else if (storedTexture.formatFlag & ARGB32_COMPRESSED)
	{
		// Decompress texture.
		uint8_t* decompressedBuffer = NULL;
		unsigned long int decompressedBufferLength = 0;

		std::cout << "Decompressing : " << storedTexture.identifier << std::endl;

		CompressionLib::Compression::DecompressByteArray(storedTexture.data, storedTexture.dataSizeBytes, &decompressedBuffer, &decompressedBufferLength);

		std::cout << "Done : " << storedTexture.identifier << std::endl;

		BufferConversionARGB32ToRGBA32(storedTexture.width, storedTexture.height, reinterpret_cast<uint32_t*>(decompressedBuffer), rgbaBuffer);

		delete [] decompressedBuffer;
	}

	// Flip texture.
	uint32_t* flippedBuffer = new uint32_t[storedTexture.width * storedTexture.height];
	for (unsigned int y = 0; y < storedTexture.height; y++)
	{
		for (unsigned int x = 0; x < storedTexture.width; x++)
		{
			unsigned int currentIndex = x + (y * storedTexture.width);
			unsigned int flippedIndex = x + ((storedTexture.height - 1 - y) * storedTexture.width);
			flippedBuffer[flippedIndex] = rgbaBuffer[currentIndex];
		}
	}

	// The texture was stored uncompressed, (which isn't wise).
	glTexImage2D(GL_TEXTURE_2D,
				 0, 					// First mip level.
				 GL_COMPRESSED_RGBA,	// Compress the texture data before storing.
				 storedTexture.width,
				 storedTexture.height,
				 0, 					// No border.
				 GL_RGBA, 				// Texture format.
				 GL_UNSIGNED_BYTE, 		// Data type.
                 flippedBuffer);        // The data.

	delete [] rgbaBuffer;
	delete [] flippedBuffer;

	// Determine if the texture was compressed successfully or not
	GLint compressed;
	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_COMPRESSED, &compressed);
	if (GL_TRUE != compressed)
	{
		printf("Texture Manager::[%s] loaded uncompressed!\n", storedTexture.identifier);
	}

	assert(glIsTexture(textureHandle));

	return textureHandle;
}

void TextureManager::BufferConversionARGB32ToRGBA32(unsigned int width, unsigned int height, uint32_t* argbBuffer, uint32_t* rgbaBuffer)
{
	for (unsigned int y = 0; y < height; y++)
	{
		for (unsigned int x = 0; x < width; x++)
		{
			unsigned int currentIndex = x + (y * width);
			uint32_t currentPixel = argbBuffer[currentIndex];		// AARRGGBB.
			uint8_t aComponent = (currentPixel & 0xff000000) >> 24;
			uint8_t rComponent = (currentPixel & 0x00ff0000) >> 16;
			uint8_t gComponent = (currentPixel & 0x0000ff00) >> 8;
			uint8_t bComponent = (currentPixel & 0x000000ff) >> 0;

			// Apparently, when OpenGL means RGBA it actually means AABBGGRR...
			rgbaBuffer[currentIndex] = (rComponent << 0) | (gComponent << 8) | (bComponent << 16) | (aComponent << 24);
		}
	}
}

TextureManager& TextureManager::GetInstance()
{
	if (NULL == textureManager)
	{
		textureManager = new TextureManager;

		assert(NULL != textureManager);
	}

	return *textureManager;
}
