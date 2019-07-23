/**
* This work is licensed under the Creative Commons Attribution 3.0 Unported License.
* To view a copy of this license, visit http://creativecommons.org/licenses/by/3.0/ or send a letter to Creative Commons,
* 444 Castro Street, Suite 900, Mountain View, California, 94041, USA.
*/

#include "MathLib.h"

static char mathLibInfo[1024];

namespace MathLib
{

#if (MATHLIB_SSE)
	__m128 vector4::vector4_clearW;
	__m128 vector4::vector4_setWTo1;
	__m128 vector4::vector4_negateVector;
#endif

void initMathLib()
{
	// Initialize the static variables:
#if (MATHLIB_SSE)
	// Set vector4_clearW fields:
	uint32_t* currentVal	= (uint32_t*)&vector4::vector4_clearW;
	*(currentVal++)	= 0x00000000;
	*(currentVal++)	= 0xffffffff;
	*(currentVal++)	= 0xffffffff;
	*(currentVal)	= 0xffffffff;

	// Set vector4_negateVector fields:
	currentVal	= (uint32_t*)&vector4::vector4_negateVector;
	*(currentVal++)	= 0x80000000;
	*(currentVal++)	= 0x80000000;
	*(currentVal++)	= 0x80000000;
	*(currentVal)	= 0x80000000;

	vector4::vector4_setWTo1	= _mm_set_ps(0.0f, 0.0f, 0.0f, 1.0f);
#endif // MATHLIB_SSE

	// Initialize the info string:
	// Operating system
	char mathLibOS[10];

#ifdef _WIN32
	sprintf_s(mathLibOS, "WINDOWS");
#else
    sprintf(mathLibInfo, "UNKNOWN");
#endif

	// SIMD acceleration
	char mathLibSIMD[100];

	if (MATHLIB_SIMD)
	{
		sprintf_s(mathLibSIMD, 100, "accelerated");
	}
	else
	{
		sprintf_s(mathLibSIMD, 100, "unaccelerated");
	}

	sprintf_s(mathLibInfo, 1024, "MathLib running on platform: [%s], SIMD acceleration: [%s].", mathLibOS, mathLibSIMD);
}

const char* getInfoString()
{
    return (mathLibInfo);
}

}
