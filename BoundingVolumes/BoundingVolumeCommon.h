#ifndef BOUNDINGVOLUMECOMMON_H_INCLUDED
#define BOUNDINGVOLUMECOMMON_H_INCLUDED

#include <MathLib.h>

#ifdef __GNUC__
#define BOUNDINGVOLUMES_INLINE inline
#define BOUNDINGVOLUMES_ALIGN(x) __attribute__ ((aligned (x)))	// Align to x byte boundaries. Must come AFTER the variable name.
#elif _MSC_VER
#define BOUNDINGVOLUMES_INLINE inline
#define BOUNDINGVOLUMES_ALIGN(x) __declspec(align(x))
#else
#error BoundingVolumes::Unsupported compiler!
#endif // __GNUC__

#endif // BOUNDINGVOLUMECOMMON_H_INCLUDED
