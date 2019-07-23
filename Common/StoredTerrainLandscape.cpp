#include "StoredTerrainLandscape.h"
#include <Compression.h>
#include <cassert>
#include <iostream>

enum StoredTerrainAttribute
{
	TerrainHeightData		= 0,
	TerrainSplat0Data		= 1,
	TerrainSplat1Data		= 2,
	TerrainDiffuse0Data		= 3,
	TerrainNormal0Data		= 4,
	TerrainDiffuse1Data		= 5,
	TerrainNormal1Data		= 6,
	TerrainDiffuse2Data 	= 7,
	TerrainNormal2Data		= 8,
	TerrainDiffuse3Data		= 9,
	TerrainNormal3Data		= 10,
	TerrainDiffuse4Data		= 11,
	TerrainNormal4Data		= 12,
	TerrainDiffuse5Data		= 13,
	TerrainNormal5Data		= 14,
	TerrainDiffuse6Data		= 15,
	TerrainNormal6Data		= 16,
	TerrainDiffuse7Data		= 17,
	TerrainNormal7Data		= 18
};

const unsigned int StoredTerrainLandscape::totalAttributes = 19;

StoredTerrainLandscape::StoredTerrainLandscape()
{
	terrainLandscapeSize = 0;

	numBinaryBlobs = 0;
	blobArray = NULL;
}

StoredTerrainLandscape::~StoredTerrainLandscape()
{
	FreeMemory();
}

void StoredTerrainLandscape::Save(FILE* fp) const
{
	fwrite(&terrainLandscapeSize, sizeof(terrainLandscapeSize), 1, fp);
	fwrite(&splatTextureSize, sizeof(splatTextureSize), 1, fp);
	fwrite(&numBinaryBlobs, sizeof(numBinaryBlobs), 1, fp);
	for (uint32_t i = 0; i < numBinaryBlobs; i++)
	{
		blobArray[i].Save(fp);
	}
}

void StoredTerrainLandscape::Load(FILE* fp)
{
	fread(&terrainLandscapeSize, sizeof(terrainLandscapeSize), 1, fp);
	fread(&splatTextureSize, sizeof(splatTextureSize), 1, fp);
	fread(&numBinaryBlobs, sizeof(numBinaryBlobs), 1, fp);

	blobArray = new AttributedBinaryBlob[numBinaryBlobs];
	for (uint32_t i = 0; i < numBinaryBlobs; i++)
	{
		blobArray[i].Load(fp);
	}
}

uint32_t StoredTerrainLandscape::GetSizeOnDisk() const
{
	uint32_t finalSize = 0;
	finalSize += sizeof(terrainLandscapeSize);
	finalSize += sizeof(numBinaryBlobs);
	for (uint32_t i = 0; i < numBinaryBlobs; i++)
	{
		finalSize += blobArray[i].SizeOnDisk();
	}
	return finalSize;
}

void StoredTerrainLandscape::FreeMemory()
{
	terrainLandscapeSize = 0;

	if (numBinaryBlobs > 0)
	{
		if (NULL != blobArray)
		{
			delete [] blobArray;
			blobArray = NULL;
		}

		numBinaryBlobs = 0;
	}
}

