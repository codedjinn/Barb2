#ifndef MEMORYALLOCATORALIGNED_H_INLCUDED
#define MEMORYALLOCATORALIGNED_H_INLCUDED

namespace Core
{

/// <summary>
/// Allocator which returns memory that has been aligned to 16-byte boundaries.
/// </summary>
class MemoryAllocatorAligned
{
	public:

		static void* Allocate(size_t size);

		static void Deallocate(void* memory);
};

}

#endif // MEMORYALLOCATORALIGNED_H_INLCUDED