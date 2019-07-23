#ifndef MATHLIB_QUATERNIONMATRIXOPS_H_INCLUDED
#define MATHLIB_QUATERNIONMATRIXOPS_H_INCLUDED

#include "MathLibCommon.h"
#include "MathLib_Quaternion.h"
#include "MathLib_Matrix4x4.h"

namespace MathLib
{

MATHLIB_INLINE void quaternion_toMatrix(const quaternion& q, matrix4x4& matrix)
{
    matrix._00 = 1.0f - 2.0f * ((q.y * q.y) + (q.z * q.z));
    matrix._01 = 2.0f * ((q.x * q.y) - (q.w * q.z));
    matrix._02 = 2.0f * ((q.x * q.z) + (q.w * q.y));void quaternion_toMatrix(const quaternion& q, matrix4x4& matrix);
    matrix._03 = 0.0f;

    matrix._10 = 2.0f * ((q.x * q.y) + (q.w * q.z));
    matrix._11 = 1.0f - 2.0f * ((q.x * q.x) + (q.z * q.z));
    matrix._12 = 2.0f * ((q.y * q.z) - (q.w * q.x));
    matrix._13 = 0.0f;

    matrix._20 = 2.0f * ((q.x * q.z) - (q.w * q.y));
    matrix._21 = 2.0f * ((q.y * q.z) + (q.w * q.x));
    matrix._22 = 1.0f - 2.0f * ((q.x * q.x) + (q.y * q.y));
    matrix._23 = 0.0f;

    matrix._30 = 0.0f;
    matrix._31 = 0.0f;
    matrix._32 = 0.0f;
    matrix._33 = 1.0f;
}

}



#endif // MATHLIB_QUATERNIONMATRIXOPS_H_INCLUDED
