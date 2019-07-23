#include "MemoryAllocatorNaive.h"
#include <cstdlib>
#include <cassert>

namespace Core
{

void* MemoryAllocatorNaive::Allocate(size_t size)
{
	void* memory = malloc(size);
	// void* memory = operator new(size);
	return memory;
}

void MemoryAllocatorNaive::Deallocate(void* memory)
{
	free(memory);
	// operator delete(memory);
}

}