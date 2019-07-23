#ifndef TERRAINCOMMON_H_INCLUDED
#define TERRAINCOMMON_H_INCLUDED

#ifdef __GNUC__
#define TERRAIN_INLINE inline
#define TERRAIN_ALIGN(X) __attribute__ ((aligned (x)))		// Align to x byte boundaries. Must come AFTER the variable name.
#elif _MSC_VER
#define TERRAIN_INLINE inline
#define TERRAIN_ALIGN(x) __declspec(align(x))
#else
#error MathLib::Unsupported compiler!
#endif // __GNUC__

#include <MathLib.h>
#include <Geometry.h>

/// For the first version of the engine, the lanscape size is fixed to 4k x 4k.
#define TERRAIN_LANDSCAPE_SIZE	4096

/// Maximum height for the terrain. Range varies from 0 to TERRAIN_MAX_HEIGHT.
#define TERRAIN_MAX_HEIGHT	500


#endif // TERRAINCOMMON_H_INCLUDED
