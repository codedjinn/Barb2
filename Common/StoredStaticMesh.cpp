#include "StoredStaticMesh.h"
#include <Compression.h>
#include <cassert>
#include <iostream>

enum StoredStaticMeshAttributes
{
	ATTRIBUTE_NUM_VERTICES				= 1, 
	ATTRIBUTE_VERTEX_ARRAY				= 2, 
	ATTRIBUTE_TEXCOORD_ARRAY			= 3, 
	ATTRIBUTE_NORMAL_ARRAY				= 4,
	ATTRIBUTE_NUM_INDICES				= 5,
	ATTRIBUTE_INDEX_ARRAY				= 6,
	ATTRIBUTE_NUM_LOD_LEVELS			= 7,
	ATTRIBUTE_LOD_DISTANCE				= 8
};

float* ExtractVertexArrayFromBlob(unsigned int numVertices, const AttributedBinaryBlob& blob)
{
	// Extract data from the binary blob.
	if (blob.compressed)
	{
		uint8_t* decompressedBuffer = NULL;
		unsigned long decompressedBufferSize = 0;

		CompressionLib::Compression::DecompressByteArray(blob.data, blob.blobSize, &decompressedBuffer, &decompressedBufferSize);
		assert(decompressedBufferSize == (numVertices * 3 * sizeof(float)));

		float* returnArray = reinterpret_cast<float*>(decompressedBuffer);
		return returnArray;
	}
	else
	{
		float* returnArray = new float[numVertices * 3];
		memcpy(returnArray, blob.data, numVertices * 3 * sizeof(float));
		return returnArray;
	}
}

float* ExtractTexCoordArrayFromBlob(unsigned int numVertices, const AttributedBinaryBlob& blob)
{
	// Extract data from the binary blob.
	if (blob.compressed)
	{
		uint8_t* decompressedBuffer = NULL;
		unsigned long decompressedBufferSize = 0;

		CompressionLib::Compression::DecompressByteArray(blob.data, blob.blobSize, &decompressedBuffer, &decompressedBufferSize);

		assert(decompressedBufferSize == (numVertices * 2 * sizeof(float)));

		float* returnArray = reinterpret_cast<float*>(decompressedBuffer);
		return returnArray;
	}
	else
	{
		float* returnArray = new float[numVertices * 2];
		memcpy(returnArray, blob.data, numVertices * 2 * sizeof(float));
		return returnArray;
	}
}

float* ExtractNormalArrayFromBlob(unsigned int numVertices, const AttributedBinaryBlob& blob)
{
	// Extract data from the binary blob.
	if (blob.compressed)
	{
		uint8_t* decompressedBuffer = NULL;
		unsigned long decompressedBufferSize = 0;

		CompressionLib::Compression::DecompressByteArray(blob.data, blob.blobSize, &decompressedBuffer, &decompressedBufferSize);
		assert(decompressedBufferSize == (numVertices * 3 * sizeof(float)));

		float* returnArray = reinterpret_cast<float*>(decompressedBuffer);
		return returnArray;
	}
	else
	{
		float* returnArray = new float[numVertices * 3];
		memcpy(returnArray, blob.data, numVertices * 3 * sizeof(float));
		return returnArray;
	}
}

uint32_t* ExtractIndexArrayFromBlob(unsigned int numIndices, const AttributedBinaryBlob& blob)
{
	// Extract data from the binary blob.
	if (blob.compressed)
	{
		uint8_t* decompressedBuffer = NULL;
		unsigned long decompressedBufferSize = 0;

		CompressionLib::Compression::DecompressByteArray(blob.data, blob.blobSize, &decompressedBuffer, &decompressedBufferSize);
		assert(decompressedBufferSize == (numIndices * sizeof(uint32_t)));

		uint32_t* returnArray = reinterpret_cast<uint32_t*>(decompressedBuffer);
		return returnArray;
	}
	else
	{
		uint32_t* returnArray = new uint32_t[numIndices];
		memcpy(returnArray, blob.data, numIndices * sizeof(uint32_t));
		return returnArray;
	}
}

