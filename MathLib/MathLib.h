/**
* This work is licensed under the Creative Commons Attribution 3.0 Unported License.
* To view a copy of this license, visit http://creativecommons.org/licenses/by/3.0/ or send a letter to Creative Commons,
* 444 Castro Street, Suite 900, Mountain View, California, 94041, USA.
*/

#ifndef MATHLIB_H_INCLUDED
#define MATHLIB_H_INCLUDED

#include <float.h>
#include "MathLibCommon.h"
#include "MathLib_Vector4.h"
#include "MathLib_Matrix4x4.h"
#include "MathLib_MatrixVectorOps.h"
#include "MathLib_Quaternion.h"
#include "MathLib_QuaternionMatrixOps.h"
#include "MathLib_Ray.h"
#include "MathLib_Plane.h"
#include "MathLib_PlaneOps.h"
#include "MathLibUtilities.h"

namespace MathLib
{

void initMathLib();
/// Initializes MathLib.
/// !! This function must be called before any MathLib functions are used. !!

const char* getInfoString();
/// Retrieves a char array with information regarding the current MathLib configuration.

}

#endif // MATHLIB_H_INCLUDED
