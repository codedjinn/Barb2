#ifndef _FRUSTUM_H_INCLUDED
#define _FRUSTUM_H_INCLUDED

namespace CameraLib
{

class FrustumConstants
{
	public:

		// Frustum planes.
		static const int FRUSTUM_PLANE_NEAR;
		static const int FRUSTUM_PLANE_FAR;
		static const int FRUSTUM_PLANE_LEFT;
		static const int FRUSTUM_PLANE_RIGHT;
		static const int FRUSTUM_PLANE_TOP;
		static const int FRUSTUM_PLANE_BOTTOM;

		// Frustum points.
		static const int FRUSTUM_POINT_LTN;
		static const int FRUSTUM_POINT_RTN;
		static const int FRUSTUM_POINT_RBN;
		static const int FRUSTUM_POINT_LBN;
		static const int FRUSTUM_POINT_LTF;
		static const int FRUSTUM_POINT_RTF;
		static const int FRUSTUM_POINT_RBF;
		static const int FRUSTUM_POINT_LBF;
};

}

#endif // _FRUSTUM_H_INCLUDED