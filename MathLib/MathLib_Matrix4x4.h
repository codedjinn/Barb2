/**
* This work is licensed under the Creative Commons Attribution 3.0 Unported License.
* To view a copy of this license, visit http://creativecommons.org/licenses/by/3.0/ or send a letter to Creative Commons,
* 444 Castro Street, Suite 900, Mountain View, California, 94041, USA.
*/

#ifndef MATHLIB_MATRIX4X4_H_INCLUDED
#define MATHLIB_MATRIX4X4_H_INCLUDED

#include "MathLibCommon.h"
#include "MathLib_Vector4.h"

namespace MathLib
{

class matrix4x4
{
    private:

        union
        {
            /// MathLib treats a SIMD register as [x y z w] so (for little endian platforms like x86) we store the x value in
            /// a higher memory location:
            struct
            {
                float _03; float _02; float _01; float _00;
                float _13; float _12; float _11; float _10;
                float _23; float _22; float _21; float _20;
                float _33; float _32; float _31; float _30;
            } MATHLIB_ALIGN(16);

#if (MATHLIB_SSE)

			struct
			{
				__m128 sse_row0;
				__m128 sse_row1;
				__m128 sse_row2;
				__m128 sse_row3;
			} MATHLIB_ALIGN(16);

#endif // (MATHLIB_SSE)
        };

    public:

        matrix4x4()
        {
        }

        matrix4x4(float m00, float m01, float m02, float m03,
                  float m10, float m11, float m12, float m13,
                  float m20, float m21, float m22, float m23,
                  float m30, float m31, float m32, float m33)
        {
            _03 = m03;
            _02 = m02;
            _01 = m01;
            _00 = m00;

            _13 = m13;
            _12 = m12;
            _11 = m11;
            _10 = m10;

            _23 = m23;
            _22 = m22;
            _21 = m21;
            _20 = m20;

            _33 = m33;
            _32 = m32;
            _31 = m31;
            _30 = m30;
        }

        matrix4x4(const matrix4x4& matrix)
        {
            matrix4x4_copy(*this, matrix);
        }

        MATHLIB_INLINE void loadIdentity()
        {
            _03 = 0.0f;
            _02 = 0.0f;
            _01 = 0.0f;
            _00 = 1.0f;

            _13 = 0.0f;
            _12 = 0.0f;
            _11 = 1.0f;
            _10 = 0.0f;

            _23 = 0.0f;
            _22 = 1.0f;
            _21 = 0.0f;
            _20 = 0.0f;

            _33 = 1.0f;
            _32 = 0.0f;
            _31 = 0.0f;
            _30 = 0.0f;
        }

		MATHLIB_INLINE void loadRotationX(float angle)
		/// Loads this matrix with the required elements to rotate around the x-axis by angle.
		/// Angle is expressed in degrees.
		{
			float angle_radians	= MATHLIB_DEG_TO_RAD(angle);
			float sinAngle		= sinf(angle_radians);
			float cosAngle		= cosf(angle_radians);

			_00	= 1.0f;
			_10	= 0.0f;
			_20	= 0.0f;
			_30	= 0.0f;

			_01	= 0.0f;
			_11	= cosAngle;
			_21	= sinAngle;
			_31	= 0.0f;

			_02	= 0.0f;
			_12	= -sinAngle;
			_22	= cosAngle;
			_32	= 0.0f;

			_03	= 0.0f;
			_13	= 0.0f;
			_23	= 0.0f;
			_33	= 1.0f;
		}

		MATHLIB_INLINE void loadRotationY(float angle)
		/// Loads this matrix with the required elements to required elements to rotate around the y-axis by angle.
		/// Angle is expressed in degrees.
		{
			float angle_radians	= MATHLIB_DEG_TO_RAD(angle);
			float sinAngle		= sinf(angle_radians);
			float cosAngle		= cosf(angle_radians);

			_00	= cosAngle;
			_10	= 0.0f;
			_20	= -sinAngle;
			_30	= 0.0f;

			_01	= 0.0f;
			_11	= 1.0f;
			_21	= 0.0f;
			_31	= 0.0f;

			_02	= sinAngle;
			_12	= 0.0f;
			_22	= cosAngle;
			_32	= 0.0f;

			_03	= 0.0f;
			_13	= 0.0f;
			_23	= 0.0f;
			_33	= 1.0f;
		}

