#include <stdio.h>
#include "Compression.h"
#include <cstdlib>
#include <cassert>

int main(int argc, char** argv)
{
	using namespace CompressionLib;

	unsigned long arrayLength = 100000;
	float array[arrayLength];
	for (unsigned long i = 0; i < arrayLength; i++)
	{
		array[i] = i;
	}

	unsigned long arraySize = arrayLength * sizeof(float);

	char* compressedArray;
	unsigned long compressedArrayLength;

	if (Compression::CompressByteArray(reinterpret_cast<const uint8_t*>(array),
									   arraySize,
								       reinterpret_cast<uint8_t**>(&compressedArray),
								       &compressedArrayLength))
	{
		float* decompressedArray;
		unsigned long decompressedArrayLength;

		Compression::DecompressByteArray(reinterpret_cast<const uint8_t*>(compressedArray),
										 compressedArrayLength,
										 reinterpret_cast<uint8_t**>(&decompressedArray),
										 &decompressedArrayLength);

		// Compare arrays for validity
		for (unsigned int i = 0; i < arrayLength; i++)
		{
			assert(decompressedArray[i] == array[i]);
			//printf("%4.2f\n", decompressedArray[i]);
		}
	}
	else
	{
		printf("Sorry, the data wasn't compressed!\n");
	}

	return 0;
}
