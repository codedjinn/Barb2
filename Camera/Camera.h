#ifndef CAMERA_H_INCLUDED
#define CAMERA_H_INCLUDED

#include <MathLib.h>

#ifdef __GNUC__
#define CAMERA_INLINE inline
#define CAMERA_ALIGN(x) __attribute__ ((aligned (x)))	// Align to x byte boundaries. Must come AFTER the variable name.
#elif _MSC_VER
#define CAMERA_INLINE inline
#define CAMERA_ALIGN(x) __declspec(align(x))
#else
#error CameraLib::Unsupported compiler!
#endif // __GNUC__

#define CAMERA_DEFAULT_NEAR_CLIP_PLANE_DISTANCE	0.1f
#define CAMERA_DEFAULT_FAR_CLIP_PLANE_DISTANCE	1000.0f

namespace CameraLib
{

/// <summary>
/// Class that encapsulates all camera related functionality for the game.
/// </summary>
class Camera
{
	public:

		Camera()
		{
			xAxis.setXYZW(1.0f, 0.0f, 0.0f, 0.0f);
			yAxis.setXYZW(0.0f, 1.0f, 0.0f, 0.0f);
			zAxis.setXYZW(0.0f, 0.0f, 1.0f, 0.0f);

			position.setXYZW(0.0f, 0.0f, 0.0f, 1.0f);

			nearClipPlaneDistance = CAMERA_DEFAULT_NEAR_CLIP_PLANE_DISTANCE;
			farClipPlaneDistance = CAMERA_DEFAULT_FAR_CLIP_PLANE_DISTANCE;

			rotationAngleX = 0.0f;
			rotationAngleY = 0.0f;
			rotationAngleZ = 0.0f;

			cameraYFOV = 60.0f;
		}

		Camera(const Camera& camera)
		{
			*this = camera;
		}

		/// <summary>
		/// Updates the camera. This should be called whenever a camera's values
		/// have changed.
		/// </summary>
		void Update();

		/// <summary> 
		/// Resets the camera orientation back to 0 on all axes.
		/// </summary>
		CAMERA_INLINE void ResetCameraOrientation()
		{
			rotationAngleX = 0.0f;
			rotationAngleY = 0.0f;
			rotationAngleZ = 0.0f;
		}

		/// <summary>
		/// Moves the camera along the x axis by the specified distance.
		/// </summary>
		CAMERA_INLINE void MoveXAxis(float distance)
		{
			MathLib::vector4_addScaledVector(position, xAxis, distance, position);
		}

		/// <summary>
		/// Moves the camera along the y axis by the specified distance.
		/// </summary>
		CAMERA_INLINE void MoveYAxis(float distance)
		{
			MathLib::vector4_addScaledVector(position, yAxis, distance, position);
		}

		/// <summary>
		/// Moves the camera along the z axis by the specified distance.
		/// </summary>
		CAMERA_INLINE void MoveZAxis(float distance)
		{
            MathLib::vector4_addScaledVector(position, zAxis, distance, position);
		}

		/// <summary>
		/// Returns the x axis of the camera in world space.
		/// </summary>
		CAMERA_INLINE MathLib::vector4& GetXAxis()
		{
			return xAxis;
		}

		/// <summary>
		/// Returns the x axis of the camera in world space.
		/// </summary>
		CAMERA_INLINE const MathLib::vector4& GetXAxis() const
		{
			return xAxis;
		}

		/// <summary>
		/// Returns the y axis of the camera in world space.
		/// </summary>
		CAMERA_INLINE MathLib::vector4& GetYAxis()
		{
			return yAxis;
		}

		/// <summary>
		/// Returns the y axis of the camera in world space.
		/// </summary>
		CAMERA_INLINE const MathLib::vector4& GetYAxis() const
		{
			return yAxis;
		}

		/// <summary>
		/// Returns the z axis of the camera in world space.
		/// </summary>
		CAMERA_INLINE MathLib::vector4& GetZAxis()
		{
			return zAxis;
		}