		MATHLIB_INLINE void loadRotationZ(float angle)
		/// Loads this matrix with the required elements to required elements to rotate around the z-axis by angle.
		/// Angle is expressed in degrees.
		{
			float angle_radians	= MATHLIB_DEG_TO_RAD(angle);
			float sinAngle		= sinf(angle_radians);
			float cosAngle		= cosf(angle_radians);

			_00	= cosAngle;
			_10	= sinAngle;
			_20	= 0.0f;
			_30	= 0.0f;

			_01	= -sinAngle;
			_11	= cosAngle;
			_21	= 0.0f;
			_31	= 0.0f;

			_02	= 0.0f;
			_12	= 0.0f;
			_22	= 1.0f;
			_32	= 0.0f;

			_03	= 0.0f;
			_13	= 0.0f;
			_23	= 0.0f;
			_33	= 1.0f;
		}

		MATHLIB_INLINE void loadScale(float scaleX, float scaleY, float scaleZ)
		{
			_00	= scaleX;
			_01	= 0.0f;
			_02	= 0.0f;
			_03	= 0.0f;

			_10	= 0.0f;
			_11	= scaleY;
			_12	= 0.0f;
			_13	= 0.0f;

			_20	= 0.0f;
			_21	= 0.0f;
			_22	= scaleZ;
			_23	= 0.0f;

			_30	= 0.0f;
			_31	= 0.0f;
			_32	= 0.0f;
			_33	= 1.0f;
		}

		MATHLIB_INLINE void loadTranslation(float translationX, float translationY, float translationZ)
		{
			_00	= 1.0f;
			_01	= 0.0f;
			_02	= 0.0f;
			_03	= translationX;

			_10	= 0.0f;
			_11	= 1.0f;
			_12	= 0.0f;
			_13	= translationY;

			_20	= 0.0f;
			_21	= 0.0f;
			_22	= 1.0f;
			_23	= translationZ;

			_30	= 0.0f;
			_31	= 0.0f;
			_32	= 0.0f;
			_33	= 1.0f;
		}

        MATHLIB_INLINE void setElements(float m00, float m01, float m02, float m03,
										float m10, float m11, float m12, float m13,
										float m20, float m21, float m22, float m23,
										float m30, float m31, float m32, float m33)
        {
            _03 = m03;
            _02 = m02;
            _01 = m01;
            _00 = m00;

            _13 = m13;
            _12 = m12;
            _11 = m11;
            _10 = m10;

            _23 = m23;
            _22 = m22;
            _21 = m21;
            _20 = m20;

            _33 = m33;
            _32 = m32;
            _31 = m31;
            _30 = m30;
        };

		MATHLIB_INLINE void getPositionComponent(vector4& position) const
		/// Convenience method. Stores the position/translation component of the affine transform into the vector.
		{
			position.setXYZW(_03, _13, _23, 1.0f);
		}

        MATHLIB_INLINE void setOpenGLMatrix(float* array) const
		{
			// We have to do it this was as our elements are stored backwards:
			array[0]	= _00;
			array[1]	= _10;
			array[2]	= _20;
			array[3]	= _30;

			array[4]	= _01;
			array[5]	= _11;
			array[6]	= _21;
			array[7]	= _31;

			array[8]	= _02;
			array[9]	= _12;
			array[10]	= _22;
			array[11]	= _32;

			array[12]	= _03;
			array[13]	= _13;
			array[14]	= _23;
			array[15]	= _33;
		}