AttributedBinaryBlob* CreateBlobFromVertexArray(float* vertexArray, const unsigned int numVertices)
{
	AttributedBinaryBlob* binaryBlob = new AttributedBinaryBlob;
	binaryBlob->attribute = ATTRIBUTE_VERTEX_ARRAY;

	uint8_t* compressedBuffer = NULL;
	unsigned long compressedBufferSize = 0;
	const unsigned long uncompressedBufferSize = numVertices * 3 * sizeof(float);
	if (CompressionLib::Compression::CompressByteArray(reinterpret_cast<const uint8_t*>(vertexArray), uncompressedBufferSize, &compressedBuffer, &compressedBufferSize))
	{
		delete[] vertexArray;

		binaryBlob->data = compressedBuffer;
		binaryBlob->blobSize = compressedBufferSize;
		binaryBlob->compressed = true;
	}
	else
	{
		binaryBlob->data = reinterpret_cast<uint8_t*>(vertexArray);
		binaryBlob->blobSize = uncompressedBufferSize;
		binaryBlob->compressed = false;
	}

	return binaryBlob;
}

AttributedBinaryBlob* CreateBlobFromTexCoordArray(float* texCoordArray, const unsigned int numVertices)
{
	AttributedBinaryBlob* binaryBlob = new AttributedBinaryBlob;
	binaryBlob->attribute = ATTRIBUTE_TEXCOORD_ARRAY;

	uint8_t* compressedBuffer = NULL;
	unsigned long compressedBufferSize = 0;
	const unsigned long uncompressedBufferSize = numVertices * 2 * sizeof(float);
	if (CompressionLib::Compression::CompressByteArray(reinterpret_cast<const uint8_t*>(texCoordArray), uncompressedBufferSize, &compressedBuffer, &compressedBufferSize))
	{
		delete[] texCoordArray;

		binaryBlob->data = compressedBuffer;
		binaryBlob->blobSize = compressedBufferSize;
		binaryBlob->compressed = true;
	}
	else
	{
		binaryBlob->data = reinterpret_cast<uint8_t*>(texCoordArray);
		binaryBlob->blobSize = uncompressedBufferSize;
		binaryBlob->compressed = false;
	}

	return binaryBlob;
}

AttributedBinaryBlob* CreateBlobFromNormalArray(float* normalArray, const unsigned int numVertices)
{
	AttributedBinaryBlob* binaryBlob = new AttributedBinaryBlob;
	binaryBlob->attribute = ATTRIBUTE_NORMAL_ARRAY;

	uint8_t* compressedBuffer = NULL;
	unsigned long compressedBufferSize = 0;
	const unsigned long uncompressedBufferSize = numVertices * 3 * sizeof(float);
	if (CompressionLib::Compression::CompressByteArray(reinterpret_cast<const uint8_t*>(normalArray), uncompressedBufferSize, &compressedBuffer, &compressedBufferSize))
	{
		delete[] normalArray;

		binaryBlob->data = compressedBuffer;
		binaryBlob->blobSize = compressedBufferSize;
		binaryBlob->compressed = true;
	}
	else
	{
		binaryBlob->data = reinterpret_cast<uint8_t*>(normalArray);
		binaryBlob->blobSize = uncompressedBufferSize;
		binaryBlob->compressed = false;
	}

	return binaryBlob;
}

AttributedBinaryBlob* CreateBlobFromIndexArray(uint32_t* indexArray, const unsigned int numIndices)
{
	AttributedBinaryBlob* binaryBlob = new AttributedBinaryBlob;
	binaryBlob->attribute = ATTRIBUTE_INDEX_ARRAY;

	uint8_t* compressedBuffer = NULL;
	unsigned long compressedBufferSize = 0;
	const unsigned long uncompressedBufferSize = numIndices * sizeof(uint32_t);
	if (CompressionLib::Compression::CompressByteArray(reinterpret_cast<const uint8_t*>(indexArray), uncompressedBufferSize, &compressedBuffer, &compressedBufferSize))
	{
		delete[] indexArray;

		binaryBlob->data = compressedBuffer;
		binaryBlob->blobSize = compressedBufferSize;
		binaryBlob->compressed = true;
	}
	else
	{
		binaryBlob->data = reinterpret_cast<uint8_t*>(indexArray);
		binaryBlob->blobSize = uncompressedBufferSize;
		binaryBlob->compressed = false;
	}

	return binaryBlob;
}

