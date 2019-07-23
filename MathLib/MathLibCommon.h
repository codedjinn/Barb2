/**
* This work is licensed under the Creative Commons Attribution 3.0 Unported License.
* To view a copy of this license, visit http://creativecommons.org/licenses/by/3.0/ or send a letter to Creative Commons,
* 444 Castro Street, Suite 900, Mountain View, California, 94041, USA.
*/

#ifndef MATHLIBCOMMON_H_INCLUDED
#define MATHLIBCOMMON_H_INCLUDED

#include <stdint.h>
#include <stdio.h>
#include <math.h>

typedef int system_int;
typedef unsigned int system_uint;

#ifdef __GNUC__
#define MATHLIB_INLINE inline
#define MATHLIB_ALIGN(x) __attribute__ ((aligned (x)))	// Align to x byte boundaries. Must come AFTER the variable name.
#elif _MSC_VER
#define MATHLIB_INLINE inline
#define MATHLIB_ALIGN(x) __declspec(align(x))
#else
#error MathLib::Unsupported compiler!
#endif // __GNUC__

#ifdef _WIN32   // Windows OS (x64 and x86)

//#undef __SSE__
#ifdef __SSE__				// On windows, we test if there is SSE support for SIMD acceleration.
#include <xmmintrin.h>
#include <pmmintrin.h>
#define MATHLIB_SIMD true
#define MATHLIB_SSE true
#else
#define MATHLIB_SIMD 0
#define MATHLIB_SSE 0
#endif // __SSE__

#elif __linux__ // Linux OS

#error MathLib::linux support not yet added!

#elif __APPLE__ // Mac OS

#error MathLib::apple support not yet added!

#else

#error MathLib::unknown operating system!
#endif // _WIN32

#define MATHLIB_PI         3.141593f
#define MATHLIB_PIdiv180   0.017453f
#define MATHLIB_180divPI   57.295789f

#define MATHLIB_DEG_TO_RAD(x) ((x) * MATHLIB_PIdiv180)
#define MATHLIB_RAD_TO_DEG(x) ((x) * MATHLIB_180divPI)

#endif // MATHLIBCOMMON_H_INCLUDED
