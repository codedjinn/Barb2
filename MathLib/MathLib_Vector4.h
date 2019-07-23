/**
* This work is licensed under the Creative Commons Attribution 3.0 Unported License.
* To view a copy of this license, visit http://creativecommons.org/licenses/by/3.0/ or send a letter to Creative Commons,
* 444 Castro Street, Suite 900, Mountain View, California, 94041, USA.
*/

#ifndef MATHLIB_VECTOR4_H_INCLUDED
#define MATHLIB_VECTOR4_H_INCLUDED

#include "MathLibCommon.h"

namespace MathLib
{

/// Forward declarations:
class matrix4x4;

class vector4
{
	private:

		union
		{
			float v[4];

#if (MATHLIB_SSE)
			__m128 sse_vec;
#endif // (MATHLIB_SSE)
		};

#if (MATHLIB_SSE)
		static __m128 vector4_clearW;
		static __m128 vector4_setWTo1;
		static __m128 vector4_negateVector;
#endif // (MATHLIB_SSE)

	public:

		vector4()
		{
		}

        vector4(float x, float y, float z, float w)
		{
			setXYZW(x, y, z, w);
		}

		vector4(const vector4& vector)
		{
            vector4_copy(*this, vector);
		}

		MATHLIB_INLINE void setX(float x)
		{
			v[3]	= x;
		}

		MATHLIB_INLINE void setY(float y)
		{
			v[2]	= y;
		}

		MATHLIB_INLINE void setZ(float z)
		{
			v[1]	= z;
		}

		MATHLIB_INLINE void setW(float w)
		{
			v[0]	= w;
		}

		MATHLIB_INLINE void setXYZ(float x, float y, float z)
		{
			v[3]	= x;
			v[2]	= y;
			v[1]	= z;
		}

		MATHLIB_INLINE void setXYZW(float x, float y, float z, float w)
		{
			v[3]	= x;
			v[2]	= y;
			v[1]	= z;
			v[0]	= w;
		}

		MATHLIB_INLINE float extractX() const
		{
			return (v[3]);
		}

		MATHLIB_INLINE float extractY() const
		{
			return (v[2]);
		}

		MATHLIB_INLINE float extractZ() const
		{
			return (v[1]);
		}

		MATHLIB_INLINE float extractW() const
		{
			return (v[0]);
		}