StoredStaticMesh::StoredStaticMesh()
{
	identifierLength = 0;
	identifier = NULL;
}

StoredStaticMesh::~StoredStaticMesh()
{
	FreeMemory();
}

uint32_t StoredStaticMesh::SizeOnDisk() const
{
	uint32_t size = 0;

	size += sizeof(identifierLength);
	size += identifierLength;

    // Num binary blobs.
    size += sizeof(uint32_t);

	for (unsigned int i = 0; i < binaryBlobs.size(); i++)
	{
		size += binaryBlobs[i]->SizeOnDisk();

		// Enable for debug output.
		if (true)
		{
			std::cout << "Saving binary blob:" << std::endl;
			std::cout << "Attribute: " << binaryBlobs[i]->attribute << std::endl;
			std::cout << "Size : " << binaryBlobs[i]->blobSize << std::endl;
			std::cout << "Compressed : " << binaryBlobs[i]->compressed << std::endl;
			std::cout << std::endl;
		}
	}

	return size;
}

void StoredStaticMesh::Save(FILE* fp) const
{
	fwrite(&identifierLength, sizeof(identifierLength), 1, fp);
	fwrite(identifier, identifierLength, 1, fp);
	
	uint32_t numBinaryBlobs = (uint32_t)binaryBlobs.size();
	fwrite(&numBinaryBlobs, sizeof(numBinaryBlobs), 1, fp);
	for (unsigned int i = 0; i < numBinaryBlobs; i++)
	{
		binaryBlobs[i]->Save(fp);
	}
}

void StoredStaticMesh::Load(FILE* fp)
{
	FreeMemory();

	fread(&identifierLength, sizeof(identifierLength), 1, fp);
	identifier = new char[identifierLength];
	fread(identifier, identifierLength, 1, fp);

	uint32_t numBinaryBlobs;
	fread(&numBinaryBlobs, sizeof(numBinaryBlobs), 1, fp);
	for (unsigned int i = 0; i < numBinaryBlobs; i++)
	{
		AttributedBinaryBlob* newBlob = new AttributedBinaryBlob;
		newBlob->Load(fp);
		binaryBlobs.push_back(newBlob);
	}
}

void StoredStaticMesh::FreeMemory()
{
	if (NULL != identifier)
	{
		delete [] identifier;
	}

	for (unsigned int i = 0; i < binaryBlobs.size(); i++)
	{
		delete binaryBlobs[i];
	}
	binaryBlobs.clear();
}

std::string StoredStaticMesh::GetIdentifier() const
{
	if (NULL == identifier)
	{
		return std::string("");
	}
	return std::string(identifier);
}

void StoredStaticMesh::SetIdentifier(const std::string& identifier)
{
	if (this->identifierLength > 0)
	{
		delete[] this->identifier;
	}

	this->identifierLength = (uint32_t)(identifier.length() + 1);
	this->identifier = new char[this->identifierLength];
	std::strcpy(this->identifier, identifier.c_str());
}

void StoredStaticMesh::LoadIdentifier(FILE* fp)
{
	uint32_t length;
	fread(&length, sizeof(identifierLength), 1, fp);

	identifierLength = length;

	if (NULL != identifier)
	{
		delete [] identifier;
	}

	identifier	= new char[identifierLength];
	fread(identifier, identifierLength, 1, fp);
}

unsigned int StoredStaticMesh::GetNumVertices() const
{
	int blobIndex = -1;
	for (unsigned int i = 0; i < binaryBlobs.size(); i++)
	{
		if (ATTRIBUTE_NUM_VERTICES == binaryBlobs[i]->attribute)
		{
			blobIndex = i;
			break;
		}
	}

	if (blobIndex < 0)
	{
		return 0;
	}

	// Extract data from the binary blob.
	return static_cast<unsigned int>(*(reinterpret_cast<uint32_t*>(binaryBlobs[blobIndex]->data)));
}

float* StoredStaticMesh::GetVertexArray() const
{
	int blobIndex = -1;
	for (unsigned int i = 0; i < binaryBlobs.size(); i++)
	{
		if (ATTRIBUTE_VERTEX_ARRAY == binaryBlobs[i]->attribute)
		{
			blobIndex = i;
			break;
		}
	}

	if (blobIndex < 0)
	{
		return NULL;
	}

	return ExtractVertexArrayFromBlob(GetNumVertices(), *binaryBlobs[blobIndex]);
}