		/// <summary>
		/// Returns the z axis of the camera in world space.
		/// </summary>
		CAMERA_INLINE const MathLib::vector4& GetZAxis() const
		{
			return zAxis;
		}

		/// <summary>
		/// Returns the position of the camera in world space.
		/// </summary>
		CAMERA_INLINE MathLib::vector4& GetPosition() 
		{
			return position;
		}

		/// <summary>
		/// Returns the position of the camera in world space.
		/// </summary>
		CAMERA_INLINE const MathLib::vector4& GetPosition() const 
		{
			return position;
		}

		/// <summary>
		/// Rotates the camera basis around the x axis by the angle, the angle is given in degrees.
		/// </summary>
		CAMERA_INLINE void RotateXAxis(float angle)
		{
			rotationAngleX += angle;
			rotationAngleX = fmodf(rotationAngleX, 360.0f);
		}

		/// <summary>
		/// Rotates the cameras basis around the y axis by the angle, the angle is given in degrees.
		/// </summary>
		CAMERA_INLINE void RotateYAxis(float angle)
		{
			rotationAngleY += angle;
			rotationAngleY = fmodf(rotationAngleY, 360.0f);
		}

		/// <summary>
		/// Rotates the cameras basis around the z axis by the angle, the angle is given in degrees.
		/// </summary>
		CAMERA_INLINE void RotatezAxis(float angle)
		{
			rotationAngleZ += angle;
			rotationAngleZ = fmodf(rotationAngleZ, 360.0f);
		}

		/// <summary>
		/// Sets the position of the camera in world space.
		/// </summary>
		CAMERA_INLINE void SetPosition(float x, float y, float z)
		{
			position.setXYZ(x, y, z);
		}

		/// <summary>
		/// Sets the distance to the near clip plane of the camera. 
		/// </summary>
		CAMERA_INLINE void SetNearClipPlaneDistance(float distance)
		{
			nearClipPlaneDistance = distance;
		}

		/// <summary>
		/// Sets the distance to the far clip plane of the camera.
		/// </summary>
		CAMERA_INLINE void SetFarClipPlaneDistance(float distance)
		{
			farClipPlaneDistance = distance;
		}

		/// <summary>
		/// Sets the field of view of the camera along the y axis.
		/// </summary>
		CAMERA_INLINE void SetCameraYFov(float yFOV)
		{
			cameraYFOV = yFOV;
		}

		/// <summary>
		/// Returns the distance to the near clip plane of the camera.
		/// </summary>
		CAMERA_INLINE float GetNearClipPlaneDistance() const
		{
			return nearClipPlaneDistance;
		}

		/// <summary>
		/// Returns the distance to the far clip plane of the camera.
		/// </summary>
		CAMERA_INLINE float GetFarClipPlaneDistance() const
		{
			return farClipPlaneDistance;
		}
		
		/// <summary>
		/// Returns the matrix required to transform a point from world space to camera space.
		/// </summary>
		const CAMERA_INLINE MathLib::matrix4x4& GetWorldSpaceToCameraSpaceTransform()
		{
            return worldToCameraSpaceMatrix;
		}

		/// <summary>
		/// Returns the matrix required to transform a point from camera space to world space.
		/// </summary>
		const CAMERA_INLINE MathLib::matrix4x4& GetCameraToWorldSpaceTransform()
		{
			return cameraToWorldSpaceMatrix;
		}
		
		/// <summary>
		/// Returns the field of view of the camera along the y axis.
		/// </summary>
		const CAMERA_INLINE float GetYFov() const { return cameraYFOV; }

		/// <summary>
		/// Returns the perspective projection transform of this camera, given the screen dimensions 
		/// it will be rendered on.
		/// </summary>
		const MathLib::matrix4x4& GetPerspectiveProjectionMatrix(uint32_t displayWidth, uint32_t displayHeight);

		/// <summary>
		/// Calculates the frustum planes used for clipping objects in world space.
		///
		/// NOTE: This method assumes you have an up to date projection matrix and world space to camera space transform cached.
		/// </summary>
		void CalculateFrustumPlanes();

		/// <summary> 
		/// Returns the frustum planes of this camera in world space.
		/// </summary>
		MathLib::plane* GetFrustumPlanes();

