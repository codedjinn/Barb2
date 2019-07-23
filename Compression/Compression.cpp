#include "Compression.h"
#include "miniz.h"
#include <cassert>

// disables following format specifier checking
// '<function>' : format string '<format-string>' requires an argument of type '<type>', but variadic argument <position> has type '<type>'
#pragma warning(push)
#pragma warning(disable: 4477)

using namespace CompressionLib;

bool Compression::CompressByteArray(const uint8_t* compressionTarget,
									const unsigned int long targetSize,
									uint8_t** compressedBuffer,
									unsigned long* compressedSize)
{
	size_t inputLength = static_cast<size_t>(targetSize);
	size_t outputLength;

	void* heapBuffer = tdefl_compress_mem_to_heap(reinterpret_cast<const void*>(compressionTarget),
												  inputLength,
												  &outputLength,
												  0);
	assert(NULL != heapBuffer);

	if (outputLength < inputLength)
	{
		if (COMPRESSION_DEBUG)
		{
			printf("Compression::Compressed %u bytes to %u bytes.\n", inputLength, outputLength);
		}

		// Now we need to copy the C compressed data to a C++ allocated array (free is not compatible with delete [],
		// which the client code will expect).
		uint8_t* returnBuffer = new uint8_t[outputLength];
		uint8_t* copyBuffer = reinterpret_cast<uint8_t*>(heapBuffer);

		for (unsigned int i = 0; i < outputLength; i++)
		{
			returnBuffer[i] = copyBuffer[i];
		}

		*compressedBuffer = returnBuffer;

		*compressedSize = static_cast<unsigned long>(outputLength);

		// Release miniz allocated memory
		free(heapBuffer);

		return true;
	}
	else
	{
		// Release miniz allocated memory
		free(heapBuffer);

		return false;
	}
}

void Compression::DecompressByteArray(const uint8_t* decompressionTarget,
									  const unsigned int long targetSize,
									  uint8_t** decompressedBuffer,
									  unsigned long* decompressedSize)
{
	size_t inputLength = static_cast<size_t>(targetSize);
	size_t outputLength;

	void* heapBuffer = tinfl_decompress_mem_to_heap(reinterpret_cast<const void*>(decompressionTarget),
													inputLength,
													&outputLength,
													0);
	if (NULL == heapBuffer)
	{
		assert(false);
	}

	if (COMPRESSION_DEBUG)
	{
		printf("Compression::Decompressed %u bytes to %u bytes\n", inputLength, outputLength);
	}

	// Now we need to copy the C decompressed data to a C++ allocated array (free is not compatible with delete
	// which the client code will expect).
	uint8_t* returnBuffer = new uint8_t[outputLength];
	uint8_t* copyBuffer = reinterpret_cast<uint8_t*>(heapBuffer);

	for (unsigned int i = 0; i < outputLength; i++)
	{
		returnBuffer[i] = copyBuffer[i];
	}

	*decompressedBuffer = returnBuffer;

	*decompressedSize = static_cast<unsigned long>(outputLength);

	// Release miniz allocated memory
	free(heapBuffer);
}