float* StoredStaticMesh::GetTexCoordArray() const
{
	int blobIndex = -1;
	for (unsigned int i = 0; i < binaryBlobs.size(); i++)
	{
		if (ATTRIBUTE_TEXCOORD_ARRAY == binaryBlobs[i]->attribute)
		{
			blobIndex = i;
			break;
		}
	}

	if (blobIndex < 0)
	{
		return NULL;
	}

	return ExtractTexCoordArrayFromBlob(GetNumVertices(), *binaryBlobs[blobIndex]);
}

float* StoredStaticMesh::GetNormalArray() const
{
	int blobIndex = -1;
	for (unsigned int i = 0; i < binaryBlobs.size(); i++)
	{
		if (ATTRIBUTE_NORMAL_ARRAY == binaryBlobs[i]->attribute)
		{
			blobIndex = i;
			break;
		}
	}

	if (blobIndex < 0)
	{
		return NULL;
	}

	return ExtractNormalArrayFromBlob(GetNumVertices(), *binaryBlobs[blobIndex]);
}

unsigned int StoredStaticMesh::GetNumIndices() const
{
	int blobIndex = -1;
	for (unsigned int i = 0; i < binaryBlobs.size(); i++)
	{
		if (ATTRIBUTE_NUM_INDICES == binaryBlobs[i]->attribute)
		{
			blobIndex = i;
			break;
		}
	}

	if (blobIndex < 0)
	{
		return 0;
	}

	// Extract data from the binary blob.
	return static_cast<unsigned int>(*(reinterpret_cast<uint32_t*>(binaryBlobs[blobIndex]->data)));
}

int StoredStaticMesh::GetLODAttributeIndex() const
{
	int blobIndex = -1;
	for (unsigned int i = 0; i < binaryBlobs.size(); i++)
	{
		if (ATTRIBUTE_NUM_LOD_LEVELS == binaryBlobs[i]->attribute)
		{
			blobIndex = i + 1;
			break;
		}
	}
	return blobIndex;
}

std::vector<StoredStaticMeshLODLevel> StoredStaticMesh::GetLODLevels() const
{
	std::vector<StoredStaticMeshLODLevel> returnList;

	int blobIndex = -1;
	for (unsigned int i = 0; i < binaryBlobs.size(); i++)
	{
		if (ATTRIBUTE_NUM_LOD_LEVELS == binaryBlobs[i]->attribute)
		{
			blobIndex = i;
			break;
		}
	}

	if (blobIndex < 0)
	{
		// Static mesh has no lods.
		return returnList;
	}

	// Extract data from the binary blob.
	unsigned int numLods = static_cast<unsigned int>(*(reinterpret_cast<uint32_t*>(binaryBlobs[blobIndex]->data)));

	int currentIndex = blobIndex + 1;

	for (unsigned int i = 0; i < numLods; i++)
	{
		StoredStaticMeshLODLevel level;

		// Read distance.
		{
			AttributedBinaryBlob* currentBlob = binaryBlobs[currentIndex];
			assert(NULL != currentBlob);
			assert(ATTRIBUTE_LOD_DISTANCE == currentBlob->attribute);

			level.distance = *reinterpret_cast<float*>(currentBlob->data);
			currentIndex++;
		}

		// Read num vertices.
		{
			AttributedBinaryBlob* currentBlob = binaryBlobs[currentIndex];
			assert(NULL != currentBlob);
			assert(ATTRIBUTE_NUM_VERTICES == currentBlob->attribute);

			level.numVertices = *reinterpret_cast<uint32_t*>(currentBlob->data);
			currentIndex++;
		}

		// Read vertex array.
		{
			AttributedBinaryBlob* currentBlob = binaryBlobs[currentIndex];
			assert(NULL != currentBlob);
			assert(ATTRIBUTE_VERTEX_ARRAY == currentBlob->attribute);

			level.vertexArray = ExtractVertexArrayFromBlob(level.numVertices, *currentBlob);
			currentIndex++;
		}

		// Read texture coordinate array.
		{
			AttributedBinaryBlob* currentBlob = binaryBlobs[currentIndex];
			assert(NULL != currentBlob);
			assert(ATTRIBUTE_TEXCOORD_ARRAY == currentBlob->attribute);

			level.texCoordArray = ExtractTexCoordArrayFromBlob(level.numVertices, *currentBlob);
			currentIndex++;
		}

		// Read normal array.
		{
			AttributedBinaryBlob* currentBlob = binaryBlobs[currentIndex];
			assert(NULL != currentBlob);
			assert(ATTRIBUTE_NORMAL_ARRAY == currentBlob->attribute);

			level.normalArray = ExtractNormalArrayFromBlob(level.numVertices, *currentBlob);
			currentIndex++;
		}

		// Read num indices.
		{
			AttributedBinaryBlob* currentBlob = binaryBlobs[currentIndex];
			assert(NULL != currentBlob);
			assert(ATTRIBUTE_NUM_INDICES == currentBlob->attribute);

			level.numIndices = *reinterpret_cast<uint32_t*>(currentBlob->data);
			currentIndex++;
		}

		// Read index array.
		{
			AttributedBinaryBlob* currentBlob = binaryBlobs[currentIndex];
			assert(NULL != currentBlob);
			assert(ATTRIBUTE_INDEX_ARRAY == currentBlob->attribute);

			level.indexArray = ExtractIndexArrayFromBlob(level.numIndices, *currentBlob);
			currentIndex++;
		}
		returnList.push_back(level);
	}
	
	return returnList;
}

