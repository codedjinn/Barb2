#include "StoredEntity.h"
#include "AttributedBinaryBlob.h"
#include <MemoryAllocatorNaive.h>
#include <Compression.h>
#include <cassert>

enum StoredEntityAttributes
{
	ATTRIBUTE_IDENTIFIER	= 1,
	ATTRIBUTE_POSITION		= 2,
	ATTRIBUTE_ORIENTATION	= 3,
	ATTRIBUTE_SCALE			= 4
};

StoredEntity::StoredEntity()
{
    m_EntityType = nullptr;
    m_EntityTypeLength = 0;
}

StoredEntity::StoredEntity(const std::string& entityType,
						   const MathLib::vector4& position,
						   const MathLib::quaternion& orientation,
						   const MathLib::vector4& scale)
{
	m_EntityTypeLength = (uint32_t)(entityType.size() + 1);
	m_EntityType = new char[m_EntityTypeLength];
	std::strcpy(m_EntityType, entityType.c_str());

	MathLib::vector4_copy(m_Position, position);
	MathLib::quaternion_copy(m_Orientation, orientation);
	MathLib::vector4_copy(m_Scale, scale);
}

StoredEntity::~StoredEntity()
{
	if (NULL != m_EntityType)
		delete [] m_EntityType;
}

uint32_t StoredEntity::SizeOnDisk() const
{
	return (sizeof(m_EntityTypeLength)+
			m_EntityTypeLength +
			(sizeof(MathLib::vector4) * 2) +
			sizeof(MathLib::quaternion));
}

void StoredEntity::Save(FILE* file) const
{
	uint32_t numBinaryBlobs = 4;	// Entity, position, orientation, scale.
	fwrite(&numBinaryBlobs, sizeof(numBinaryBlobs), 1, file);

	// Entity Type blob.
	{
		AttributedBinaryBlob binaryBlob;
		binaryBlob.attribute = StoredEntityAttributes::ATTRIBUTE_IDENTIFIER;
		binaryBlob.blobSize = m_EntityTypeLength;
		binaryBlob.data = new uint8_t[m_EntityTypeLength];
		memcpy(binaryBlob.data, m_EntityType, m_EntityTypeLength);
		binaryBlob.compressed = false;
		binaryBlob.Save(file);
	}

	// Position blob.
	{
		AttributedBinaryBlob binaryBlob;
		binaryBlob.attribute = StoredEntityAttributes::ATTRIBUTE_POSITION;
		binaryBlob.blobSize = sizeof(MathLib::vector4);
		binaryBlob.data = new uint8_t[binaryBlob.blobSize];
		memcpy(binaryBlob.data, reinterpret_cast<const void*>(&m_Position), sizeof(MathLib::vector4));
		binaryBlob.compressed = false;
		binaryBlob.Save(file);
	}

	// Orientation blob.
	{
		AttributedBinaryBlob binaryBlob;
		binaryBlob.attribute = StoredEntityAttributes::ATTRIBUTE_ORIENTATION;
		binaryBlob.blobSize = sizeof(MathLib::quaternion);
		binaryBlob.data = new uint8_t[binaryBlob.blobSize];
		memcpy(binaryBlob.data, reinterpret_cast<const void*>(&m_Orientation), sizeof(MathLib::quaternion));
		binaryBlob.compressed = false;
		binaryBlob.Save(file);
	}

	// Scale blob.
	{
		AttributedBinaryBlob binaryBlob;
		binaryBlob.attribute = StoredEntityAttributes::ATTRIBUTE_SCALE;
		binaryBlob.blobSize = sizeof(MathLib::vector4);
		binaryBlob.data = new uint8_t[binaryBlob.blobSize];
		memcpy(binaryBlob.data, reinterpret_cast<const void*>(&m_Scale), sizeof(MathLib::vector4));
		binaryBlob.compressed = false;
		binaryBlob.Save(file);
	}
}

void StoredEntity::Load(FILE* file)
{
	uint32_t numBlobs;
	fread(&numBlobs, sizeof(uint32_t), 1, file);

	for (uint32_t i = 0; i < numBlobs; i++)
	{
		AttributedBinaryBlob blob;
		blob.Load(file);
		
		switch (blob.attribute)
		{
			case StoredEntityAttributes::ATTRIBUTE_IDENTIFIER:
				m_EntityTypeLength = blob.blobSize;
				m_EntityType = new char[m_EntityTypeLength];
				memcpy(m_EntityType, blob.data, m_EntityTypeLength);
				break;

			case StoredEntityAttributes::ATTRIBUTE_POSITION:
				memcpy(reinterpret_cast<void*>(&m_Position), blob.data, sizeof(m_Position));
				break;

			case StoredEntityAttributes::ATTRIBUTE_ORIENTATION:
				memcpy(reinterpret_cast<void*>(&m_Orientation), blob.data, sizeof(m_Orientation));
				break;

			case StoredEntityAttributes::ATTRIBUTE_SCALE:
				memcpy(reinterpret_cast<void*>(&m_Scale), blob.data, sizeof(m_Scale));
				break;
		}
	}
}

std::string StoredEntity::GetEntityType() const
{
	return std::string(m_EntityType);
}

const MathLib::vector4& StoredEntity::GetPosition() const
{
	return m_Position;
}

const MathLib::quaternion& StoredEntity::GetOrientation() const
{
	return m_Orientation;
}

const MathLib::vector4& StoredEntity::GetScale() const
{
	return m_Scale;
}
