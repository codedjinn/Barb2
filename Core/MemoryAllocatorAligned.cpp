#include "MemoryAllocatorAligned.h"
#include <cstdlib>
#include <cassert>

namespace Core
{

void* MemoryAllocatorAligned::Allocate(size_t size)
{
	void* memory = _aligned_malloc(size, 16);
	assert(0 == (reinterpret_cast<size_t>(memory) & 15));

	return memory;
}

void MemoryAllocatorAligned::Deallocate(void* memory)
{
	_aligned_free(memory);
}

}