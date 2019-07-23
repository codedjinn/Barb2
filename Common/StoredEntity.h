#ifndef STOREDENTITY_H_INCLUDED
#define STOREDENTITY_H_INCLUDED

#include <stdio.h>
#include <stdint.h>
#include <string>
#include <MathLib.h>

class StoredEntity
{
	public:

		/// <summary>
		/// Default constructor.
		/// </summary>
		StoredEntity();

		/// <summary>
		/// Constructor.
		/// </summary>
		/// <param name="entityType">The base type of the entity.</param>
		/// <param name="position">The world space position of the entity.</param>
		/// <param name="entityType">The orientation of the entity.</param>
		/// <param name="entityType">The scaling factors of the entity.</param>
		StoredEntity(const std::string& entityType,
					 const MathLib::vector4& position,
					 const MathLib::quaternion& orientation,
					 const MathLib::vector4& scale);

		/// <summary>
		/// Destructor.
		/// </summary>
		~StoredEntity();

		/// <summary>
		/// Convenience method to return the amount of space that this file will 
		/// consume on disk, in its current format.
		/// </summary>
		uint32_t SizeOnDisk() const;

		/// <summary>
		/// Saves the entity to disk.
		/// <param name="file">The open file to write the entity to.</param>
		/// </summary>
		void Save(FILE* file) const;

		/// <summary>
		/// Loads the entity from disk.
		/// <param name="file">The open file to load the entity from.</param>
		/// </summary.
		void Load(FILE* file);

		/// <summary>
		/// Returns the base type of this entity.
		/// </summary>
		std::string GetEntityType() const;

		/// <summary>
		/// Returns the position of the entity in world space.
		/// </summary>
		const MathLib::vector4& GetPosition() const;

		/// <summary>
		/// Returns the orientation of the entity in world space.
		/// </summary>
		const MathLib::quaternion& GetOrientation() const;

		/// <summary>
		/// Returns the scale factors of the entity.
		/// </summary>
		const MathLib::vector4& GetScale() const;

	protected:

		uint32_t m_EntityTypeLength;
		char* m_EntityType;

		MathLib::vector4 m_Position;
		MathLib::quaternion m_Orientation;
		MathLib::vector4 m_Scale;
};

#endif // STOREDENTITY_H_INCLUDED