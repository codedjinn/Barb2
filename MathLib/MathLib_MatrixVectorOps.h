#ifndef MATHLIB_MATRIXVECTOROPS_H_INCLUDED
#define MATHLIB_MATRIXVECTOROPS_H_INCLUDED

#include "MathLibCommon.h"
#include "MathLib_Vector4.h"
#include "MathLib_Matrix4x4.h"

namespace MathLib
{

MATHLIB_INLINE void matrix4x4_vectorMul(const matrix4x4& matrix, const vector4& vector, vector4& result)
/// Performs an multiplication of the given vector (column matrix) by the given matrix.
/// This method corresponds to the mathematical statement result = matrix * vector i.e the vector is a column vector.
/// This method is safe for when vector and result are the same object.
{
#if (MATHLIB_SIMD)
#if (MATHLIB_SSE)

	__m128 m0	= _mm_mul_ps(matrix.sse_row0, vector.sse_vec);
	__m128 m1	= _mm_mul_ps(matrix.sse_row1, vector.sse_vec);
	__m128 m2	= _mm_mul_ps(matrix.sse_row2, vector.sse_vec);
	__m128 m3	= _mm_mul_ps(matrix.sse_row3, vector.sse_vec);

	m0	= _mm_hadd_ps(m1, m0);
	m2	= _mm_hadd_ps(m3, m2);
	m0	= _mm_hadd_ps(m2, m0);

	result.sse_vec	= m0;

#endif // (MATHLIB_SSE)
#else

	float vectorX	= vector.extractX();
	float vectorY	= vector.extractY();
	float vectorZ	= vector.extractZ();
	float vectorW	= vector.extractW();

	result.setXYZW((matrix._03 * vectorW) + (matrix._02 * vectorZ) + (matrix._01 * vectorY) + (matrix._00 * vectorX),
				   (matrix._13 * vectorW) + (matrix._12 * vectorZ) + (matrix._11 * vectorY) + (matrix._10 * vectorX),
				   (matrix._23 * vectorW) + (matrix._22 * vectorZ) + (matrix._21 * vectorY) + (matrix._20 * vectorX),
				   (matrix._33 * vectorW) + (matrix._32 * vectorZ) + (matrix._31 * vectorY) + (matrix._30 * vectorX));

#endif // (MATHLIB_SIMD)
}

MATHLIB_INLINE void matrix4x4_vectorBatchMul(const matrix4x4& matrix, vector4 const * const vectorArray, uint32_t numVectors, vector4* const resultArray)
/// Performs a batch matrix vector multiplication (column matrix).
/// @note This method is suitable for when vectorArray and resultArray are the same array!
{
#if (MATHLIB_SIMD)
#if (MATHLIB_SSE)

	// Load matrix:
	__m128 matrix_row0	= matrix.sse_row0;
	__m128 matrix_row1	= matrix.sse_row1;
	__m128 matrix_row2	= matrix.sse_row2;
	__m128 matrix_row3	= matrix.sse_row3;

	for (uint32_t i = 0; i < numVectors; i++)
	{
		__m128 m0	= _mm_mul_ps(matrix_row0, vectorArray[i].sse_vec);
		__m128 m1	= _mm_mul_ps(matrix_row1, vectorArray[i].sse_vec);
		__m128 m2	= _mm_mul_ps(matrix_row2, vectorArray[i].sse_vec);
		__m128 m3	= _mm_mul_ps(matrix_row3, vectorArray[i].sse_vec);

		m0	= _mm_hadd_ps(m1, m0);
		m2	= _mm_hadd_ps(m3, m2);
		m0	= _mm_hadd_ps(m2, m0);

		resultArray[i].sse_vec	= m0;
	}

#endif // (MATHLIB_SSE)
#else

	for (uint32_t i = 0; i < numVectors; i++)
	{
		float vectorX	= vectorArray[i].extractX();
		float vectorY	= vectorArray[i].extractY();
		float vectorZ	= vectorArray[i].extractZ();
		float vectorW	= vectorArray[i].extractW();

		resultArray[i].setXYZW((matrix._03 * vectorW) + (matrix._02 * vectorZ) + (matrix._01 * vectorY) + (matrix._00 * vectorX),
							   (matrix._13 * vectorW) + (matrix._12 * vectorZ) + (matrix._11 * vectorY) + (matrix._10 * vectorX),
							   (matrix._23 * vectorW) + (matrix._22 * vectorZ) + (matrix._21 * vectorY) + (matrix._20 * vectorX),
							   (matrix._33 * vectorW) + (matrix._32 * vectorZ) + (matrix._31 * vectorY) + (matrix._30 * vectorX));
	}

#endif // (MATHLIB_SIMD)
}

}

#endif // MATHLIB_MATRIXVECTOROPS_H_INCLUDED
