#ifndef GENERICOBJECTPOOL_H_INCLUDED
#define GENERICOBJECTPOOL_H_INCLUDED

#include <new>
#include <stdint.h>
#include <assert.h>
#include "MemoryAllocatorNaive.h"

namespace Core
{

template <class TObject, class TMemoryAllocator=MemoryAllocatorNaive>
class GenericObjectPool
{
	public:

		GenericObjectPool(uint32_t size)
		{
			assert(size > 0);

			m_Size = size;

			m_Objects = (TObject*)TMemoryAllocator::Allocate(size * sizeof(TObject));

			m_FirstDeleted = NULL;

			m_NextObjectToAssign = 0;
		}

		~GenericObjectPool()
		{
			if (NULL != m_Objects)
				TMemoryAllocator::Deallocate(m_Objects);
		}

		/// <summary>
		/// Allocates a new object in the pool and returns it (the default constructor of the 
		/// object type is returned).
		/// If no more objects are available in the pool NULL is returned.
		/// </summary>
		TObject* Allocate()
		{
			TObject* object;

			if (NULL == m_FirstDeleted)
			{
				// Then there are no deleted objects available to reuse. Check if there is 
				// space to allocate a new object.
				//assert(m_NextObjectToAssign < m_Size);
				if (m_NextObjectToAssign < m_Size)
				{
					// There is space, allocate the new object.
					object = new (m_Objects + m_NextObjectToAssign) TObject();
					m_NextObjectToAssign++;

					return object;
				}

				// There is no more space available, return NULL.
				return NULL;
			}
			else 
			{
				object = m_FirstDeleted;
				m_FirstDeleted = *((TObject**)m_FirstDeleted);
				new (object) TObject();

				return object;
			}
		}

		/// <summary>
		/// Allocates a new object in the pool UNCONSTRUCTED and returns it.
		/// If no more objects are available in the pool NULL is returned.
		/// </summary>
		TObject* AllocateUnconstructed()
		{
			if (NULL == m_FirstDeleted)
			{
				// Then there are no deleted objects available to reuse. Check if there is 
				// space to allocate a new object.
				//assert(m_NextObjectToAssign < m_Size);
				if (m_NextObjectToAssign < m_Size)
					return m_Objects + m_NextObjectToAssign++;

				// There is no more space available, return NULL.
				return NULL;
			}
			else 
			{
				TObject* object = m_FirstDeleted;
				m_FirstDeleted = *((TObject**)m_FirstDeleted);
				
				return object;
			}
		}

		void Deallocate(TObject* object)
		{
			// Perform checks to see that the object is correct.
			uintptr_t objectAsInt = reinterpret_cast<uintptr_t>(object);
			uintptr_t objectArrayAsInt = reinterpret_cast<uintptr_t>(m_Objects);
			uintptr_t objectArrayEndBoundAsInt = objectArrayAsInt + (m_Size - 1) * sizeof(TObject);
			
			bool objectIsInPoolRange = (objectAsInt >= objectArrayAsInt) && (objectAsInt <= objectArrayEndBoundAsInt);
			bool objectIsAligned = ((objectAsInt - objectArrayAsInt) % sizeof(TObject) == 0);

			assert(objectIsInPoolRange && objectIsAligned);
			if (objectIsInPoolRange && objectIsAligned)
			{
				object->~TObject();

				(*(TObject**)object) = m_FirstDeleted;
				m_FirstDeleted = object;
			}
		}

	protected:

		uint32_t m_Size;
		uint32_t m_NextObjectToAssign;

		TObject* m_Objects;
		TObject* m_FirstDeleted;
};

}

#endif // GENERICOBJECTPOOL_H_INCLUDED