		/// <summary>
		/// Returns the frustum points, cut off at a a certain distance.
		/// </summary>
		MathLib::vector4* GetFrustumPoints(float startDistance, float endDistance);

		/// <summary>
		/// Returns the radius of a sphere that encapsulates a specified frustum slice.
		/// </summary>
		void GetFrustumSliceBoundingSphere(float startDistance, float endDistance, float& sphereRadius, MathLib::vector4& spherePosition);

		/// <summary>
		/// Returns the extents of the near clipping plane. 
		/// i.e the (x, y) of the upper right corner of the plane.
		/// </summary>
		void GetNearPlaneExtents(float& xExtent, float& yExtent) const;


		Camera& operator=(const Camera& rhs);

	protected:

		/// We define a standard OpenGL compatible camera system i.e the camera system is right handed with
		/// the negative-z direction defining the view direction.
		MathLib::vector4 xAxis;
		MathLib::vector4 yAxis;
		MathLib::vector4 zAxis;

		MathLib::vector4 position;

		/// When the GetPerspectiveProjectionMatrix() and GetWorldSpaceToCameraSpaceTransform() are called they cache their
		/// results in these matrices. Some methods like CalculateFrustumPlanes() use the chached results in their computations.
		MathLib::matrix4x4 cameraToWorldSpaceMatrix;
		MathLib::matrix4x4 worldToCameraSpaceMatrix;
		MathLib::matrix4x4 projectionMatrix;
		MathLib::plane frustumPlanes[6];
		MathLib::vector4 frustumPoints[8];

		float nearClipPlaneDistance;
		float farClipPlaneDistance;

		float rotationAngleX;
		float rotationAngleY;
		float rotationAngleZ;

		/// Defines the vertical field of view of this camera. Stored in degrees.
		float cameraYFOV;					

		float aspectRatio;

		void UpdateBasis();

		CAMERA_INLINE void PerformRotationXAxis(float angle)
		{
			float angleRads	= MATHLIB_DEG_TO_RAD(angle);
			float sinAngle = sinf(angleRads);
			float cosAngle = cosf(angleRads);

			// Caclulate modified y axis.
			MathLib::vector4 perpendicularContribution;
			MathLib::vector4_scale(zAxis, sinAngle, perpendicularContribution);
			MathLib::vector4_scale(yAxis, cosAngle, yAxis);
			MathLib::vector4_add(yAxis, perpendicularContribution, yAxis);

			// Calculate modified z axis.
			MathLib::vector4_crossProduct(xAxis, yAxis, zAxis);
		}

		CAMERA_INLINE void PerformRotationYAxis(float angle)
		{
			float angleRads	= MATHLIB_DEG_TO_RAD(angle);
			float sinAngle = sinf(angleRads);
			float cosAngle = cosf(angleRads);

			// Calculate modified z axis.
			MathLib::vector4 perpendicularContribution;
			MathLib::vector4_scale(xAxis, sinAngle, perpendicularContribution);
			MathLib::vector4_scale(zAxis, cosAngle, zAxis);
			MathLib::vector4_add(zAxis, perpendicularContribution, zAxis);

			// Calculate modified x axis.
			MathLib::vector4_crossProduct(yAxis, zAxis, xAxis);
		}

		CAMERA_INLINE void PerformRotationZAxis(float angle)
		{
			float angleRads	= MATHLIB_DEG_TO_RAD(angle);
			float sinAngle = sinf(angleRads);
			float cosAngle = cosf(angleRads);

			// Calculate modified y axis.
			MathLib::vector4 perpendicularContribution;
			MathLib::vector4_scale(xAxis, sinAngle, perpendicularContribution);
			MathLib::vector4_scale(yAxis, cosAngle, yAxis);
			MathLib::vector4_sub(yAxis, perpendicularContribution, yAxis);

			// Calculate modified x axis.
			MathLib::vector4_crossProduct(yAxis, zAxis, xAxis);
		}

} CAMERA_ALIGN(16);

}

#endif // CAMERA_H_INCLUDED
