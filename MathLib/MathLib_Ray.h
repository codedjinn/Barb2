/**
* This work is licensed under the Creative Commons Attribution 3.0 Unported License.
* To view a copy of this license, visit http://creativecommons.org/licenses/by/3.0/ or send a letter to Creative Commons,
* 444 Castro Street, Suite 900, Mountain View, California, 94041, USA.
*/

#ifndef MATHLIB_RAY_H_INCLUDED
#define MATHLIB_RAY_H_INCLUDED

#include "MathLibCommon.h"
#include "MathLib_Vector4.h"

namespace MathLib
{

class ray
{
	private:

		vector4 position;
		vector4 direction;

	public:

		ray()
		{
		}

		ray(const vector4& position, const vector4& direction)
		{
			vector4_copy(ray::position, position);
			vector4_copy(ray::direction, direction);
		}

		ray(float position_x, float position_y, float position_z,
			float direction_x, float direction_y, float direction_z)
		{
			position.setXYZW(position_x, position_y, position_z, 1.0f);
			direction.setXYZW(direction_x, direction_y, direction_z, 0.0f);
		}

		MATHLIB_INLINE void setPosition(const vector4& position)
		{
			vector4_copy(ray::position, position);
			vector4_setToPoint(ray::position);
		}

		MATHLIB_INLINE void setDirection(const vector4& direction)
		{
			vector4_copy(ray::direction, direction);
			vector4_setToVector(ray::direction);
		}

		MATHLIB_INLINE const vector4& getPosition() const
		{
			return (position);
		}

		MATHLIB_INLINE const vector4& getDirection() const
		{
			return (direction);
		}
} MATHLIB_ALIGN(16);

}

#endif // MATHLIB_RAY_H_INCLUDED
