#include "Camera.h"
#include "Frustum.h"
#include <iostream>

namespace CameraLib
{

void Camera::CalculateFrustumPlanes()
{
	// Initialize the camera space plane array.
	float halfViewingAngle = cameraYFOV * 0.5f;

	float yOpposite = nearClipPlaneDistance * tanf(MATHLIB_DEG_TO_RAD(halfViewingAngle));
	float xOpposite = nearClipPlaneDistance * tanf(MATHLIB_DEG_TO_RAD(halfViewingAngle)) * aspectRatio;

	MathLib::vector4 cameraSpacePlanes[6];
	cameraSpacePlanes[FrustumConstants::FRUSTUM_PLANE_NEAR].setXYZW(0.0f, 0.0f, -1.0f, 0.0f);
	cameraSpacePlanes[FrustumConstants::FRUSTUM_PLANE_FAR].setXYZW(0.0f, 0.0f, 1.0f, 0.0f);
	cameraSpacePlanes[FrustumConstants::FRUSTUM_PLANE_LEFT].setXYZW(nearClipPlaneDistance, 0.0f, -xOpposite, 0.0f);
	cameraSpacePlanes[FrustumConstants::FRUSTUM_PLANE_RIGHT].setXYZW(-nearClipPlaneDistance, 0.0f, -xOpposite, 0.0f);
	cameraSpacePlanes[FrustumConstants::FRUSTUM_PLANE_BOTTOM].setXYZW(0.0f, nearClipPlaneDistance, -yOpposite, 0.0f);
	cameraSpacePlanes[FrustumConstants::FRUSTUM_PLANE_TOP].setXYZW(0.0f, -nearClipPlaneDistance, -yOpposite, 0.0f);

	MathLib::matrix4x4 worldToCameraSpaceMatrixTransposed;
	MathLib::matrix4x4_copy(worldToCameraSpaceMatrixTransposed, worldToCameraSpaceMatrix);
	MathLib::matrix4x4_transpose(worldToCameraSpaceMatrixTransposed);

	// Transform each of the planes.
	MathLib::matrix4x4_vectorBatchMul(worldToCameraSpaceMatrixTransposed, cameraSpacePlanes, 6, cameraSpacePlanes);

	// Assign the normal values to the furstum array (the plane takes care of normalizing them).
	for (unsigned int i = 0; i < 6; i++)
	{
		frustumPlanes[i].setNormal(cameraSpacePlanes[i]);
	}

	// Now we must calculate the points on the plane.
	// For the left, right, top and bottom planes, this is trivially the camera's world space position. But for the
	// near and far planes it's a little more complicated.
	frustumPlanes[FrustumConstants::FRUSTUM_PLANE_LEFT].setPointOnPlane(position);
	frustumPlanes[FrustumConstants::FRUSTUM_PLANE_RIGHT].setPointOnPlane(position);
	frustumPlanes[FrustumConstants::FRUSTUM_PLANE_BOTTOM].setPointOnPlane(position);
	frustumPlanes[FrustumConstants::FRUSTUM_PLANE_TOP].setPointOnPlane(position);

	MathLib::vector4 nearClipPlanePoint;
	MathLib::vector4 farClipPlanePoint;
	MathLib::vector4_addScaledVector(position, zAxis, -nearClipPlaneDistance, nearClipPlanePoint);
	MathLib::vector4_addScaledVector(position, zAxis, -farClipPlaneDistance, farClipPlanePoint);

	frustumPlanes[FrustumConstants::FRUSTUM_PLANE_NEAR].setPointOnPlane(nearClipPlanePoint);
	frustumPlanes[FrustumConstants::FRUSTUM_PLANE_FAR].setPointOnPlane(farClipPlanePoint);
}

MathLib::vector4* Camera::GetFrustumPoints(float startDistance, float endDistance)
{
	float halfViewingAngle = cameraYFOV * 0.5f;

	// Create frustum near plane.
	float yOppositeNear = startDistance * tanf(MATHLIB_DEG_TO_RAD(halfViewingAngle));
	float xOppositeNear = startDistance * tanf(MATHLIB_DEG_TO_RAD(halfViewingAngle)) * aspectRatio;

	frustumPoints[FrustumConstants::FRUSTUM_POINT_LTN].setXYZW(-xOppositeNear, yOppositeNear, -startDistance, 1.0f);
	frustumPoints[FrustumConstants::FRUSTUM_POINT_RTN].setXYZW(xOppositeNear, yOppositeNear, -startDistance, 1.0f);
	frustumPoints[FrustumConstants::FRUSTUM_POINT_RBN].setXYZW(xOppositeNear, -yOppositeNear, -startDistance, 1.0f);
	frustumPoints[FrustumConstants::FRUSTUM_POINT_LBN].setXYZW(-xOppositeNear, -yOppositeNear, -startDistance, 1.0f);
	
	// Create frustum far plane.
	float yOppositeFar = endDistance * tanf(MATHLIB_DEG_TO_RAD(halfViewingAngle));
	float xOppositeFar = endDistance * tanf(MATHLIB_DEG_TO_RAD(halfViewingAngle)) * aspectRatio;

	frustumPoints[FrustumConstants::FRUSTUM_POINT_LTF].setXYZW(-xOppositeFar, yOppositeFar, -endDistance, 1.0f);
	frustumPoints[FrustumConstants::FRUSTUM_POINT_RTF].setXYZW(xOppositeFar, yOppositeFar, -endDistance, 1.0f);
	frustumPoints[FrustumConstants::FRUSTUM_POINT_RBF].setXYZW(xOppositeFar, -yOppositeFar, -endDistance, 1.0f);
	frustumPoints[FrustumConstants::FRUSTUM_POINT_LBF].setXYZW(-xOppositeFar, -yOppositeFar, -endDistance, 1.0f);

	// Transform into world space.
	MathLib::matrix4x4_vectorBatchMul(cameraToWorldSpaceMatrix, frustumPoints, 8, frustumPoints);
	
	return frustumPoints;
}

void Camera::GetFrustumSliceBoundingSphere(float startDistance, float endDistance, float& sphereRadius, MathLib::vector4& spherePosition)
{
	float halfViewingAngle = cameraYFOV * 0.5f;

	MathLib::vector4 frustumPoint;
	
	// Create frustum far plane.
	float yOppositeFar = endDistance * tanf(MATHLIB_DEG_TO_RAD(halfViewingAngle));
	float xOppositeFar = endDistance * tanf(MATHLIB_DEG_TO_RAD(halfViewingAngle)) * aspectRatio;

	frustumPoint.setXYZW(-xOppositeFar, yOppositeFar, -endDistance, 1.0f);

	MathLib::vector4 frustumMidpoint(0.0f, 0.0f, -(startDistance + (endDistance - startDistance) * 0.5f), 1.0f);

	sphereRadius = MathLib::vector4_distance(frustumMidpoint, frustumPoint);

	MathLib::matrix4x4_vectorMul(cameraToWorldSpaceMatrix, frustumMidpoint, spherePosition);
}

MathLib::plane* Camera::GetFrustumPlanes()
{
	return frustumPlanes;
}

void Camera::Update()
{
	UpdateBasis();

    MathLib::vector4& camera_xAxis = GetXAxis();
    MathLib::vector4& camera_yAxis = GetYAxis();
    MathLib::vector4& camera_zAxis = GetZAxis();
    MathLib::vector4& camera_position = GetPosition();

	// Create world to camera space transform.
	{
		MathLib::matrix4x4 worldToCameraTransform_basisComponent
		(
			camera_xAxis.extractX(), camera_xAxis.extractY(), camera_xAxis.extractZ(), 0.0f,
			camera_yAxis.extractX(), camera_yAxis.extractY(), camera_yAxis.extractZ(), 0.0f,
			camera_zAxis.extractX(), camera_zAxis.extractY(), camera_zAxis.extractZ(), 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f
		);

		MathLib::matrix4x4 worldToCameraTransform_translationComponent
		(
			1.0f, 0.0f, 0.0f, -camera_position.extractX(),
			0.0f, 1.0f, 0.0f, -camera_position.extractY(),
			0.0f, 0.0f, 1.0f, -camera_position.extractZ(),
			0.0f, 0.0f, 0.0f, 1.0f
		);

		MathLib::matrix4x4 worldToCameraTransform;
		MathLib::matrix4x4_mul(worldToCameraTransform_basisComponent, worldToCameraTransform_translationComponent, worldToCameraTransform);
		MathLib::matrix4x4_copy(worldToCameraSpaceMatrix, worldToCameraTransform);
	}

	// Create camera to world space transform.
	{
		MathLib::matrix4x4 orientation
		(
			camera_xAxis.extractX(), camera_yAxis.extractX(), camera_zAxis.extractX(), 0.0f, 
			camera_xAxis.extractY(), camera_yAxis.extractY(), camera_zAxis.extractY(), 0.0f, 
			camera_xAxis.extractZ(), camera_yAxis.extractZ(), camera_zAxis.extractZ(), 0.0f, 
			0.0f, 0.0f, 0.0f, 1.0f
		);

		MathLib::matrix4x4 translation
		(
			1.0f, 0.0f, 0.0f, camera_position.extractX(), 
			0.0f, 1.0f, 0.0f, camera_position.extractY(), 
			0.0f, 0.0f, 1.0f, camera_position.extractZ(), 
			0.0f, 0.0f, 0.0f, 1.0f
		);

		MathLib::matrix4x4_mul(translation, orientation, cameraToWorldSpaceMatrix);
	}
}

void Camera::UpdateBasis()
{
	// Reset the basis vectors, and recalculate the rotations from scratch.
	// This is to avoid accumulative loss of orthogonality over time.
	xAxis.setXYZW(1.0f, 0.0f, 0.0f, 0.0f);
	yAxis.setXYZW(0.0f, 1.0f, 0.0f, 0.0f);
	zAxis.setXYZW(0.0f, 0.0f, 1.0f, 0.0f);

	PerformRotationXAxis(rotationAngleX);

	MathLib::matrix4x4 rotateAroundY;
	rotateAroundY.loadRotationY(rotationAngleY);
	MathLib::matrix4x4_vectorMul(rotateAroundY, xAxis, xAxis);
	MathLib::matrix4x4_vectorMul(rotateAroundY, yAxis, yAxis);
	MathLib::matrix4x4_vectorMul(rotateAroundY, zAxis, zAxis);

	PerformRotationZAxis(rotationAngleZ);
}

const MathLib::matrix4x4& Camera::GetPerspectiveProjectionMatrix(uint32_t displayWidth, uint32_t displayHeight)
{
    aspectRatio	= (float)displayWidth / (float)displayHeight;

    float top = nearClipPlaneDistance * tan(MATHLIB_DEG_TO_RAD(cameraYFOV * 0.5f));
	float right = top * aspectRatio;

	MathLib::matrix4x4 projectionMatrix;
	projectionMatrix.loadIdentity();
    projectionMatrix._00 = nearClipPlaneDistance / right;
    projectionMatrix._11 = nearClipPlaneDistance / top;
    projectionMatrix._22 = -(farClipPlaneDistance + nearClipPlaneDistance) / (farClipPlaneDistance - nearClipPlaneDistance);
    projectionMatrix._23 = (-2.0f * nearClipPlaneDistance * farClipPlaneDistance) / (farClipPlaneDistance - nearClipPlaneDistance);
	projectionMatrix._32 = -1.0f;
	projectionMatrix._33 = 0.0f;

	// Cache results
	MathLib::matrix4x4_copy(this->projectionMatrix, projectionMatrix);

    return this->projectionMatrix;
}

void Camera::GetNearPlaneExtents(float& xExtent, float& yExtent) const
{
	float halfViewingAngle = cameraYFOV * 0.5f;

	xExtent = nearClipPlaneDistance * tanf(MATHLIB_DEG_TO_RAD(halfViewingAngle)) * aspectRatio;
	yExtent = nearClipPlaneDistance * tanf(MATHLIB_DEG_TO_RAD(halfViewingAngle));
}

Camera& Camera::operator=(const Camera& rhs)
{
	rotationAngleX = rhs.rotationAngleX;
	rotationAngleY = rhs.rotationAngleY;
	rotationAngleZ = rhs.rotationAngleZ;

	nearClipPlaneDistance = rhs.nearClipPlaneDistance;
	farClipPlaneDistance = rhs.farClipPlaneDistance;

	MathLib::vector4_copy(position, rhs.position);

	cameraYFOV = rhs.cameraYFOV;

	aspectRatio = rhs.aspectRatio;

	return *this;
}

}

