#ifndef COMMONTYPES_H_INCLUDED
#define COMMONTYPES_H_INCLUDED

#include "Common.h"

typedef unsigned char byte;

COMMON_INLINE byte EncodeUnitToByte(float unit)
{
	return (byte)(unit * 255.0f);
}

COMMON_INLINE float DecodeByteToUnit(byte unit)
{
    static const float unitDivider = 1.0f / 255.0f;

	return (float)unit * unitDivider;
}

#endif // COMMONTYPES_H_INCLUDED