void StoredTerrainLandscape::SetValues(unsigned int terrainLandscapeSize, float* heightData,
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
									   const std::string& normal7Path)
{
	FreeMemory();

	numBinaryBlobs = totalAttributes;
	blobArray = new AttributedBinaryBlob[numBinaryBlobs];

	// Store the terrain height data.
	{
		const unsigned int blobIndex = 0;

		terrainLandscapeSize = static_cast<uint32_t>(terrainLandscapeSize);

		unsigned long compressedSize;
		unsigned long targetSize = terrainLandscapeSize * terrainLandscapeSize * sizeof(float);

		blobArray[blobIndex].attribute = TerrainHeightData;

		if (CompressionLib::Compression::CompressByteArray(reinterpret_cast<const uint8_t*>(heightData),
														   targetSize,
														   &blobArray[blobIndex].data,
														   &compressedSize))
		{
			blobArray[blobIndex].compressed = true;
			blobArray[blobIndex].blobSize = static_cast<uint32_t>(compressedSize);

			std::cout << "StoredTerrainLandscape::Compressed height size from " << targetSize << " bytes to " << compressedSize << " bytes." << std::endl;
		}
		else
		{
			blobArray[blobIndex].compressed = false;

			unsigned int numHeights = terrainLandscapeSize * terrainLandscapeSize;
			float* heightArrayCopy = new float[numHeights];
			for (unsigned int i = 0; i < numHeights; i++)
			{
				heightArrayCopy[i] = heightData[i];
			}
			blobArray[blobIndex].blobSize = targetSize;
			blobArray[blobIndex].data = reinterpret_cast<uint8_t*>(heightArrayCopy);
		}
	}

	// Splat textures.
	splatTextureSize = static_cast<uint32_t>(splatTextureDimension);

	// Store splat texture 0 data.
	{
		const unsigned int blobIndex = 1;

		unsigned long compressedSize;
		unsigned long targetSize = splatTextureSize * splatTextureSize * 4;		/// 4 bytes per splat pixel.

		blobArray[blobIndex].attribute = TerrainSplat0Data;

		if (CompressionLib::Compression::CompressByteArray(reinterpret_cast<const uint8_t*>(splatTexture0Data),
														   targetSize,
														   &blobArray[blobIndex].data,
														   &compressedSize))
		{
			blobArray[blobIndex].compressed = true;
			blobArray[blobIndex].blobSize = static_cast<uint32_t>(compressedSize);

			std::cout << "StoredTerrainLandscape::Compressed splat texture 0 size from " << targetSize << " bytes to " << compressedSize << " bytes." << std::endl;
		}
		else
		{
			blobArray[blobIndex].compressed = false;

			unsigned int numElements = splatTextureSize * splatTextureSize * 4;
			uint8_t* textureSplatCopy = new uint8_t[numElements];
			for (unsigned int i = 0; i < numElements; i++)
			{
				textureSplatCopy[i] = splatTexture0Data[i];
			}
			blobArray[blobIndex].blobSize = numElements;
			blobArray[blobIndex].data = reinterpret_cast<uint8_t*>(textureSplatCopy);
		}
	}

	// Store splat texture 1 data.
	{
		const unsigned int blobIndex = 2;

		unsigned long compressedSize;
		unsigned long targetSize = splatTextureSize * splatTextureSize * 4;			/// 4 bytes per splat pixel.

		blobArray[blobIndex].attribute = TerrainSplat1Data;

		if (CompressionLib::Compression::CompressByteArray(reinterpret_cast<const uint8_t*>(splatTexture1Data),
														   targetSize,
														   &blobArray[blobIndex].data,
														   &compressedSize))
		{
			blobArray[blobIndex].compressed = true;
			blobArray[blobIndex].blobSize = static_cast<uint32_t>(compressedSize);

			std::cout << "StoredTerrainLandscape::Compressed splat texture 1 size from " << targetSize << " bytes to " << compressedSize << " bytes." << std::endl;
		}
		else
		{
			blobArray[blobIndex].compressed = false;

			unsigned int numElements = splatTextureSize * splatTextureSize * 4;
			uint8_t* textureSplatCopy = new uint8_t[numElements];
			for (unsigned int i = 0; i < numElements; i++)
			{
				textureSplatCopy[i] = splatTexture1Data[i];
			}
			blobArray[blobIndex].blobSize = numElements;
			blobArray[blobIndex].data = reinterpret_cast<uint8_t*>(textureSplatCopy);
		}
	}

	// Texture 0 Diffuse path.
	{
		const unsigned int blobIndex = 3;
		blobArray[blobIndex].attribute = TerrainDiffuse0Data;
		blobArray[blobIndex].compressed = false;
		blobArray[blobIndex].blobSize = (uint32_t)(diffuse0Path.length() + 1);

		char* value = new char[blobArray[blobIndex].blobSize];
		std::strcpy(value, diffuse0Path.c_str());
		blobArray[blobIndex].data = reinterpret_cast<uint8_t*>(value);
	}

	// Texture 0 Normal path.
	{
		const unsigned int blobIndex = 4;
		blobArray[blobIndex].attribute = TerrainNormal0Data;
		blobArray[blobIndex].compressed = false;
		blobArray[blobIndex].blobSize = (uint32_t)(normal0Path.length() + 1);

		char* value = new char[blobArray[blobIndex].blobSize];
		std::strcpy(value, normal0Path.c_str());
		blobArray[blobIndex].data = reinterpret_cast<uint8_t*>(value);
	}

	// Texture 1 Diffuse path.
	{
		const unsigned int blobIndex = 5;
		blobArray[blobIndex].attribute = TerrainDiffuse1Data;
		blobArray[blobIndex].compressed = false;
		blobArray[blobIndex].blobSize = (uint32_t)(diffuse1Path.length() + 1);

		char* value = new char[blobArray[blobIndex].blobSize];
		std::strcpy(value, diffuse1Path.c_str());
		blobArray[blobIndex].data = reinterpret_cast<uint8_t*>(value);
	}

	// Texture 1 Normal path.
	{
		const unsigned int blobIndex = 6;
		blobArray[blobIndex].attribute = TerrainNormal1Data;
		blobArray[blobIndex].compressed = false;
		blobArray[blobIndex].blobSize = (uint32_t)(normal1Path.length() + 1);

		char* value = new char[blobArray[blobIndex].blobSize];
		std::strcpy(value, normal1Path.c_str());
		blobArray[blobIndex].data = reinterpret_cast<uint8_t*>(value);
	}

	// Texture 2 Diffuse path.
	{
		const unsigned int blobIndex = 7;
		blobArray[blobIndex].attribute = TerrainDiffuse2Data;
		blobArray[blobIndex].compressed = false;
		blobArray[blobIndex].blobSize = (uint32_t)(diffuse2Path.length() + 1);

		char* value = new char[blobArray[blobIndex].blobSize];
		std::strcpy(value, diffuse2Path.c_str());
		blobArray[blobIndex].data = reinterpret_cast<uint8_t*>(value);
	}

	// Texture 2 Normal path.
	{
		const unsigned int blobIndex = 8;
		blobArray[blobIndex].attribute = TerrainNormal2Data;
		blobArray[blobIndex].compressed = false;
		blobArray[blobIndex].blobSize = (uint32_t)(normal2Path.length() + 1);

		char* value = new char[blobArray[blobIndex].blobSize];
		std::strcpy(value, normal2Path.c_str());
		blobArray[blobIndex].data = reinterpret_cast<uint8_t*>(value);
	}

	// Texture 3 Diffuse path.
	{
		const unsigned int blobIndex = 9;
		blobArray[blobIndex].attribute = TerrainDiffuse3Data;
		blobArray[blobIndex].compressed = false;
		blobArray[blobIndex].blobSize = (uint32_t)(diffuse3Path.length() + 1);

		char* value = new char[blobArray[blobIndex].blobSize];
		std::strcpy(value, diffuse3Path.c_str());
		blobArray[blobIndex].data = reinterpret_cast<uint8_t*>(value);
	}

	// Texture 3 Normal path.
	{
		const unsigned int blobIndex = 10;
		blobArray[blobIndex].attribute = TerrainNormal3Data;
		blobArray[blobIndex].compressed = false;
		blobArray[blobIndex].blobSize = (uint32_t)(normal3Path.length() + 1);

		char* value = new char[blobArray[blobIndex].blobSize];
		std::strcpy(value, normal3Path.c_str());
		blobArray[blobIndex].data = reinterpret_cast<uint8_t*>(value);
	}

	// Texture 4 Diffuse path.
	{
		const unsigned int blobIndex = 11;
		blobArray[blobIndex].attribute = TerrainDiffuse4Data;
		blobArray[blobIndex].compressed = false;
		blobArray[blobIndex].blobSize = (uint32_t)(diffuse4Path.length() + 1);

		char* value = new char[blobArray[blobIndex].blobSize];
		std::strcpy(value, diffuse4Path.c_str());
		blobArray[blobIndex].data = reinterpret_cast<uint8_t*>(value);
	}

	// Texture 4 Normal path.
	{
		const unsigned int blobIndex = 12;
		blobArray[blobIndex].attribute = TerrainNormal4Data;
		blobArray[blobIndex].compressed = false;
		blobArray[blobIndex].blobSize = (uint32_t)(normal4Path.length() + 1);

		char* value = new char[blobArray[blobIndex].blobSize];
		std::strcpy(value, normal4Path.c_str());
		blobArray[blobIndex].data = reinterpret_cast<uint8_t*>(value);
	}

	// Texture 5 Diffuse path.
	{
		const unsigned int blobIndex = 13;
		blobArray[blobIndex].attribute = TerrainDiffuse5Data;
		blobArray[blobIndex].compressed = false;
		blobArray[blobIndex].blobSize = (uint32_t)(diffuse5Path.length() + 1);

		char* value = new char[blobArray[blobIndex].blobSize];
		std::strcpy(value, diffuse5Path.c_str());
		blobArray[blobIndex].data = reinterpret_cast<uint8_t*>(value);
	}

	// Texture 5 Normal path.
	{
		const unsigned int blobIndex = 14;
		blobArray[blobIndex].attribute = TerrainNormal5Data;
		blobArray[blobIndex].compressed = false;
		blobArray[blobIndex].blobSize = (uint32_t)(normal5Path.length() + 1);

		char* value = new char[blobArray[blobIndex].blobSize];
		std::strcpy(value, normal5Path.c_str());
		blobArray[blobIndex].data = reinterpret_cast<uint8_t*>(value);
	}

	// Texture 6 Diffuse path.
	{
		const unsigned int blobIndex = 15;
		blobArray[blobIndex].attribute = TerrainDiffuse6Data;
		blobArray[blobIndex].compressed = false;
		blobArray[blobIndex].blobSize = (uint32_t)(diffuse6Path.length() + 1);

		char* value = new char[blobArray[blobIndex].blobSize];
		std::strcpy(value, diffuse6Path.c_str());
		blobArray[blobIndex].data = reinterpret_cast<uint8_t*>(value);
	}

	// Texture 6 Normal path.
	{
		const unsigned int blobIndex = 16;
		blobArray[blobIndex].attribute = TerrainNormal6Data;
		blobArray[blobIndex].compressed = false;
		blobArray[blobIndex].blobSize = (uint32_t)(normal6Path.length() + 1);

		char* value = new char[blobArray[blobIndex].blobSize];
		std::strcpy(value, normal6Path.c_str());
		blobArray[blobIndex].data = reinterpret_cast<uint8_t*>(value);
	}

	// Texture 7 Diffuse path.
	{
		const unsigned int blobIndex = 17;
		blobArray[blobIndex].attribute = TerrainDiffuse7Data;
		blobArray[blobIndex].compressed = false;
		blobArray[blobIndex].blobSize = (uint32_t)(diffuse7Path.length() + 1);

		char* value = new char[blobArray[blobIndex].blobSize];
		std::strcpy(value, diffuse7Path.c_str());
		blobArray[blobIndex].data = reinterpret_cast<uint8_t*>(value);
	}

	// Texture 7 Normal path.
	{
		const unsigned int blobIndex = 18;
		blobArray[blobIndex].attribute = TerrainNormal7Data;
		blobArray[blobIndex].compressed = false;
		blobArray[blobIndex].blobSize = (uint32_t)(normal7Path.length() + 1);

		char* value = new char[blobArray[blobIndex].blobSize];
		std::strcpy(value, normal7Path.c_str());
		blobArray[blobIndex].data = reinterpret_cast<uint8_t*>(value);
	}
}