uint32_t* StoredStaticMesh::GetIndexArray() const
{
	int blobIndex = -1;
	for (unsigned int i = 0; i < binaryBlobs.size(); i++)
	{
		if (ATTRIBUTE_INDEX_ARRAY == binaryBlobs[i]->attribute)
		{
			blobIndex = i;
			break;
		}
	}

	if (blobIndex < 0)
	{
		return NULL;
	}

	return ExtractIndexArrayFromBlob(GetNumIndices(), *binaryBlobs[blobIndex]);
}

void StoredStaticMesh::AddLODLevel(const StoredStaticMeshLODLevel& lodLevel)
{
	// Determine the number and starting location of the binary blobs.
	int blobIndex = -1;
	for (unsigned int i = 0; i < binaryBlobs.size(); i++)
	{
		if (ATTRIBUTE_NUM_LOD_LEVELS == binaryBlobs[i]->attribute)
		{
			blobIndex = i;
			break;
		}
	}

	if (-1 == blobIndex)
	{
		// Create a new lod section in the blob collection
		AttributedBinaryBlob* binaryBlob = new AttributedBinaryBlob;
		binaryBlob->attribute = ATTRIBUTE_NUM_LOD_LEVELS;

		uint32_t* data = new uint32_t;
		*data = 0;
		binaryBlob->data = reinterpret_cast<uint8_t*>(data);

		binaryBlob->compressed = false;
		binaryBlob->blobSize = sizeof(uint32_t);

		binaryBlobs.push_back(binaryBlob);

		blobIndex = (uint32_t)(binaryBlobs.size() - 1);
	}

	// Read the number of binary blobs.
	uint32_t& blobData = *reinterpret_cast<uint32_t*>(binaryBlobs[blobIndex]->data);
	uint32_t numLODLevels = blobData;
	blobData++;

	AttributedBinaryBlob* blobsToInsert[7];
	// Create LOD level binary blobs.
	{
		{
			AttributedBinaryBlob* binaryBlob = new AttributedBinaryBlob;
			binaryBlob->attribute = ATTRIBUTE_LOD_DISTANCE;

			float* distance = new float;
			*distance = lodLevel.distance;
			binaryBlob->data = reinterpret_cast<uint8_t*>(distance);
			binaryBlob->blobSize = sizeof(float);
			binaryBlob->compressed = false;

			blobsToInsert[0] = binaryBlob;
		}

		// Num vertices.
		{
			AttributedBinaryBlob* binaryBlob = new AttributedBinaryBlob;
			binaryBlob->attribute = ATTRIBUTE_NUM_VERTICES;
			binaryBlob->blobSize = sizeof(uint32_t);

			uint32_t* data = new uint32_t;
			*data = lodLevel.numVertices;

			binaryBlob->data = reinterpret_cast<uint8_t*>(data);
			binaryBlob->compressed = false;

			blobsToInsert[1] = binaryBlob;
		}

		// Vertex array.
		blobsToInsert[2] = CreateBlobFromVertexArray(lodLevel.vertexArray, lodLevel.numVertices);

		// Tex coord array.
		blobsToInsert[3] = CreateBlobFromTexCoordArray(lodLevel.texCoordArray, lodLevel.numVertices);

		// Normal array.
		blobsToInsert[4] = CreateBlobFromNormalArray(lodLevel.normalArray, lodLevel.numVertices);

		// Num indices.
		{
			AttributedBinaryBlob* binaryBlob = new AttributedBinaryBlob;
			binaryBlob->attribute = ATTRIBUTE_NUM_INDICES;
			binaryBlob->blobSize = sizeof(uint32_t);

			uint32_t* data = new uint32_t;
			*data = lodLevel.numIndices;

			binaryBlob->data = reinterpret_cast<uint8_t*>(data);
			binaryBlob->compressed = false;

			blobsToInsert[5] = binaryBlob;
		}

		// Index array.
		blobsToInsert[6] = CreateBlobFromIndexArray(lodLevel.indexArray, lodLevel.numIndices);
	}

	std::vector<AttributedBinaryBlob*>::iterator locationToInsert = binaryBlobs.begin();
	locationToInsert += blobIndex + 1;
	locationToInsert += numLODLevels * 7;			// There are currently 7 attributed binary blob components per LOD level.

	binaryBlobs.insert(locationToInsert, blobsToInsert, blobsToInsert + 7);

}

