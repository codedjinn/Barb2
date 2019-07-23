#include "StaticMeshDBIndex.h"
#include "StoredStaticMesh.h"
#include <cassert>
#include <stdio.h>
#include <vector>

StaticMeshDBIndex::StaticMeshDBIndex()
{
}

StaticMeshDBIndex::~StaticMeshDBIndex()
{
}

int StaticMeshDBIndex::OffsetOfMesh(const std::string& identifier) const
{
	std::map<std::string, uint32_t>::const_iterator indexIterator = index.find(identifier);

	// If we cannot find the mesh in the index return -1
	if (index.end() == indexIterator)
	{
		return -1;
	}

	return indexIterator->second;
}

void StaticMeshDBIndex::Load(FILE* file)
{
	// Start from the beginning of the file
	fseek(file, 0L, SEEK_SET);

	// Read number of meshes.
	uint32_t numMeshes;
	fread(&numMeshes, sizeof(uint32_t), 1, file);

	uint32_t currentIndexOffset = sizeof(uint32_t);

	std::vector<uint32_t> meshOffsets;
	for (unsigned int i = 0; i < numMeshes; i++)
	{
		uint32_t currentOffset;

		// Move file back to the index offset
		fseek(file,
			  static_cast<long>(currentIndexOffset),
			  SEEK_SET);

		// Advance index offset
		currentIndexOffset += 4;

		// Read the offset of the stored mesh
		fread(&currentOffset, sizeof(uint32_t), 1, file);

		// Now we can load the mesh identifier
		fseek(file,
			  static_cast<long>(currentOffset),
			  SEEK_SET);

		StoredStaticMesh storedStaticMesh;
		storedStaticMesh.LoadIdentifier(file);

		std::pair<std::string, uint32_t> newElement(storedStaticMesh.GetIdentifier(), currentOffset);
		index.insert(newElement);
	}
}

std::vector<std::string> StaticMeshDBIndex::GetDatabaseContents() const
{
	std::vector<std::string> returnList;

	std::map<std::string, uint32_t>::const_iterator currentElement = index.begin();
	while (currentElement != index.end())
	{
		returnList.push_back(currentElement->first);
		currentElement++;
	}

	return returnList;
}

void StaticMeshDBIndex::PrintIndex() const
{
	std::map<std::string, uint32_t>::const_iterator currentElement = index.begin();
	while (currentElement != index.end())
	{
		printf("%s : %u\n", currentElement->first.c_str(), currentElement->second);
		currentElement++;
	}
}