float* StoredTerrainLandscape::ReturnHeightData() const
{
	int blobIndex = -1;
	for (unsigned int i = 0; i < numBinaryBlobs; i++)
	{
		if (TerrainHeightData == blobArray[i].attribute)
		{
			blobIndex = i;
			break;
		}
	}

	if (blobIndex < 0)
	{
		return NULL;
	}

	AttributedBinaryBlob& heightBlob = blobArray[TerrainHeightData];
	if (heightBlob.compressed)
	{
		// Decompress and return array.
		uint8_t* decompressionTarget;
		unsigned long decompressedSize;

		CompressionLib::Compression::DecompressByteArray(reinterpret_cast<const uint8_t*>(heightBlob.data),
														 static_cast<unsigned long>(heightBlob.blobSize),
														 &decompressionTarget,
														 &decompressedSize);

		std::cout << "StoredTerrainLandscape::Decompressed height size from " << heightBlob.blobSize << " bytes to " << decompressedSize << " bytes." << std::endl;

		return reinterpret_cast<float*>(decompressionTarget);
	}
	else
	{
		// Create a copy of the array and return it.
		float* returnArray = new float[heightBlob.blobSize >> 2];
		for (unsigned int i = 0; i < heightBlob.blobSize >> 2; i++)
		{
			returnArray[i] = reinterpret_cast<float*>(heightBlob.data)[i];
		}

		return returnArray;
	}
}