void StoredStaticMesh::DeleteLODLevel(int index)
{
	if (index < 0)
		return;

	// Determine the starting location of the binary blobs.
	int blobIndex = -1;
	for (unsigned int i = 0; i < binaryBlobs.size(); i++)
	{
		if (ATTRIBUTE_NUM_LOD_LEVELS == binaryBlobs[i]->attribute)
		{
			blobIndex = i;
			break;
		}
	}

	if (-1 == blobIndex)
	{
		// There are no lods associated with this static mesh yet.
		return;
	}
	else
	{
		AttributedBinaryBlob& currentBlob = *binaryBlobs[blobIndex];
		assert(!currentBlob.compressed);

		// Check to see that the index is in the right range.
		uint32_t& numBinaryBlobs = *reinterpret_cast<uint32_t*>(currentBlob.data);
		if ((uint32_t)index >= numBinaryBlobs)
			return;

		numBinaryBlobs--;
	}

	blobIndex++;
	blobIndex += index * 7;
	
	// Remove the next 7 items to remove the lod level.
	for (int i = 0; i < 7; i++)
		delete binaryBlobs[blobIndex + i];

	binaryBlobs.erase(binaryBlobs.begin() + blobIndex, binaryBlobs.begin() + blobIndex + 7);
}

void StoredStaticMesh::SetValues(const std::string& identifier,
								 unsigned int numVertices,
								 float* vertexArray,
								 float* texCoordArray,
								 float* normalArray,
								 unsigned int numIndices,
								 uint32_t* indexArray)
{
	FreeMemory();

	// Identifier.
	{
		if (this->identifierLength > 0)
		{
			delete[] this->identifier;
		}

		identifierLength = (uint32_t)(identifier.length() + 1);
		this->identifier = new char[identifierLength];
		std::strcpy(this->identifier, identifier.c_str());
	}

	// Num vertices.
	{
		AttributedBinaryBlob* binaryBlob = new AttributedBinaryBlob;
		binaryBlob->attribute = ATTRIBUTE_NUM_VERTICES;
		binaryBlob->blobSize = sizeof(uint32_t);

		uint32_t* data = new uint32_t;
		*data = numVertices;

		binaryBlob->data = reinterpret_cast<uint8_t*>(data);
		binaryBlob->compressed = false;

		binaryBlobs.push_back(binaryBlob);
	}

	// Vertex array.
	binaryBlobs.push_back(CreateBlobFromVertexArray(vertexArray, numVertices));

	// Tex coord array.
	binaryBlobs.push_back(CreateBlobFromTexCoordArray(texCoordArray, numVertices));

	// Normal array.
	binaryBlobs.push_back(CreateBlobFromNormalArray(normalArray, numVertices));

	// Num indices.
	{
		AttributedBinaryBlob* binaryBlob = new AttributedBinaryBlob;
		binaryBlob->attribute = ATTRIBUTE_NUM_INDICES;
		binaryBlob->blobSize = sizeof(uint32_t);

		uint32_t* data = new uint32_t;
		*data = numIndices;

		binaryBlob->data = reinterpret_cast<uint8_t*>(data);
		binaryBlob->compressed = false;

		binaryBlobs.push_back(binaryBlob);
	}

	// Index array.
	binaryBlobs.push_back(CreateBlobFromIndexArray(indexArray, numIndices));
}

