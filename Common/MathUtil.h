#ifndef MATHUTIL_H_INCLUDED
#define MATHUTIL_H_INCLUDED

#include "Common.h"

#include <math.h>

namespace CommonCode
{

class MathUtil
{
	public:

        static COMMON_INLINE int abs(int num)
        {
            if (num < 0)
            {
                num = -num;
            }
            return num;
        }

        // Need epsilon ?
        static COMMON_INLINE float absf(float num)
        {
            if (num < 0)
            {
                num = -num;
            }
            return num;
        }

		///<summary>
		/// Determines equality by using a tolerance value. Tolerance is 0.001f.
		///</summary>
		static COMMON_INLINE bool fEquals(float left, float right)
		{
			return fabs(left - right) < 0.001f;
		}

		///<summary>
		/// Determines equality by using a tolerance value.
		///</summary>
		/// <param name="tolerance">The amount of tolerance in the equality check</param>
		static COMMON_INLINE bool fEquals(float left, float right, float tolerance)
		{
			return (fabs(left - right) < tolerance);
		}

		///<summary>
		/// Quick distance function on a 2d plane.
		///</summary>
		static COMMON_INLINE float Distance2D(float x1, float y1, float x2, float y2)
		{
			float dx = x1 - x2;
			float dy = y1 - y2;
			return sqrtf( (dx * dx) + (dy * dy) );
		}
};

}

#endif // MATHUTIL_H_INCLUDED