uint8_t* StoredTerrainLandscape::ReturnSplatTexture0Data() const
{
	int blobIndex = -1;
	for (unsigned int i = 0; i < numBinaryBlobs; i++)
	{
		if (TerrainSplat0Data == blobArray[i].attribute)
		{
			blobIndex = i;
			break;
		}
	}

	if (blobIndex < 0)
	{
		return NULL;
	}

	AttributedBinaryBlob& textureSplat0Blob = blobArray[blobIndex];
	if (textureSplat0Blob.compressed)
	{
		// Decompress and return array.
		uint8_t* decompressionTarget;
		unsigned long decompressedSize;

		CompressionLib::Compression::DecompressByteArray(reinterpret_cast<const uint8_t*>(textureSplat0Blob.data),
														 static_cast<unsigned long>(textureSplat0Blob.blobSize),
														 &decompressionTarget,
														 &decompressedSize);

		std::cout << "StoredTerrainLandscape::Decompressed texture splat 0 size from " << textureSplat0Blob.blobSize << " bytes to " << decompressedSize << " bytes." << std::endl;

		return decompressionTarget;
	}
	else
	{
		// Create a copy of the data and return it.
		uint8_t* returnArray = new uint8_t[textureSplat0Blob.blobSize];
		for (unsigned int i = 0; i < textureSplat0Blob.blobSize; i++)
		{
			returnArray[i] = textureSplat0Blob.data[i];
		}
		
		return returnArray;
	}
}

