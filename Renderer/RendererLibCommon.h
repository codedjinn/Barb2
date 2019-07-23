#ifndef RENDERERLIBCOMMON_H_INCLUDED
#define RENDERERLIBCOMMON_H_INCLUDED

#ifdef __GNUC__
#define RENDERER_INLINE inline
#define RENDERER_ALIGN(x) __attribute__ ((aligned (x)))	// Align to x byte boundaries. Must come AFTER the variable name.
#elif _MSC_VER
#define RENDERER_INLINE inline
#define RENDERER_ALIGN(x) __declspec(align(x))
typedef __int32 int32_t;
#else
#error RendererLib::Unsupported compiler!
#endif // __GNUC__

#include <gl/glew.h>
#include <gl/gl.h>
#include <gl/glu.h>

#endif // RENDERERLIBCOMMON_H_INCLUDED
