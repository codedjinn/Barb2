#ifndef ATTRIBUTEDBINARYBLOB_H_INCLUDED
#define ATTRIBUTEDBINARYBLOB_H_INCLUDED

#include <inttypes.h>
#include <stdio.h>
#include "CommonTypes.h"

/**
* AttributeBinaryBlob:
* This is an atomic binary data structure that enables extensible binary files.
* An AttributedBinaryBlob is a block of binary data that has a context dependent (i.e assigned by the producing/consuming system)
* attribute that describes the data that is contained. It also has a compressed flag specifying whether the data is compressed or not.
* Binary files can be easily extended by simply writing more attributed binary blobs into them and the consuming system
* has the choice of whether to consume or ignore that particular blob.
*/
class AttributedBinaryBlob
{
	public:

		uint32_t attribute;

		bool compressed;

		uint32_t blobSize;
		uint8_t* data;

		void Save(FILE* fp) const;
		void Load(FILE* fp);

		AttributedBinaryBlob();
		~AttributedBinaryBlob();

		/**
		* Frees the data array and resets the blob to a blank state.
		*/
		void FreeMemory();

		/**
		* Convenience method to return the amount of space that this file will
		* consume on disk, in its current format.
		*/
		uint32_t SizeOnDisk() const;

	protected:
};


#endif // ATTRIBUTEDBINARYBLOB_H_INCLUDED