uint8_t* StoredTerrainLandscape::ReturnSplatTexture1Data() const
{
	int blobIndex = -1;
	for (unsigned int i = 0; i < numBinaryBlobs; i++)
	{
		if (TerrainSplat1Data == blobArray[i].attribute)
		{
			blobIndex = i;
			break;
		}
	}

	if (blobIndex < 0)
	{
		return NULL;
	}

	AttributedBinaryBlob& textureSplat1Blob = blobArray[blobIndex];
	if (textureSplat1Blob.compressed)
	{
		// Decompress and return array.
		uint8_t* decompressionTarget;
		unsigned long decompressedSize;

		CompressionLib::Compression::DecompressByteArray(reinterpret_cast<const uint8_t*>(textureSplat1Blob.data),
														 static_cast<unsigned long>(textureSplat1Blob.blobSize),
														 &decompressionTarget,
														 &decompressedSize);

		std::cout << "StoredTerrainLandscape::Decompressed texture splat 1 size from " << textureSplat1Blob.blobSize << " bytes to " << decompressedSize << " bytes." << std::endl;

		return decompressionTarget;
	}
	else
	{
		// Create a copy of the data and return it.
		uint8_t* returnArray = new uint8_t[textureSplat1Blob.blobSize];
		for (unsigned int i = 0; i < textureSplat1Blob.blobSize; i++)
		{
			returnArray[i] = textureSplat1Blob.data[i];
		}

		return returnArray;
	}
}

const std::string StoredTerrainLandscape::ReturnDiffuse0Path() const
{
	int blobIndex = -1;
	for (unsigned int i = 0; i < numBinaryBlobs; i++)
	{
		if (TerrainDiffuse0Data == blobArray[i].attribute)
		{
			blobIndex = i;
			break;
		}
	}

	if (blobIndex < 0)
	{
		return "";
	}

	AttributedBinaryBlob& textureIDBlob = blobArray[blobIndex];

	std::string returnString = std::string(reinterpret_cast<char*>(textureIDBlob.data));
	return returnString;
}

