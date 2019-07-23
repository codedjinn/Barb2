#include "Frustum.h"

namespace CameraLib
{

const int FrustumConstants::FRUSTUM_PLANE_NEAR		= 0;
const int FrustumConstants::FRUSTUM_PLANE_FAR		= 1;
const int FrustumConstants::FRUSTUM_PLANE_LEFT		= 2;
const int FrustumConstants::FRUSTUM_PLANE_RIGHT		= 3;
const int FrustumConstants::FRUSTUM_PLANE_BOTTOM	= 4;
const int FrustumConstants::FRUSTUM_PLANE_TOP		= 5;

const int FrustumConstants::FRUSTUM_POINT_LTN	= 0;
const int FrustumConstants::FRUSTUM_POINT_RTN	= 1;
const int FrustumConstants::FRUSTUM_POINT_RBN	= 2;
const int FrustumConstants::FRUSTUM_POINT_LBN	= 3;
const int FrustumConstants::FRUSTUM_POINT_LTF	= 4;
const int FrustumConstants::FRUSTUM_POINT_RTF	= 5;
const int FrustumConstants::FRUSTUM_POINT_RBF	= 6;
const int FrustumConstants::FRUSTUM_POINT_LBF	= 7;

}