#include "AttributedBinaryBlob.h"

AttributedBinaryBlob::AttributedBinaryBlob()
{
	attribute = 0;
	compressed = false;
	blobSize = 0;
	data = NULL;
}

AttributedBinaryBlob::~AttributedBinaryBlob()
{
	FreeMemory();
}

void AttributedBinaryBlob::Save(FILE* fp) const
{
	fwrite(&attribute, sizeof(attribute), 1, fp);
	fwrite(&compressed, sizeof(compressed), 1, fp);
	fwrite(&blobSize, sizeof(blobSize), 1, fp);
	fwrite(data, blobSize, 1, fp);
}

void AttributedBinaryBlob::Load(FILE* fp)
{
	FreeMemory();

	fread(&attribute, sizeof(attribute), 1, fp);
	fread(&compressed, sizeof(compressed), 1, fp);
	fread(&blobSize, sizeof(blobSize), 1, fp);
	data = new uint8_t[blobSize];
	fread(data, blobSize, 1, fp);
}

uint32_t AttributedBinaryBlob::SizeOnDisk() const
{
	uint32_t finalSize = 0;
	finalSize += sizeof(attribute);
	finalSize += sizeof(compressed);
	finalSize += sizeof(blobSize);
	finalSize += blobSize;
	return finalSize;
}

void AttributedBinaryBlob::FreeMemory()
{
	if (NULL != data)
	{
		delete [] data;
	}

	attribute = 0;
	compressed = false;
	blobSize = 0;
	data = NULL;
}