const std::string StoredTerrainLandscape::ReturnNormal0Path() const
{
	int blobIndex = -1;
	for (unsigned int i = 0; i < numBinaryBlobs; i++)
	{
		if (TerrainNormal0Data == blobArray[i].attribute)
		{
			blobIndex = i;
			break;
		}
	}

	if (blobIndex < 0)
	{
		return "";
	}

	AttributedBinaryBlob& textureIDBlob = blobArray[blobIndex];

	std::string returnString = std::string(reinterpret_cast<char*>(textureIDBlob.data));
	return returnString;
}

const std::string StoredTerrainLandscape::ReturnDiffuse1Path() const
{
	int blobIndex = -1;
	for (unsigned int i = 0; i < numBinaryBlobs; i++)
	{
		if (TerrainDiffuse1Data == blobArray[i].attribute)
		{
			blobIndex = i;
			break;
		}
	}

	if (blobIndex < 0)
	{
		return "";
	}

	AttributedBinaryBlob& textureIDBlob = blobArray[blobIndex];

	std::string returnString = std::string(reinterpret_cast<char*>(textureIDBlob.data));
	return returnString;
}

const std::string StoredTerrainLandscape::ReturnNormal1Path() const
{
	int blobIndex = -1;
	for (unsigned int i = 0; i < numBinaryBlobs; i++)
	{
		if (TerrainNormal1Data == blobArray[i].attribute)
		{
			blobIndex = i;
			break;
		}
	}

	if (blobIndex < 0)
	{
		return "";
	}

	AttributedBinaryBlob& textureIDBlob = blobArray[blobIndex];

	std::string returnString = std::string(reinterpret_cast<char*>(textureIDBlob.data));
	return returnString;
}

const std::string StoredTerrainLandscape::ReturnDiffuse2Path() const
{
	int blobIndex = -1;
	for (unsigned int i = 0; i < numBinaryBlobs; i++)
	{
		if (TerrainDiffuse2Data == blobArray[i].attribute)
		{
			blobIndex = i;
			break;
		}
	}

	if (blobIndex < 0)
	{
		return "";
	}

	AttributedBinaryBlob& textureIDBlob = blobArray[blobIndex];

	std::string returnString = std::string(reinterpret_cast<char*>(textureIDBlob.data));
	return returnString;
}

const std::string StoredTerrainLandscape::ReturnNormal2Path() const
{
	int blobIndex = -1;
	for (unsigned int i = 0; i < numBinaryBlobs; i++)
	{
		if (TerrainNormal2Data == blobArray[i].attribute)
		{
			blobIndex = i;
			break;
		}
	}

	if (blobIndex < 0)
	{
		return "";
	}

	AttributedBinaryBlob& textureIDBlob = blobArray[blobIndex];

	std::string returnString = std::string(reinterpret_cast<char*>(textureIDBlob.data));
	return returnString;
}

const std::string StoredTerrainLandscape::ReturnDiffuse3Path() const
{
	int blobIndex = -1;
	for (unsigned int i = 0; i < numBinaryBlobs; i++)
	{
		if (TerrainDiffuse3Data == blobArray[i].attribute)
		{
			blobIndex = i;
			break;
		}
	}

	if (blobIndex < 0)
	{
		return "";
	}

	AttributedBinaryBlob& textureIDBlob = blobArray[blobIndex];

	std::string returnString = std::string(reinterpret_cast<char*>(textureIDBlob.data));
	return returnString;
}

const std::string StoredTerrainLandscape::ReturnNormal3Path() const
{
	int blobIndex = -1;
	for (unsigned int i = 0; i < numBinaryBlobs; i++)
	{
		if (TerrainNormal3Data == blobArray[i].attribute)
		{
			blobIndex = i;
			break;
		}
	}

	if (blobIndex < 0)
	{
		return "";
	}

	AttributedBinaryBlob& textureIDBlob = blobArray[blobIndex];

	std::string returnString = std::string(reinterpret_cast<char*>(textureIDBlob.data));
	return returnString;
}

const std::string StoredTerrainLandscape::ReturnDiffuse4Path() const
{
	int blobIndex = -1;
	for (unsigned int i = 0; i < numBinaryBlobs; i++)
	{
		if (TerrainDiffuse4Data == blobArray[i].attribute)
		{
			blobIndex = i;
			break;
		}
	}

	if (blobIndex < 0)
	{
		return "";
	}

	AttributedBinaryBlob& textureIDBlob = blobArray[blobIndex];

	std::string returnString = std::string(reinterpret_cast<char*>(textureIDBlob.data));
	return returnString;
}

