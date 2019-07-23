#include "TextureDBIndex.h"
#include "StoredTexture.h"
#include <assert.h>
#include <stdio.h>
#include <vector>

TextureDBIndex::TextureDBIndex()
{
}

TextureDBIndex::~TextureDBIndex()
{
}

int TextureDBIndex::OffsetOfTexture(const std::string& identifier)
{
	std::map<std::string, uint32_t>::const_iterator indexIterator = index.find(identifier);

	// If we cannot find the texture in the index return -1
	if (index.end() == indexIterator)
	{
		return -1;
	}

	return indexIterator->second;
}

void TextureDBIndex::Load(FILE* file)
{
	// Start from the beginning of the file
	fseek(file, 0L, SEEK_SET);

	uint32_t numTextures;
	fread(&numTextures, sizeof(uint32_t), 1, file);

	uint32_t currentIndexOffset	= 4;

	std::vector<uint32_t> textureOffsets;
	for (unsigned int i = 0; i < numTextures; i++)
	{
		uint32_t currentOffset;

		// Move file back to the index offset
		fseek(file,
			  static_cast<long>(currentIndexOffset),
			  SEEK_SET);

		// Advance index offset
		currentIndexOffset += 4;

		// Read the offset of the stored texture
		fread(&currentOffset, sizeof(uint32_t), 1, file);

		// Now we can load the texture identifier
		fseek(file,
			  static_cast<long>(currentOffset),
			  SEEK_SET);

		StoredTexture storedTexture;
		storedTexture.LoadIdentifier(file);

		std::pair<std::string, uint32_t> newElement(storedTexture.identifier, currentOffset);

		index.insert(newElement);
	}
}

void TextureDBIndex::PrintIndex() const
{
	std::map<std::string, uint32_t>::const_iterator currentElement = index.begin();
	while (currentElement != index.end())
	{
		printf("%s : %u\n", currentElement->first.c_str(), currentElement->second);

		currentElement++;
	}
}

std::vector<std::string> TextureDBIndex::GetTextureList() const
{
	std::vector<std::string> returnVector;
	std::map<std::string, uint32_t>::const_iterator currentElement = index.begin();
	while (currentElement != index.end())
	{
		returnVector.push_back(currentElement->first);
		currentElement++;
	}
	return returnVector;
}
