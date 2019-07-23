#include "StoredTexture.h"
#include <Compression.h>
#include <math.h>

StoredTexture::StoredTexture()
{
	identifierLength = 0;
	identifier = NULL;

	formatFlag = 0;

	width = 0;
	height = 0;

	dataSizeBytes = 0;
	data = NULL;
}

StoredTexture::~StoredTexture()
{
	if (identifierLength > 0)
	{
		delete [] identifier;
	}

	if (dataSizeBytes)
	{
		delete [] data;
	}
}

uint32_t StoredTexture::SizeOnDisk() const
{
	uint32_t size = 0;

	size += sizeof(identifierLength);
	size += identifierLength;

	size += sizeof(formatFlag);

	size += sizeof(width);
	size += sizeof(height);

	size += sizeof(dataSizeBytes);
	size += dataSizeBytes;

	return size;
}

void StoredTexture::Save(FILE* fp) const
{
	fwrite(&identifierLength, sizeof(identifierLength), 1, fp);

	fwrite(identifier, identifierLength, 1, fp);

	fwrite(&formatFlag, sizeof(formatFlag), 1, fp);

	fwrite(&width, sizeof(width), 1, fp);
	fwrite(&height, sizeof(height), 1, fp);

	fwrite(&dataSizeBytes, sizeof(dataSizeBytes), 1, fp);

	fwrite(data, dataSizeBytes, 1, fp);
}

void StoredTexture::Load(FILE* fp)
{
	fread(&identifierLength, sizeof(identifierLength), 1, fp);

	if (NULL != identifier)
	{
		delete [] identifier;
	}

	identifier	= new char[identifierLength];
	fread(identifier, identifierLength, 1, fp);

	fread(&formatFlag, sizeof(formatFlag), 1, fp);

	fread(&width, sizeof(width), 1, fp);
	fread(&height, sizeof(height), 1, fp);

	fread(&dataSizeBytes, sizeof(dataSizeBytes), 1, fp);

	if (NULL != data)
	{
		delete [] data;
	}

	data = new uint8_t[dataSizeBytes];
	fread(data, dataSizeBytes, 1, fp);
}

void StoredTexture::LoadIdentifier(FILE* fp)
{
	fread(&identifierLength, sizeof(identifierLength), 1, fp);

	if (NULL != identifier)
	{
		delete [] identifier;
	}

	identifier	= new char[identifierLength];
	fread(identifier, identifierLength, 1, fp);
}

static void GammaCorrectBuffer(uint8_t* buffer, unsigned int bufferSize, float gammaFactor)
{
	// Gamma correct values (except the alpha value).
	for (unsigned int i = 0; i < bufferSize; i += 4)
	{
		float r = static_cast<float>(buffer[i + 0]) / 255.0f;
		float g = static_cast<float>(buffer[i + 1]) / 255.0f;
		float b = static_cast<float>(buffer[i + 2]) / 255.0f;

		r = (float)pow(r, gammaFactor);
		g = (float)pow(g, gammaFactor);
		b = (float)pow(b, gammaFactor);

		buffer[i + 0] = static_cast<uint8_t>(r * 255.0f);
		buffer[i + 1] = static_cast<uint8_t>(g * 255.0f);
		buffer[i + 2] = static_cast<uint8_t>(b * 255.0f);
	}
}

void StoredTexture::ApplyGammaCorrection(float gammaFactor)
{
	if (formatFlag & ARGB32_COMPRESSED)
	{
		// Decompress byte array.
		uint8_t* decompressedBuffer = NULL;
		unsigned long int decompressedBufferSize = 0;

		CompressionLib::Compression::DecompressByteArray(data, dataSizeBytes, &decompressedBuffer, &decompressedBufferSize);

		// Apply gamma correction.
		GammaCorrectBuffer(decompressedBuffer, decompressedBufferSize, gammaFactor);

		// Free old memory.
		delete [] data;

		// Attempt to recompress byte array.
		unsigned long int compressedBufferSize = 0;
		if (CompressionLib::Compression::CompressByteArray(decompressedBuffer, decompressedBufferSize, &data, &compressedBufferSize))
		{
			dataSizeBytes = static_cast<int>(compressedBufferSize);

			delete [] decompressedBuffer;
		}
		else
		{
			// Could not compress new array. Set format flag to uncompressed and transfer memory.
			formatFlag &= ~ARGB32_COMPRESSED;
			formatFlag |= ARGB32_RAW;

			data = decompressedBuffer;
			dataSizeBytes = decompressedBufferSize;
		}

		return;
	}

	if (formatFlag & ARGB32_RAW)
	{
		// Apply gamma correction.
		GammaCorrectBuffer(data, dataSizeBytes, gammaFactor);
		return;
	}
}



