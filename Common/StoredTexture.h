#ifndef STOREDTEXTURE_H_INCLUDED
#define STOREDTEXTURE_H_INCLUDED

#include <inttypes.h>
#include <stdio.h>

enum TextureFormatFlag
{
    ARGB32_RAW          = 0,            // (0xAARRGGBB) Uncompressed.
    ARGB32_COMPRESSED   = 1, 			// (0xAARRGGBB) Compressed. Need to decompress before usage.
    GAMMA_CORRECTED		= 2
};

/**
* StoredTexture:
* Interchange format between runtime engine and file system.
*/
class StoredTexture
{
	public:

		uint32_t identifierLength;
		char* identifier;

		uint32_t formatFlag;

		uint32_t width;
		uint32_t height;

		uint32_t dataSizeBytes;
        uint8_t* data;

		StoredTexture();

		~StoredTexture();

		void ApplyGammaCorrection(float gammaFactor);

		/**
		* Convenience method to return the amount of space that this file will
		* consume on disk, in it's current format.
		*/
		uint32_t SizeOnDisk() const;

		void Save(FILE* fp) const;
		void Load(FILE* fp);

		/**
		* Convenience method to load just the identifier from a file.
		*/
		void LoadIdentifier(FILE* fp);
};

#endif // STOREDTEXTURE_H_INCLUDED
