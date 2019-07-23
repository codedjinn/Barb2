#ifndef COMPRESSIONCOMMON_H_INCLUDED
#define COMPRESSIONCOMMON_H_INCLUDED

#ifdef __GNUC__
#define COMPRESSION_INLINE inline
#define COMPRESSION_ALIGN(x) __attribute__ ((aligned (x)))			/// Align to x byte boundaries. Must come after the variable name.
#elif _MSC_VER
#define COMPRESSION_INLINE inline
#else
#error Compression Library::Unsupported compiler!
#endif // __GNUC__

#undef __STRICT_ANSI__
#include <stdio.h>

/// Set for debug
#define COMPRESSION_DEBUG false

#endif // COMPRESSIONCOMMON_H_INCLUDED
