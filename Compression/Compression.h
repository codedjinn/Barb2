#ifndef COMPRESSION_H_INCLUDED
#define COMPRESSION_H_INCLUDED

#include "CompressionCommon.h"
#include <inttypes.h>

namespace CompressionLib
{

/**
* Compression:
* This is a facade interface into one or more compression libraries, either in house or external.
* This is a static interface, you should never have instantiated a Compression instance.
*/
class Compression
{
	public:

		/**
		* Compresses the byte array.
		* @param compressionTarget	: Pointer to the base of the byte array you wish to compress.
		* @param targetSize			: The size of the byte array (in bytes).
		* @param compressedBuffer	: This will hold the compressed buffer after this function returns (AND ONLY IF IT RETURNS TRUE!)
		*							  The function will allocate the memory for you. After using it, you must manually
		* 							  free the memory.
		* @param compressedSize		: This will hold the size (in bytes) of the compressed buffer (ONLY IF IT RETURNS TRUE!)
		*
		* @return bool				: Returns whether or not the data could be compressed. (Sometimes, the data set is such
		*							  that it cannot be compressed successfully).
		*/
		static bool CompressByteArray(const uint8_t* compressionTarget,
									  const unsigned long targetSize,
									  uint8_t** compressedBuffer,
									  unsigned long* compressedSize);

		/**
		* Decompresses the byte array.
		* @param decompressionTarget	: Pointer to the base of the byte array you wish to decompress.
		* @param targetSize				: The size of the byte array (in bytes).
		* @param decompressedBuffer		: This will hold the decompressed buffer after this function returns.
		* 								  The function will allocate the memory for you. After using it, you must
		* 								  manually free the memory.
		* @param decompressedSize		: This will hold the size (in bytes) of the decompressed buffer.
		*/
		static void DecompressByteArray(const uint8_t* decompressionTarget,
										const unsigned long targetSize,
										uint8_t** decompressedBuffer,
										unsigned long* decompressedSize);

	protected:

		Compression();
};

}

#endif // COMPRESSION_H_INCLUDED