		friend void initMathLib();
		friend void matrix4x4_copy(matrix4x4& dest, const matrix4x4& src);
        friend void matrix4x4_add(const matrix4x4& matrix1, const matrix4x4& matrix2, matrix4x4& result);
        friend void matrix4x4_sub(const matrix4x4& matrix1, const matrix4x4& matrix2, matrix4x4& result);
        friend void matrix4x4_scalarMul(const matrix4x4& matrix, float scalar, matrix4x4& result);
        friend void matrix4x4_mul(const matrix4x4& matrix1, const matrix4x4& matrix2, matrix4x4& result);
        friend void matrix4x4_vectorMul(const matrix4x4& matrix, const vector4& vector, vector4& result);
        friend void matrix4x4_vectorBatchMul(const matrix4x4& matrix, vector4 const * const vectorArray, uint32_t numVectors, vector4* const resultArray);
        friend void matrix4x4_transpose(matrix4x4& matrix);
        friend float matrix4x4_determinant(const matrix4x4& matrix);
        friend bool matrix4x4_inverse(const matrix4x4& matrix, matrix4x4& result);
} MATHLIB_ALIGN(16);

MATHLIB_INLINE void matrix4x4_copy(matrix4x4& dest, const matrix4x4& src)
{
#if (MATHLIB_SIMD)
#if (MATHLIB_SSE)

    dest.sse_row0   = src.sse_row0;
    dest.sse_row1   = src.sse_row1;
    dest.sse_row2   = src.sse_row2;
    dest.sse_row3   = src.sse_row3;

#endif // (MATHLIB_SSE)
#else

    dest._03    = src._03;
    dest._02    = src._02;
    dest._01    = src._01;
    dest._00    = src._00;

    dest._13    = src._13;
    dest._12    = src._12;
    dest._11    = src._11;
    dest._10    = src._10;

    dest._23    = src._23;
    dest._22    = src._22;
    dest._21    = src._21;
    dest._20    = src._20;

    dest._33    = src._33;
    dest._32    = src._32;
    dest._31    = src._31;
    dest._30    = src._30;

#endif // (MATHLIB_SIMD)
}

MATHLIB_INLINE void matrix4x4_add(const matrix4x4& matrix1, const matrix4x4& matrix2, matrix4x4& result)
/// Performs a matrix addition and stores matrix sum in reuslt.
/// Corresponds to the mathematical statement result = matrix1 + matrix2
{
#if (MATHLIB_SIMD)
#if (MATHLIB_SSE)

	result.sse_row0 = _mm_add_ps(matrix1.sse_row0, matrix2.sse_row0);
	result.sse_row1 = _mm_add_ps(matrix1.sse_row1, matrix2.sse_row1);
	result.sse_row2 = _mm_add_ps(matrix1.sse_row2, matrix2.sse_row2);
	result.sse_row3 = _mm_add_ps(matrix1.sse_row3, matrix2.sse_row3);

#endif // (MATHLIB_SSE)
#else

	result._03  = matrix1._03 + matrix2._03;
	result._02  = matrix1._02 + matrix2._02;
	result._01  = matrix1._01 + matrix2._01;
	result._00  = matrix1._00 + matrix2._00;

	result._13  = matrix1._13 + matrix2._13;
	result._12  = matrix1._12 + matrix2._12;
	result._11  = matrix1._11 + matrix2._11;
	result._10  = matrix1._10 + matrix2._10;

	result._23  = matrix1._23 + matrix2._23;
	result._22  = matrix1._22 + matrix2._22;
	result._21  = matrix1._21 + matrix2._21;
	result._20  = matrix1._20 + matrix2._20;

	result._33  = matrix1._33 + matrix2._33;
	result._32  = matrix1._32 + matrix2._32;
	result._31  = matrix1._31 + matrix2._31;
	result._30  = matrix1._30 + matrix2._30;

#endif // (MATHLIB_SIMD)
}

MATHLIB_INLINE void matrix4x4_sub(const matrix4x4& matrix1, const matrix4x4& matrix2, matrix4x4& result)
/// Performs a matrix subtraction and stores the matrix difference in result.
/// Corresponds to the mathematical statement result = matrix1 - matrix2
{
#if (MATHLIB_SIMD)
#if (MATHLIB_SSE)

    result.sse_row0 = _mm_sub_ps(matrix1.sse_row0, matrix2.sse_row0);
    result.sse_row1 = _mm_sub_ps(matrix1.sse_row1, matrix2.sse_row1);
    result.sse_row2 = _mm_sub_ps(matrix1.sse_row2, matrix2.sse_row2);
    result.sse_row3 = _mm_sub_ps(matrix1.sse_row3, matrix2.sse_row3);

#endif // (MATHLIB_SSE)
#else

    result._03  = matrix1._03 - matrix2._03;
    result._02  = matrix1._02 - matrix2._02;
    result._01  = matrix1._01 - matrix2._01;
    result._00  = matrix1._00 - matrix2._00;

    result._13  = matrix1._13 - matrix2._13;
    result._12  = matrix1._12 - matrix2._12;
    result._11  = matrix1._11 - matrix2._11;
    result._10  = matrix1._10 - matrix2._10;

    result._23  = matrix1._23 - matrix2._23;
    result._22  = matrix1._22 - matrix2._22;
    result._21  = matrix1._21 - matrix2._21;
    result._20  = matrix1._20 - matrix2._20;

    result._33  = matrix1._33 - matrix2._33;
    result._32  = matrix1._32 - matrix2._32;
    result._31  = matrix1._31 - matrix2._31;
    result._30  = matrix1._30 - matrix2._30;

#endif // (MATHLIB_SIMD)
}

MATHLIB_INLINE void matrix4x4_scalarMul(const matrix4x4& matrix, float scalar, matrix4x4& result)
/// Performs a matrix-scalar multiplication and stores the scaled matrix in result.
/// Corresponds to the mathematical statement reuslt = matrix * scalar
{
#if (MATHLIB_SIMD)
#if (MATHLIB_SSE)

    __m128 scaleVec = _mm_load1_ps(&scalar);
    result.sse_row0 = _mm_mul_ps(matrix.sse_row0, scaleVec);
    result.sse_row1 = _mm_mul_ps(matrix.sse_row1, scaleVec);
    result.sse_row2 = _mm_mul_ps(matrix.sse_row2, scaleVec);
    result.sse_row3 = _mm_mul_ps(matrix.sse_row3, scaleVec);

#endif // (MATHLIB_SSE)
#else

    result._03  = matrix._03 * scalar;
    result._02  = matrix._02 * scalar;
    result._01  = matrix._01 * scalar;
    result._00  = matrix._00 * scalar;

    result._13  = matrix._13 * scalar;
    result._12  = matrix._12 * scalar;
    result._11  = matrix._11 * scalar;
    result._10  = matrix._10 * scalar;

    result._23  = matrix._23 * scalar;
    result._22  = matrix._22 * scalar;
    result._21  = matrix._21 * scalar;
    result._20  = matrix._20 * scalar;

    result._33  = matrix._33 * scalar;
    result._32  = matrix._32 * scalar;
    result._31  = matrix._31 * scalar;
    result._30  = matrix._30 * scalar;

#endif // (MATHLIB_SIMD)
}

MATHLIB_INLINE void matrix4x4_mul(const matrix4x4& matrix1, const matrix4x4& matrix2, matrix4x4& result)
/// Performs a matrix-matrix multiplication and stores the product in result.
/// Corresponds to the mathematical statement reuslt = matrix1 * matrix2
/// @warning The three matrices must be distinct, the result will be incorrect if result is the same object as matrix1 or matrix2.
{
#if (MATHLIB_SIMD)
#if (MATHLIB_SSE)

	__m128 row0;
	__m128 row1;
	__m128 row2;
	__m128 row3;
	__m128 value0;

	// Compute row 0 of the matrix product:
	value0	= _mm_shuffle_ps(matrix1.sse_row0, matrix1.sse_row0, _MM_SHUFFLE(3, 3, 3, 3));
	row0 	= _mm_mul_ps(matrix2.sse_row0, value0);

	value0	= _mm_shuffle_ps(matrix1.sse_row0, matrix1.sse_row0, _MM_SHUFFLE(2, 2, 2, 2));
	row0	= _mm_add_ps(row0, _mm_mul_ps(matrix2.sse_row1, value0));

	value0	= _mm_shuffle_ps(matrix1.sse_row0, matrix1.sse_row0, _MM_SHUFFLE(1, 1, 1, 1));
	row0	= _mm_add_ps(row0, _mm_mul_ps(matrix2.sse_row2, value0));

	value0	= _mm_shuffle_ps(matrix1.sse_row0, matrix1.sse_row0, _MM_SHUFFLE(0, 0, 0, 0));
	row0	= _mm_add_ps(row0, _mm_mul_ps(matrix2.sse_row3, value0));

	// Compute row 1 of the matrix product:
	value0	= _mm_shuffle_ps(matrix1.sse_row1, matrix1.sse_row1, _MM_SHUFFLE(3, 3, 3, 3));
	row1 	= _mm_mul_ps(matrix2.sse_row0, value0);

	value0	= _mm_shuffle_ps(matrix1.sse_row1, matrix1.sse_row1, _MM_SHUFFLE(2, 2, 2, 2));
	row1	= _mm_add_ps(row1, _mm_mul_ps(matrix2.sse_row1, value0));

	value0	= _mm_shuffle_ps(matrix1.sse_row1, matrix1.sse_row1, _MM_SHUFFLE(1, 1, 1, 1));
	row1	= _mm_add_ps(row1, _mm_mul_ps(matrix2.sse_row2, value0));

	value0	= _mm_shuffle_ps(matrix1.sse_row1, matrix1.sse_row1, _MM_SHUFFLE(0, 0, 0, 0));
	row1	= _mm_add_ps(row1, _mm_mul_ps(matrix2.sse_row3, value0));

	// Compute row 2 of the matrix product:
	value0	= _mm_shuffle_ps(matrix1.sse_row2, matrix1.sse_row2, _MM_SHUFFLE(3, 3, 3, 3));
	row2	= _mm_mul_ps(matrix2.sse_row0, value0);

	value0	= _mm_shuffle_ps(matrix1.sse_row2, matrix1.sse_row2, _MM_SHUFFLE(2, 2, 2, 2));
	row2	= _mm_add_ps(row2, _mm_mul_ps(matrix2.sse_row1, value0));

	value0	= _mm_shuffle_ps(matrix1.sse_row2, matrix1.sse_row2, _MM_SHUFFLE(1, 1, 1, 1));
	row2	= _mm_add_ps(row2, _mm_mul_ps(matrix2.sse_row2, value0));

	value0	= _mm_shuffle_ps(matrix1.sse_row2, matrix1.sse_row2, _MM_SHUFFLE(0, 0, 0, 0));
	row2	= _mm_add_ps(row2, _mm_mul_ps(matrix2.sse_row3, value0));

	// Compute row 3 of the matrix product:
	value0	= _mm_shuffle_ps(matrix1.sse_row3, matrix1.sse_row3, _MM_SHUFFLE(3, 3, 3, 3));
	row3 	= _mm_mul_ps(matrix2.sse_row0, value0);

	value0	= _mm_shuffle_ps(matrix1.sse_row3, matrix1.sse_row3, _MM_SHUFFLE(2, 2, 2, 2));
	row3	= _mm_add_ps(row3, _mm_mul_ps(matrix2.sse_row1, value0));

	value0	= _mm_shuffle_ps(matrix1.sse_row3, matrix1.sse_row3, _MM_SHUFFLE(1, 1, 1, 1));
	row3	= _mm_add_ps(row3, _mm_mul_ps(matrix2.sse_row2, value0));

	value0	= _mm_shuffle_ps(matrix1.sse_row3, matrix1.sse_row3, _MM_SHUFFLE(0, 0, 0, 0));
	row3	= _mm_add_ps(row3, _mm_mul_ps(matrix2.sse_row3, value0));

	// Write results back to memory:
	result.sse_row0	= row0;
	result.sse_row1	= row1;
	result.sse_row2	= row2;
	result.sse_row3	= row3;

#endif // (MATHLIB_SSE)
#else

	result._03	= (matrix1._03 * matrix2._33) + (matrix1._02 * matrix2._23) + (matrix1._01 * matrix2._13) + (matrix1._00 * matrix2._03);
	result._02	= (matrix1._03 * matrix2._32) + (matrix1._02 * matrix2._22) + (matrix1._01 * matrix2._12) + (matrix1._00 * matrix2._02);
	result._01	= (matrix1._03 * matrix2._31) + (matrix1._02 * matrix2._21) + (matrix1._01 * matrix2._11) + (matrix1._00 * matrix2._01);
	result._00	= (matrix1._03 * matrix2._30) + (matrix1._02 * matrix2._20) + (matrix1._01 * matrix2._10) + (matrix1._00 * matrix2._00);

	result._13	= (matrix1._13 * matrix2._33) + (matrix1._12 * matrix2._23) + (matrix1._11 * matrix2._13) + (matrix1._10 * matrix2._03);
	result._12	= (matrix1._13 * matrix2._32) + (matrix1._12 * matrix2._22) + (matrix1._11 * matrix2._12) + (matrix1._10 * matrix2._02);
	result._11	= (matrix1._13 * matrix2._31) + (matrix1._12 * matrix2._21) + (matrix1._11 * matrix2._11) + (matrix1._10 * matrix2._01);
	result._10	= (matrix1._13 * matrix2._30) + (matrix1._12 * matrix2._20) + (matrix1._11 * matrix2._10) + (matrix1._10 * matrix2._00);

	result._23	= (matrix1._23 * matrix2._33) + (matrix1._22 * matrix2._23) + (matrix1._21 * matrix2._13) + (matrix1._20 * matrix2._03);
	result._22	= (matrix1._23 * matrix2._32) + (matrix1._22 * matrix2._22) + (matrix1._21 * matrix2._12) + (matrix1._20 * matrix2._02);
	result._21	= (matrix1._23 * matrix2._31) + (matrix1._22 * matrix2._21) + (matrix1._21 * matrix2._11) + (matrix1._20 * matrix2._01);
	result._20	= (matrix1._23 * matrix2._30) + (matrix1._22 * matrix2._20) + (matrix1._21 * matrix2._10) + (matrix1._20 * matrix2._00);

	result._33	= (matrix1._33 * matrix2._33) + (matrix1._32 * matrix2._23) + (matrix1._31 * matrix2._13) + (matrix1._30 * matrix2._03);
	result._32	= (matrix1._33 * matrix2._32) + (matrix1._32 * matrix2._22) + (matrix1._31 * matrix2._12) + (matrix1._30 * matrix2._02);
	result._31	= (matrix1._33 * matrix2._31) + (matrix1._32 * matrix2._21) + (matrix1._31 * matrix2._11) + (matrix1._30 * matrix2._01);
	result._30	= (matrix1._33 * matrix2._30) + (matrix1._32 * matrix2._20) + (matrix1._31 * matrix2._10) + (matrix1._30 * matrix2._00);

#endif // (MATHLIB_SIMD)
}

MATHLIB_INLINE void matrix4x4_transpose(matrix4x4& matrix)
/// Transposes the given matrix.
{
#if (MATHLIB_SIMD)
#if (MATHLIB_SSE)

	__m128 value0;
	__m128 value1;
	__m128 value2;
	__m128 value3;

	// Transpose first two rows:
	value0	= _mm_movehl_ps(matrix.sse_row0, matrix.sse_row1);			// [m00][m01][m10][m11]
	value1	= _mm_movehl_ps(matrix.sse_row2, matrix.sse_row3);			// [m20][m21][m30][m31]
	value2	= _mm_shuffle_ps(value1, value0, _MM_SHUFFLE(3, 1, 3, 1));	// [m00][m10][m20][m30]
	value3	= _mm_shuffle_ps(value1, value0, _MM_SHUFFLE(2, 0, 2, 0));	// [m01][m11][m21][m31]

	// Load next two values:
	value0	= _mm_movelh_ps(matrix.sse_row0, matrix.sse_row1);			// [m12][m13][m02][m03]
	value1	= _mm_movelh_ps(matrix.sse_row2, matrix.sse_row3);			// [m32][m33][m22][m23]

	// Write results back to matrix:
	matrix.sse_row0	= value2;
	matrix.sse_row1	= value3;

	// Transpose last two rows:
	value2	= _mm_shuffle_ps(value1, value0, _MM_SHUFFLE(1, 3, 1, 3));	// [m02][m12][m22][m32]
	value3	= _mm_shuffle_ps(value1, value0, _MM_SHUFFLE(0, 2, 0, 2));	// [m03][m13][m23][m33]

	// Write results back to matrix:
	matrix.sse_row2	= value2;
	matrix.sse_row3	= value3;

#endif // (MATHLIB_SSE)
#else

	float temp;

	temp	= matrix._03;
	matrix._03	= matrix._30;
	matrix._30	= temp;

	temp	= matrix._02;
	matrix._02	= matrix._20;
	matrix._20	= temp;

	temp	= matrix._01;
	matrix._01	= matrix._10;
	matrix._10	= temp;

	temp	= matrix._13;
	matrix._13	= matrix._31;
	matrix._31	= temp;

	temp	= matrix._12;
	matrix._12	= matrix._21;
	matrix._21	= temp;

	temp	= matrix._23;
	matrix._23	= matrix._32;
	matrix._32	= temp;

#endif // (MATHLIB_SIMD)
}

MATHLIB_INLINE float matrix4x4_determinant(const matrix4x4& matrix)
/// Computes the determinant of the given matrix.
{
	float m22m33	= matrix._22 * matrix._33;
	float m23m32	= matrix._23 * matrix._32;
	float m21m33	= matrix._21 * matrix._33;
	float m23m31	= matrix._23 * matrix._31;
	float m21m32	= matrix._21 * matrix._32;
	float m22m31	= matrix._22 * matrix._31;
	float m20m33	= matrix._20 * matrix._33;
	float m23m30	= matrix._23 * matrix._30;
	float m20m32	= matrix._20 * matrix._32;
	float m22m30	= matrix._22 * matrix._30;
	float m20m31	= matrix._20 * matrix._31;
	float m21m30	= matrix._21 * matrix._30;

	return ((matrix._00 * (matrix._11 * (m22m33 - m23m32) - matrix._12 * (m21m33 - m23m31) + matrix._13 * (m21m32 - m22m31))) -
			(matrix._01 * (matrix._10 * (m22m33 - m23m32) - matrix._12 * (m20m33 - m23m30) + matrix._13 * (m20m32 - m22m30))) +
			(matrix._02 * (matrix._10 * (m21m33 - m23m31) - matrix._11 * (m20m33 - m23m30) + matrix._13 * (m20m31 - m21m30))) -
			(matrix._03 * (matrix._10 * (m21m32 - m22m31) - matrix._11 * (m20m32 - m22m30) + matrix._12 * (m20m31 - m21m30))));
}

MATHLIB_INLINE bool matrix4x4_inverse(const matrix4x4& matrix, matrix4x4& result)
/// If possible, this method computes the inverse of the provided matrix and stores it in result.
/// Will return true if the inverse was able to be computed (in which case result holds the inverse), or
/// returns false if the matrix is singular (in which case result holds an undefined matrix).
{
	// We need to compute the first row adjoint terms:
	float m22m33	= matrix._22 * matrix._33;
	float m23m32	= matrix._23 * matrix._32;
	float m21m33	= matrix._21 * matrix._33;
	float m23m31	= matrix._23 * matrix._31;
	float m21m32	= matrix._21 * matrix._32;
	float m22m31	= matrix._22 * matrix._31;
	float m20m33	= matrix._20 * matrix._33;
	float m23m30	= matrix._23 * matrix._30;
	float m20m32	= matrix._20 * matrix._32;
	float m22m30	= matrix._22 * matrix._30;
	float m20m31	= matrix._20 * matrix._31;
	float m21m30	= matrix._21 * matrix._30;

	// Compute first column of classical adjoint:
	float CA00	= +(matrix._11 * (m22m33 - m23m32) - matrix._12 * (m21m33 - m23m31) + matrix._13 * (m21m32 - m22m31));
	float CA10	= -(matrix._10 * (m22m33 - m23m32) - matrix._12 * (m20m33 - m23m30) + matrix._13 * (m20m32 - m22m30));
	float CA20	= +(matrix._10 * (m21m33 - m23m31) - matrix._11 * (m20m33 - m23m30) + matrix._13 * (m20m31 - m21m30));
	float CA30	= -(matrix._10 * (m21m32 - m22m31) - matrix._11 * (m20m32 - m22m30) + matrix._12 * (m20m31 - m21m30));

	// Compute determinant, if 0.0, matrix is singular and we return false:
	float determinant	= (matrix._00 * CA00) + (matrix._01 * CA10) + (matrix._02 * CA20) + (matrix._03 * CA30);
	if (0.0f == determinant)
		return (false);

	float determinant_recip	= 1.0f / determinant;

	// Compute second column of classical adjoint:
	float CA01	= -(matrix._01 * (m22m33 - m23m32) - matrix._02 * (m21m33 - m23m31) + matrix._03 * (m21m32 - m22m31));
	float CA11	= +(matrix._00 * (m22m33 - m23m32) - matrix._02 * (m20m33 - m23m30) + matrix._03 * (m20m32 - m22m30));
	float CA21	= -(matrix._00 * (m21m33 - m23m31) - matrix._01 * (m20m33 - m23m30) + matrix._03 * (m20m31 - m21m30));
	float CA31	= +(matrix._00 * (m21m32 - m22m31) - matrix._01 * (m20m32 - m22m30) + matrix._02 * (m20m31 - m21m30));

	// Compute third column of classical adjoint:
	float m12m33	= matrix._12 * matrix._33;
	float m13m32	= matrix._13 * matrix._32;
	float m11m33	= matrix._11 * matrix._33;
	float m13m31	= matrix._13 * matrix._31;
	float m11m32	= matrix._11 * matrix._32;
	float m12m31	= matrix._12 * matrix._31;
	float m10m33	= matrix._10 * matrix._33;
	float m13m30	= matrix._13 * matrix._30;
	float m10m32	= matrix._10 * matrix._32;
	float m12m30	= matrix._12 * matrix._30;
	float m10m31	= matrix._10 * matrix._31;
	float m11m30	= matrix._11 * matrix._30;

	float CA02	= +(matrix._01 * (m12m33 - m13m32) - matrix._02 * (m11m33 - m13m31) + matrix._03 * (m11m32 - m12m31));
	float CA12	= -(matrix._00 * (m12m33 - m13m32) - matrix._02 * (m10m33 - m13m30) + matrix._03 * (m10m32 - m12m30));
	float CA22	= +(matrix._00 * (m11m33 - m13m31) - matrix._01 * (m10m33 - m13m30) + matrix._03 * (m10m31 - m11m30));
	float CA32	= -(matrix._00 * (m11m32 - m12m31) - matrix._01 * (m10m32 - m12m30) + matrix._02 * (m10m31 - m11m30));

	// Compute fourth column of classical adjoint:
	float m12m23	= matrix._12 * matrix._23;
	float m13m22	= matrix._13 * matrix._22;
	float m11m23	= matrix._11 * matrix._23;
	float m13m21	= matrix._13 * matrix._21;
	float m11m22	= matrix._11 * matrix._22;
	float m12m21	= matrix._12 * matrix._21;
	float m10m23	= matrix._10 * matrix._23;
	float m13m20	= matrix._13 * matrix._20;
	float m10m22	= matrix._10 * matrix._22;
	float m12m20	= matrix._12 * matrix._20;
	float m10m21	= matrix._10 * matrix._21;
	float m11m20	= matrix._11 * matrix._20;

	float CA03	= -(matrix._01 * (m12m23 - m13m22) - matrix._02 * (m11m23 - m13m21) + matrix._03 * (m11m22 - m12m21));
	float CA13	= +(matrix._00 * (m12m23 - m13m22) - matrix._02 * (m10m23 - m13m20) + matrix._03 * (m10m22 - m12m20));
	float CA23	= -(matrix._00 * (m11m23 - m13m21) - matrix._01 * (m10m23 - m13m20) + matrix._03 * (m10m21 - m11m20));
	float CA33	= +(matrix._00 * (m11m22 - m12m21) - matrix._01 * (m10m22 - m12m20) + matrix._02 * (m10m21 - m11m20));

	result._00	= CA00 * determinant_recip;
	result._01	= CA01 * determinant_recip;
	result._02	= CA02 * determinant_recip;
	result._03	= CA03 * determinant_recip;

	result._10	= CA10 * determinant_recip;
	result._11	= CA11 * determinant_recip;
	result._12	= CA12 * determinant_recip;
	result._13	= CA13 * determinant_recip;

	result._20	= CA20 * determinant_recip;
	result._21	= CA21 * determinant_recip;
	result._22	= CA22 * determinant_recip;
	result._23	= CA23 * determinant_recip;

	result._30	= CA30 * determinant_recip;
	result._31	= CA31 * determinant_recip;
	result._32	= CA32 * determinant_recip;
	result._33	= CA33 * determinant_recip;

	return (true);
}

}

#endif // MATHLIB_MATRIX4X4_H_INCLUDED