const std::string StoredTerrainLandscape::ReturnNormal4Path() const
{
	int blobIndex = -1;
	for (unsigned int i = 0; i < numBinaryBlobs; i++)
	{
		if (TerrainNormal4Data == blobArray[i].attribute)
		{
			blobIndex = i;
			break;
		}
	}

	if (blobIndex < 0)
	{
		return "";
	}

	AttributedBinaryBlob& textureIDBlob = blobArray[blobIndex];

	std::string returnString = std::string(reinterpret_cast<char*>(textureIDBlob.data));
	return returnString;
}

const std::string StoredTerrainLandscape::ReturnDiffuse5Path() const
{
	int blobIndex = -1;
	for (unsigned int i = 0; i < numBinaryBlobs; i++)
	{
		if (TerrainDiffuse5Data == blobArray[i].attribute)
		{
			blobIndex = i;
			break;
		}
	}

	if (blobIndex < 0)
	{
		return "";
	}

	AttributedBinaryBlob& textureIDBlob = blobArray[blobIndex];

	std::string returnString = std::string(reinterpret_cast<char*>(textureIDBlob.data));
	return returnString;
}

const std::string StoredTerrainLandscape::ReturnNormal5Path() const
{
	int blobIndex = -1;
	for (unsigned int i = 0; i < numBinaryBlobs; i++)
	{
		if (TerrainNormal5Data == blobArray[i].attribute)
		{
			blobIndex = i;
			break;
		}
	}

	if (blobIndex < 0)
	{
		return "";
	}

	AttributedBinaryBlob& textureIDBlob = blobArray[blobIndex];

	std::string returnString = std::string(reinterpret_cast<char*>(textureIDBlob.data));
	return returnString;
}

const std::string StoredTerrainLandscape::ReturnDiffuse6Path() const
{
	int blobIndex = -1;
	for (unsigned int i = 0; i < numBinaryBlobs; i++)
	{
		if (TerrainDiffuse6Data == blobArray[i].attribute)
		{
			blobIndex = i;
			break;
		}
	}

	if (blobIndex < 0)
	{
		return "";
	}

	AttributedBinaryBlob& textureIDBlob = blobArray[blobIndex];

	std::string returnString = std::string(reinterpret_cast<char*>(textureIDBlob.data));
	return returnString;
}

const std::string StoredTerrainLandscape::ReturnNormal6Path() const
{
	int blobIndex = -1;
	for (unsigned int i = 0; i < numBinaryBlobs; i++)
	{
		if (TerrainNormal6Data == blobArray[i].attribute)
		{
			blobIndex = i;
			break;
		}
	}

	if (blobIndex < 0)
	{
		return "";
	}

	AttributedBinaryBlob& textureIDBlob = blobArray[blobIndex];

	std::string returnString = std::string(reinterpret_cast<char*>(textureIDBlob.data));
	return returnString;
}


const std::string StoredTerrainLandscape::ReturnDiffuse7Path() const
{
	int blobIndex = -1;
	for (unsigned int i = 0; i < numBinaryBlobs; i++)
	{
		if (TerrainDiffuse7Data == blobArray[i].attribute)
		{
			blobIndex = i;
			break;
		}
	}

	if (blobIndex < 0)
	{
		return "";
	}

	AttributedBinaryBlob& textureIDBlob = blobArray[blobIndex];

	std::string returnString = std::string(reinterpret_cast<char*>(textureIDBlob.data));
	return returnString;
}

const std::string StoredTerrainLandscape::ReturnNormal7Path() const
{
	int blobIndex = -1;
	for (unsigned int i = 0; i < numBinaryBlobs; i++)
	{
		if (TerrainNormal7Data == blobArray[i].attribute)
		{
			blobIndex = i;
			break;
		}
	}

	if (blobIndex < 0)
	{
		return "";
	}

	AttributedBinaryBlob& textureIDBlob = blobArray[blobIndex];

	std::string returnString = std::string(reinterpret_cast<char*>(textureIDBlob.data));
	return returnString;
}
