#ifndef MEMORYALLOCATORNAIVE_H_INCLUDED
#define MEMORYALLOCATORNAIVE_H_INCLUDED

namespace Core
{

/// <summary>
/// Basic memory allocator, memory returned has no alignment guarantees etc.
/// </summary>
class MemoryAllocatorNaive
{
	public:

		static void* Allocate(size_t size);
		static void Deallocate(void* memory);
};

}

#endif // MEMORYALLOCATORNAIVE_H_INCLUDED