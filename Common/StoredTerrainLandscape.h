#ifndef STOREDTERRAINLANDSCAPE_H_INCLUDED
#define STOREDTERRAINLANDSCAPE_H_INCLUDED

#include <inttypes.h>
#include <cstring>
#include <string>
#include <stdio.h>
#include "AttributedBinaryBlob.h"

class StoredTerrainLandscape
{
	public:

		uint32_t terrainLandscapeSize;			/// The dimension of the heightmap.

		uint32_t splatTextureSize;				/// The dimension of the splat texture.

		uint32_t numBinaryBlobs;
		AttributedBinaryBlob* blobArray;		/// The AttributedBinaryBlob array contains the actual core data of the terrain.

		StoredTerrainLandscape();
		~StoredTerrainLandscape();

		void SetValues(unsigned int terrainLandscapeSize, float* heightData,
					   unsigned int splatTextureDimension,
					   uint8_t* splatTexture0Data,
					   uint8_t* splatTexture1Data,
					   const std::string& diffuse0Path,
					   const std::string& normal0Path,
					   const std::string& diffuse1Path,
					   const std::string& normal1Path,
					   const std::string& diffuse2Path,
					   const std::string& normal2Path,
					   const std::string& diffuse3Path,
					   const std::string& normal3Path,
					   const std::string& diffuse4Path,
					   const std::string& normal4Path,
					   const std::string& diffuse5Path,
					   const std::string& normal5Path,
					   const std::string& diffuse6Path,
					   const std::string& normal6Path,
					   const std::string& diffuse7Path,
					   const std::string& normal7Path);
		/// Sets the terrain attributes. This creates a copy of the height data so you
		/// don't need to worry about it being freed unintentionally.
		/// @note This will free the currently allocated memory for this object.

		float* ReturnHeightData() const;
		/// Convenience method to, if possible, return the height data attribute for this
		/// StoredTerrainLandscape instance. Takes care of all decompression and memory
		/// allocation for you.

		uint8_t* ReturnSplatTexture0Data() const;
		/// Convenience method to, if possible, return the splat texture 0 data attribute for this
		/// StoredTerrainLandscape instance. Takes care of all decompression and memory
		/// allocation for you.

		uint8_t* ReturnSplatTexture1Data() const;
		/// Convenience method to, if possible, return the splat texture 1 data attribute for this
		/// StoredTerrainLandscape instance. Takes care of all decompression and memory
		/// allocation for you.

		const std::string ReturnDiffuse0Path() const;
		/// Convenience method to, if possible, return the splat texture 0 diffuse identifier for this
		/// StoredTerrainLandscape instance. Takes care of all decompression and memory
		/// allocation for you.

		const std::string ReturnNormal0Path() const;
		/// Convenience method to, if possible, return the splat texture 0 normal identifier for this
		/// StoredTerrainLandscape instance. Takes care of all decompression and memory
		/// allocation for you.

		const std::string ReturnDiffuse1Path() const;
		/// Convenience method to, if possible, return the splat texture 1 diffuse identifier for this
		/// StoredTerrainLandscape instance. Takes care of all decompression and memory
		/// allocation for you.

		const std::string ReturnNormal1Path() const;
		/// Convenience method to, if possible, return the splat texture 1 normal identifier for this
		/// StoredTerrainLandscape instance. Takes care of all decompression and memory
		/// allocation for you.

		const std::string ReturnDiffuse2Path() const;
		/// Convenience method to, if possible, return the splat texture 2 diffuse identifier for this
		/// StoredTerrainLandscape instance. Takes care of all decompression and memory
		/// allocation for you.

		const std::string ReturnNormal2Path() const;
		/// Convenience method to, if possible, return the splat texture 2 normal identifier for this
		/// StoredTerrainLandscape instance. Takes care of all decompression and memory
		/// allocation for you.

		const std::string ReturnDiffuse3Path() const;
		/// Convenience method to, if possible, return the splat texture 3 diffuse identifier for this
		/// StoredTerrainLandscape instance. Takes care of all decompression and memory
		/// allocation for you.

		const std::string ReturnNormal3Path() const;
		/// Convenience method to, if possible, return the splat texture 3 normal identifier for this
		/// StoredTerrainLandscape instance. Takes care of all decompression and memory
		/// allocation for you.

		const std::string ReturnDiffuse4Path() const;
		/// Convenience method to, if possible, return the splat texture 4 diffuse identifier for this
		/// StoredTerrainLandscape instance. Takes care of all decompression and memory
		/// allocation for you.

		const std::string ReturnNormal4Path() const;
		/// Convenience method to, if possible, return the splat texture 4 normal identifier for this
		/// StoredTerrainLandscape instance. Takes care of all decompression and memory
		/// allocation for you.

		const std::string ReturnDiffuse5Path() const;
		/// Convenience method to, if possible, return the splat texture 5 diffuse identifier for this
		/// StoredTerrainLandscape instance. Takes care of all decompression and memory
		/// allocation for you.

		const std::string ReturnNormal5Path() const;
		/// Convenience method to, if possible, return the splat texture 5 normal identifier for this
		/// StoredTerrainLandscape instance. Takes care of all decompression and memory
		/// allocation for you.

		const std::string ReturnDiffuse6Path() const;
		/// Convenience method to, if possible, return the splat texture 6 diffuse identifier for this
		/// StoredTerrainLandscape instance. Takes care of all decompression and memory
		/// allocation for you.

		const std::string ReturnNormal6Path() const;
		/// Convenience method to, if possible, return the splat texture 6 normal identifier for this
		/// StoredTerrainLandscape instance. Takes care of all decompression and memory
		/// allocation for you.

		const std::string ReturnDiffuse7Path() const;
		/// Convenience method to, if possible, return the splat texture 7 diffuse identifier for this
		/// StoredTerrainLandscape instance. Takes care of all decompression and memory
		/// allocation for you.

		const std::string ReturnNormal7Path() const;
		/// Convenience method to, if possible, return the splat texture 7 normal identifier for this
		/// StoredTerrainLandscape instance. Takes care of all decompression and memory
		/// allocation for you.

		void Save(FILE* fp) const;
		void Load(FILE* fp);

	protected:

		uint32_t GetSizeOnDisk() const;
		/// Convenience method to return the amount of space that this file will
		/// consume on disk, in its current format.

		void FreeMemory();

		const static unsigned int totalAttributes;
};

#endif // STOREDTERRAINLANDSCAPE_H_INCLUDED