		friend void initMathLib();
		friend void vector4_setToPoint(vector4& vector);
		friend void vector4_setToVector(vector4& vector);
		friend void vector4_copy(vector4& dest, const vector4& src);
		friend void vector4_add(const vector4& vector1, const vector4& vector2, vector4& result);
		friend void vector4_sub(const vector4& vector1, const vector4& vector2, vector4& result);
		friend void vector4_scale(const vector4& vector1, float scaleFactor, vector4& result);
		friend void vector4_addScaledVector(const vector4& vector1, const vector4& vector2, float scaleFactor, vector4& result);
		friend float vector4_magnitude(const vector4& vector);
		friend void vector4_componentProduct(const vector4& vector1, const vector4& vector2, vector4& result);
		friend float vector4_dotProduct(const vector4& vector1, const vector4& vector2);
		friend void vector4_crossProduct(const vector4& vector1, const vector4& vector2, vector4& result);
		friend void vector4_normalize(vector4& vector);
		friend float vector4_distance(const vector4& vector1, const vector4& vector2);
		friend void vector4_negate(const vector4& vector, vector4& result);
		friend void matrix4x4_vectorMul(const matrix4x4& matrix, const vector4& vector, vector4& result);
		friend void matrix4x4_vectorBatchMul(const matrix4x4& matrix, vector4 const * const vectorArray, uint32_t numVectors, vector4* const resultArray);
} MATHLIB_ALIGN(16);

MATHLIB_INLINE void vector4_setToPoint(vector4& vector)
{
#if (MATHLIB_SIMD)
#if (MATHLIB_SSE)

	vector.sse_vec	= _mm_and_ps(vector.sse_vec, vector4::vector4_clearW);
	vector.sse_vec	= _mm_add_ps(vector.sse_vec, vector4::vector4_setWTo1);

#endif // (MATHLIB_SSE)
#else

	vector.setW(1.0f);

#endif // (MATHLIB_SIMD)
}

MATHLIB_INLINE void vector4_setToVector(vector4& vector)
{
#if (MATHLIB_SIMD)
#if (MATHLIB_SSE)

	vector.sse_vec	= _mm_and_ps(vector.sse_vec, vector4::vector4_clearW);

#endif // (MATHLIB_SSE)
#else

	vector.setW(0.0f);

#endif // (MATHLIB_SIMD)
}

MATHLIB_INLINE void vector4_copy(vector4& dest, const vector4& src)
{
#if (MATHLIB_SIMD)
#if (MATHLIB_SSE)

	_mm_store_ps(dest.v, src.sse_vec);

#endif // (MATHLIB_SSE)
#else

	dest.setXYZW(src.extractX(),
				 src.extractY(),
				 src.extractZ(),
				 src.extractW());

#endif // (MATHLIB_SIMD)
}

MATHLIB_INLINE void vector4_add(const vector4& vector1, const vector4& vector2, vector4& result)
/// Adds vector1 to vector2 and stores the result. This function is safe for when
/// vector1 is the same object as result.
{
#if (MATHLIB_SIMD)
#if (MATHLIB_SSE)

	result.sse_vec	= _mm_add_ps(vector1.sse_vec, vector2.sse_vec);

#endif // (MATHLIB_SSE)
#else

	result.setXYZW(vector1.extractX() + vector2.extractX(),
				   vector1.extractY() + vector2.extractY(),
				   vector1.extractZ() + vector2.extractZ(),
				   vector1.extractW() + vector2.extractW());

#endif // (MATHLIB_SIMD)
}

MATHLIB_INLINE void vector4_sub(const vector4& vector1, const vector4& vector2, vector4& result)
/// Subtracts vector2 from vector 1 and stores the result. This function is safe for when
/// either of the vectors are the same object as result.
{
#if (MATHLIB_SIMD)
#if (MATHLIB_SSE)

	result.sse_vec	= _mm_sub_ps(vector1.sse_vec, vector2.sse_vec);

#endif // (MATHLIB_SSE)
#else

	result.setXYZW(vector1.extractX() - vector2.extractX(),
				   vector1.extractY() - vector2.extractY(),
				   vector1.extractZ() - vector2.extractZ(),
				   vector1.extractW() - vector2.extractW());

#endif // (MATHLIB_SIMD)
}

MATHLIB_INLINE void vector4_scale(const vector4& vector1, float scaleFactor, vector4& result)
/// Scales vector1 by scaleFactor and stores the result. This function is safe for when
/// vector1 is the same object as result.
{
#if (MATHLIB_SIMD)
#if (MATHLIB_SSE)

	__m128 permutedFloatVec;
	permutedFloatVec	= _mm_load1_ps(&scaleFactor);
	result.sse_vec		= _mm_mul_ps(vector1.sse_vec, permutedFloatVec);

#endif // (MATHLIB_SSE)
#else

	result.setXYZW(vector1.extractX() * scaleFactor,
				   vector1.extractY() * scaleFactor,
				   vector1.extractZ() * scaleFactor,
				   vector1.extractW() * scaleFactor);

#endif // (MATHLIB_SIMD)
}

MATHLIB_INLINE void vector4_addScaledVector(const vector4& vector1, const vector4& vector2, float scaleFactor, vector4& result)
/// Adds the scaled vector2 to vector1 and stores the sum in result.
/// Corresponds to the expression result = vector1 + (scaleFactor * vector2).
/// This function is safe for when vector1 is the same object as result.
{
#if (MATHLIB_SIMD)
#if (MATHLIB_SSE)

	__m128 scaledVector;
	scaledVector	= _mm_load1_ps(&scaleFactor);
	scaledVector	= _mm_mul_ps(vector2.sse_vec, scaledVector);
	result.sse_vec	= _mm_add_ps(vector1.sse_vec, scaledVector);

#endif // (MATHLIB_SSE)
#else

	result.setXYZW(vector1.extractX() + (vector2.extractX() * scaleFactor),
				   vector1.extractY() + (vector2.extractY() * scaleFactor),
				   vector1.extractZ() + (vector2.extractZ() * scaleFactor),
				   vector1.extractW() + (vector2.extractW() * scaleFactor));

#endif // (MATHLIB_SIMD)
}

MATHLIB_INLINE float vector4_magnitude(const vector4& vector)
/// Returns the magnitude of the vector.
{
#if (MATHLIB_SIMD)
#if (MATHLIB_SSE)

	__m128 vector0;
	__m128 vector1;
	__m128 vector2;

	vector0	= _mm_mul_ps(vector.sse_vec, vector.sse_vec);					// Square the vector elements.
	vector1	= _mm_shuffle_ps(vector0, vector0, _MM_SHUFFLE(2, 1, 0, 3));	// Shuffle around, add elements
	vector2	= _mm_add_ps(vector0, vector1);									// (x * x) + (y * y)
	vector1	= _mm_shuffle_ps(vector0, vector0, _MM_SHUFFLE(1, 0, 3, 2));
	vector2	= _mm_add_ps(vector2, vector1);									// (x * x) + (y * y) + (z * z)
	vector2	= _mm_sqrt_ps(vector2);
	vector2	= _mm_shuffle_ps(vector2, vector2, _MM_SHUFFLE(3, 3, 3, 3));

	return (_mm_cvtss_f32(vector2));

#endif // (MATHLIB_SSE)
#else

	float x	= vector.extractX();
	float y = vector.extractY();
	float z = vector.extractZ();

	return (sqrtf((x * x) + (y * y) + (z * z)));

#endif // (MATHLIB_SIMD)
}

MATHLIB_INLINE void vector4_componentProduct(const vector4& vector1, const vector4& vector2, vector4& result)
/// Computes the component product of the two vectors and stores the result in result.
{
#if (MATHLIB_SIMD)
#if (MATHLIB_SSE)

	result.sse_vec	= _mm_mul_ps(vector1.sse_vec, vector2.sse_vec);

#endif // (MATHLIB_SSE)
#else

	float xresult	= vector1.extractX() * vector2.extractX();
	float yresult	= vector1.extractY() * vector2.extractY();
	float zresult	= vector1.extractZ() * vector2.extractZ();
	float wresult	= vector1.extractW() * vector2.extractW();

	result.setXYZW(xresult, yresult, zresult, wresult);

#endif // (MATHLIB_SIMD)
}

MATHLIB_INLINE float vector4_dotProduct(const vector4& vector1, const vector4& vector2)
/// Returns the dot product of the two vectors. This does not take the w component into account.
{
	/// Native FPU code will be faster here because the result will likely be used in a scalar
	/// codepath, so we should avoid an unnecessary store:
	float xcomp	= vector1.extractX() * vector2.extractX();
	float ycomp	= vector1.extractY() * vector2.extractY();
	float zcomp	= vector1.extractZ() * vector2.extractZ();

	return (xcomp + ycomp + zcomp);
}

MATHLIB_INLINE void vector4_crossProduct(const vector4& vector1, const vector4& vector2, vector4& result)
/// Computes the cross product of vector1 and vector2 and stores cross product in result.
{
#if (MATHLIB_SIMD)
#if (MATHLIB_SSE)

	result.sse_vec	= _mm_sub_ps(_mm_mul_ps(_mm_shuffle_ps(vector1.sse_vec, vector1.sse_vec, _MM_SHUFFLE(3, 1, 2, 0)), _mm_shuffle_ps(vector2.sse_vec, vector2.sse_vec, _MM_SHUFFLE(2, 3, 1, 0))),
								 _mm_mul_ps(_mm_shuffle_ps(vector1.sse_vec, vector1.sse_vec, _MM_SHUFFLE(2, 3, 1, 0)), _mm_shuffle_ps(vector2.sse_vec, vector2.sse_vec, _MM_SHUFFLE(3, 1, 2, 0))));

#endif // (MATHLIB_SSE)
#else

	float xcomp	= (vector1.extractY() * vector2.extractZ()) - (vector1.extractZ() * vector2.extractY());
	float ycomp	= (vector1.extractZ() * vector2.extractX()) - (vector1.extractX() * vector2.extractZ());
	float zcomp	= (vector1.extractX() * vector2.extractY()) - (vector1.extractY() * vector2.extractX());

	result.setXYZ(xcomp, ycomp, zcomp);

#endif // (MATHLIB_SIMD)
}

MATHLIB_INLINE void vector4_normalize(vector4& vector)
/// Normalizes the given vector.
{
#if (MATHLIB_SIMD)
#if (MATHLIB_SSE)

	__m128 vec0;
	__m128 vec1;

	vec0			= _mm_and_ps(vector.sse_vec, vector4::vector4_clearW);
	vec0			= _mm_mul_ps(vec0, vec0);
	vec1			= vec0;
	vec0			= _mm_shuffle_ps(vec0, vec0, _MM_SHUFFLE(2, 1, 0, 3));
	vec1			= _mm_add_ps(vec0, vec1);
	vec0			= vec1;
	vec1			= _mm_shuffle_ps(vec1, vec1, _MM_SHUFFLE(1, 0, 3, 2));
	vec0			= _mm_add_ps(vec0, vec1);
	vec0			= _mm_rsqrt_ps(vec0);
	vector.sse_vec	= _mm_mul_ps(vec0, vector.sse_vec);

#endif // (MATHLIB_SSE)
#else

	float vector_magnitude	= vector4_magnitude(vector);

	vector4_scale(vector, 1.0f / vector_magnitude, vector);

#endif // (MATHLIB_SIMD)
}

MATHLIB_INLINE float vector4_distance(const vector4& vector1, const vector4& vector2)
/// Returns the distance between the two vectors.
{
#if (MATHLIB_SIMD)
#if (MATHLIB_SSE)

	__m128 vec0;
	__m128 vec1;

	vec0	= _mm_sub_ps(vector2.sse_vec, vector1.sse_vec);
	vec0	= _mm_mul_ps(vec0, vec0);										// vec0 = (x * x) (y * y) (z * z) (w * w)
	vec1	= _mm_shuffle_ps(vec0, vec0, _MM_SHUFFLE(2, 1, 0, 3));			// Shuffle around, add elements.
	vec1	= _mm_add_ps(vec0, vec1);										// r3 = (x * x) + (y * y)
	vec0	= _mm_shuffle_ps(vec0, vec0, _MM_SHUFFLE(1, 0, 3, 2));			// Shuffle around, add elements.
	vec1	= _mm_add_ps(vec0, vec1);										// r3 = (x * x) + (y * y) + (z * z)
	vec1	= _mm_sqrt_ps(vec1);											// square root vector, shuffle and return

	return (_mm_cvtss_f32(_mm_shuffle_ps(vec1, vec1, _MM_SHUFFLE(2, 1, 0, 3))));

#endif // (MATHLIB_SSE)
#else

	float xcomp	= vector2.extractX() - vector1.extractX();
	float ycomp	= vector2.extractY() - vector1.extractY();
	float zcomp	= vector2.extractZ() - vector1.extractZ();

	return (sqrtf((xcomp * xcomp) + (ycomp * ycomp) + (zcomp * zcomp)));

#endif // (MATHLIB_SIMD)
}

MATHLIB_INLINE void vector4_negate(const vector4& vector, vector4& result)
/// Negates the vector and stores the result in result.
{
#if (MATHLIB_SIMD)
#if (MATHLIB_SSE)

	result.sse_vec	= _mm_xor_ps(vector.sse_vec, vector4::vector4_negateVector);

#endif // (MATHLIB_SSE)
#else

	result.setXYZ(-vector.extractX(),
				  -vector.extractY(),
				  -vector.extractZ());

#endif // (MATHLIB_SIMD)
}

}

#endif // MATHLIB_VECTOR4_H_INCLUDED