void StoredStaticMesh::SetValues(const std::string& identifier,
								 unsigned int numVertices,
								 float* vertexArray,
								 float* texCoordArray,
								 float* normalArray,
								 unsigned int numIndices,
								 uint32_t* indexArray, 
								 const std::vector<StoredStaticMeshLODLevel>& lodLevels)
{
	SetValues(identifier,
			  numVertices,
			  vertexArray,
			  texCoordArray,
			  normalArray,
			  numIndices,
			  indexArray);

	// Num LodLevels.
	{
		AttributedBinaryBlob* binaryBlob = new AttributedBinaryBlob;
		binaryBlob->attribute = ATTRIBUTE_NUM_LOD_LEVELS;

		uint32_t* data = new uint32_t;
		*data = (uint32_t)lodLevels.size();
		binaryBlob->data = reinterpret_cast<uint8_t*>(data);

		binaryBlob->compressed = false;
		binaryBlob->blobSize = sizeof(uint32_t);

		binaryBlobs.push_back(binaryBlob);
	}

	for (unsigned int i = 0; i < lodLevels.size(); i++)
	{
		const StoredStaticMeshLODLevel& currentLODLevel = lodLevels[i];

		// Write out the LOD level.
		// Distance.
		{
			AttributedBinaryBlob* binaryBlob = new AttributedBinaryBlob;
			binaryBlob->attribute = ATTRIBUTE_LOD_DISTANCE;
			
			float* distance = new float;
			*distance = currentLODLevel.distance;
			binaryBlob->data = reinterpret_cast<uint8_t*>(distance);
			binaryBlob->blobSize = sizeof(float);
			binaryBlob->compressed = false;

			binaryBlobs.push_back(binaryBlob);
		}

		// Num vertices.
		{
			AttributedBinaryBlob* binaryBlob = new AttributedBinaryBlob;
			binaryBlob->attribute = ATTRIBUTE_NUM_VERTICES;
			binaryBlob->blobSize = sizeof(uint32_t);

			uint32_t* data = new uint32_t;
			*data = currentLODLevel.numVertices;

			binaryBlob->data = reinterpret_cast<uint8_t*>(data);
			binaryBlob->compressed = false;

			binaryBlobs.push_back(binaryBlob);
		}

		// Vertex array.
		binaryBlobs.push_back(CreateBlobFromVertexArray(currentLODLevel.vertexArray, currentLODLevel.numVertices));
		
		// Tex coord array.
		binaryBlobs.push_back(CreateBlobFromTexCoordArray(currentLODLevel.texCoordArray, currentLODLevel.numVertices));

		// Normal array.
		binaryBlobs.push_back(CreateBlobFromNormalArray(currentLODLevel.normalArray, currentLODLevel.numVertices));

		// Num indices.
		{
			AttributedBinaryBlob* binaryBlob = new AttributedBinaryBlob;
			binaryBlob->attribute = ATTRIBUTE_NUM_INDICES;
			binaryBlob->blobSize = sizeof(uint32_t);

			uint32_t* data = new uint32_t;
			*data = currentLODLevel.numIndices;

			binaryBlob->data = reinterpret_cast<uint8_t*>(data);
			binaryBlob->compressed = false;

			binaryBlobs.push_back(binaryBlob);
		}

		// Index array.
		binaryBlobs.push_back(CreateBlobFromIndexArray(currentLODLevel.indexArray, currentLODLevel.numIndices));
	}
}

