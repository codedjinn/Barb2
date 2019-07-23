#ifndef GEOMETRYLIBCOMMON_H_INCLUDED
#define GEOMETRYLIBCOMMON_H_INCLUDED

#ifdef __GNUC__
#define GEOMETRYLIB_INLINE inline
#define GEOMETRYLIB_ALIGN(x)  __attribute__ ((aligned(x)))		/// Align to x byte boundaries. Must come AFTER the variable name.
#elif _MSC_VER
#define GEOMETRYLIB_INLINE inline
#define GEOMETRYLIB_ALIGN(x) __declspec(align(x))
#else
#error GeometryLib::Unsupported Compiler!
#endif // __GNUC__

#define BOX_FACES_FRONT		0
#define BOX_FACES_LEFT		1
#define BOX_FACES_RIGHT		2
#define BOX_FACES_TOP		3
#define BOX_FACES_BOTTOM	4
#define BOX_FACES_BACK		5

#endif // GEOMETRYLIBCOMMON_H_INCLUDED
