#include "DirectionalLightShadowBasis.h"
#include "LightManager.h"
#include "Renderer.h"
#include <Frustum.h>
#include <Camera.h>
#include <iostream>

namespace RendererLib
{

DirectionalLightShadowBasis::DirectionalLightShadowBasis()
{
}

DirectionalLightShadowBasis::DirectionalLightShadowBasis(const MathLib::vector4& lightDirection, float frustumEnclosingSphereRadius, const MathLib::vector4& frustumEnclosingSpherePosition)
{
	MathLib::vector4_copy(this->lightDirection, lightDirection);

	GenerateWvpMatrix(lightDirection, frustumEnclosingSphereRadius, frustumEnclosingSpherePosition);
}

void DirectionalLightShadowBasis::GenerateWvpMatrix(const MathLib::vector4& lightDirection, float frustumEnclosingSphereRadius, const MathLib::vector4& frustumEnclosingSpherePosition)
{
	using namespace CameraLib;

	MathLib::vector4 negativeLightDirection;
	MathLib::vector4_scale(lightDirection, -1.0f, negativeLightDirection);
	MathLib::vector4_normalize(negativeLightDirection);

	MathLib::vector4 centerPoint;
	centerPoint.setXYZW(0.0f, 1000.0f, 0.0f, 1.0f);

	MathLib::vector4 rightVector(-1.0f, 0.0f, 0.0f, 0.0f);

	MathLib::vector4 upVector;
	MathLib::vector4_crossProduct(negativeLightDirection, rightVector, upVector);
	MathLib::vector4_normalize(upVector);

	MathLib::vector4_crossProduct(upVector, negativeLightDirection, rightVector);
	MathLib::vector4_normalize(rightVector);

	// Now that we have an orthonormal basis. Generate the world to view and view to world matrices.
	MathLib::matrix4x4 viewWorldMatrix;
	MathLib::matrix4x4 worldViewMatrix;
	
	// Calculate the view-world matrix.
	{
		MathLib::matrix4x4 orientationMatrix
		(
			rightVector.extractX(), upVector.extractX(), negativeLightDirection.extractX(), 0.0f, 
			rightVector.extractY(), upVector.extractY(), negativeLightDirection.extractY(), 0.0f, 
			rightVector.extractZ(), upVector.extractZ(), negativeLightDirection.extractZ(), 0.0f, 
			0.0f, 0.0f, 0.0f, 1.0f
		);

		MathLib::matrix4x4 translationMatrix
		(
			1.0f, 0.0f, 0.0f, centerPoint.extractX(), 
			0.0f, 1.0f, 0.0f, centerPoint.extractY(), 
			0.0f, 0.0f, 1.0f, centerPoint.extractZ(), 
			0.0f, 0.0f, 0.0f, 1.0f
		);

		MathLib::matrix4x4_mul(translationMatrix, orientationMatrix, viewWorldMatrix);
	}

	// Calculate the world-view matrix.
	{
		MathLib::matrix4x4 invOrientationMatrix
		(
			rightVector.extractX(), rightVector.extractY(), rightVector.extractZ(), 0.0f, 
			upVector.extractX(), upVector.extractY(), upVector.extractZ(), 0.0f, 
			negativeLightDirection.extractX(), negativeLightDirection.extractY(), negativeLightDirection.extractZ(), 0.0f, 
			0.0f, 0.0f, 0.0f, 1.0f
		);

		MathLib::matrix4x4 invTranslationMatrix
		(
			1.0f, 0.0f, 0.0f, -centerPoint.extractX(), 
			0.0f, 1.0f, 0.0f, -centerPoint.extractY(), 
			0.0f, 0.0f, 1.0f, -centerPoint.extractZ(), 
			0.0f, 0.0f, 0.0f, 1.0f
		);

		MathLib::matrix4x4_mul(invOrientationMatrix, invTranslationMatrix, worldViewMatrix);
	}

	// Calculate the projection matrix.
	MathLib::matrix4x4 projectionMatrix;
	{
		float shadowMapResolution = (float)(LightManager::GetInstance().GetShadowMapsResolution());

		// Calculate the view space extents of the frustum points.
		float f = (frustumEnclosingSphereRadius * 2.0f) / (shadowMapResolution);
		
		MathLib::vector4 centerViewSpace;
		MathLib::matrix4x4_vectorMul(worldViewMatrix, frustumEnclosingSpherePosition, centerViewSpace);

		float minX = centerViewSpace.extractX() - frustumEnclosingSphereRadius;
		minX = floor(minX / f) * f;
		
		float minY = centerViewSpace.extractY() - frustumEnclosingSphereRadius;
		minY = floor(minY / f) * f;

		float viewportExtent = floor((frustumEnclosingSphereRadius * 2.0f) / f) * f;	// Ensure view point extents are a texel multiple.

		float maxX = minX + viewportExtent;
		float maxY = minY + viewportExtent;
		
		float minZ = -5000.0f;
		float maxZ = -1.0f;

		// Generate the projection matrix.
		{
			float& r = maxX;
			float& l = minX;
			float& t = maxY;
			float& b = minY;
			float& n = maxZ;
			float& f = minZ;

			nearClipPlaneDistance = -n;
			farClipPlaneDistance = -f;

			MathLib::matrix4x4 projectionMatrix
			(
				2.0f / (r - l), 0.0f, 0.0f, -(r + l) / (r - l), 
				0.0f, 2.0f / (t - b), 0.0f, -(t + b) / (t - b), 
				0.0f, 0.0f, 2.0f / (f - n), -(f + n) / (f - n), 
				0.0f, 0.0f, 0.0f, 1.0
			);

			// Now calculate the world-view-projection matrix.
			MathLib::matrix4x4_mul(projectionMatrix, worldViewMatrix, worldViewProjection);
		}

		// Now that we have the min and max of the viewing volume in view space we can.
		// Generate the world space viewing planes.
		{
			// Get planes in view space.
			MathLib::vector4 viewSpacePlaneNormals[6];
			MathLib::vector4 viewSpacePlanePositions[6];

			viewSpacePlaneNormals[FrustumConstants::FRUSTUM_PLANE_NEAR].setXYZW(0.0f, 0.0f, -1.0f, 0.0f);
			viewSpacePlanePositions[FrustumConstants::FRUSTUM_PLANE_NEAR].setXYZW(0.0f, 0.0f, maxZ, 1.0f);
			
			viewSpacePlaneNormals[FrustumConstants::FRUSTUM_PLANE_RIGHT].setXYZW(-1.0f, 0.0f, 0.0f, 0.0f);
			viewSpacePlanePositions[FrustumConstants::FRUSTUM_PLANE_RIGHT].setXYZW(maxX, 0.0f, 0.0f, 1.0f);

			viewSpacePlaneNormals[FrustumConstants::FRUSTUM_PLANE_LEFT].setXYZW(1.0f, 0.0f, 0.0f, 0.0f);
			viewSpacePlanePositions[FrustumConstants::FRUSTUM_PLANE_LEFT].setXYZW(minX, 0.0f, 0.0f, 1.0f);

			viewSpacePlaneNormals[FrustumConstants::FRUSTUM_PLANE_TOP].setXYZW(0.0f, -1.0f, 0.0f, 0.0f);
			viewSpacePlanePositions[FrustumConstants::FRUSTUM_PLANE_TOP].setXYZW(0.0f, maxY, 0.0f, 1.0f);

			viewSpacePlaneNormals[FrustumConstants::FRUSTUM_PLANE_BOTTOM].setXYZW(0.0f, 1.0f, 0.0f, 0.0f);
			viewSpacePlanePositions[FrustumConstants::FRUSTUM_PLANE_BOTTOM].setXYZW(0.0f, minY, 0.0f, 1.0f);

			viewSpacePlaneNormals[FrustumConstants::FRUSTUM_PLANE_FAR].setXYZW(0.0f, 0.0f, 1.0f, 0.0f);
			viewSpacePlanePositions[FrustumConstants::FRUSTUM_PLANE_FAR].setXYZW(0.0f, 0.0f, minZ, 1.0f);

			// The matrix we need to transform the normals of the planes is the transpose of the world to view matrix.
			MathLib::matrix4x4 viewWorldNormal;
			MathLib::matrix4x4_copy(viewWorldNormal, worldViewMatrix);
			MathLib::matrix4x4_transpose(viewWorldNormal);

			// Transform the points and vectors.
			MathLib::matrix4x4_vectorBatchMul(viewWorldMatrix, viewSpacePlanePositions, 6, viewSpacePlanePositions);
			MathLib::matrix4x4_vectorBatchMul(viewWorldNormal, viewSpacePlaneNormals, 6, viewSpacePlaneNormals);

			// Transfer to world space planes.
			for (int i = 0; i < 6; i++)
			{
				worldSpacePlanes[i].setNormal(viewSpacePlaneNormals[i]);
				worldSpacePlanes[i].setPointOnPlane(viewSpacePlanePositions[i]);
			}
		}	

		// Generate the world space frustum points.
		{
			// Get points in view space.
			worldSpaceFrustumPoints[FrustumConstants::FRUSTUM_POINT_LTN].setXYZW(minX, maxY, maxZ, 1.0f);
			worldSpaceFrustumPoints[FrustumConstants::FRUSTUM_POINT_RTN].setXYZW(maxX, maxY, maxZ, 1.0f);
			worldSpaceFrustumPoints[FrustumConstants::FRUSTUM_POINT_RBN].setXYZW(maxX, minY, maxZ, 1.0f);
			worldSpaceFrustumPoints[FrustumConstants::FRUSTUM_POINT_LBN].setXYZW(minX, minY, maxZ, 1.0f);
			worldSpaceFrustumPoints[FrustumConstants::FRUSTUM_POINT_LTF].setXYZW(minX, maxY, minZ, 1.0f);
			worldSpaceFrustumPoints[FrustumConstants::FRUSTUM_POINT_RTF].setXYZW(maxX, maxY, minZ, 1.0f);
			worldSpaceFrustumPoints[FrustumConstants::FRUSTUM_POINT_RBF].setXYZW(maxX, minY, minZ, 1.0f);
			worldSpaceFrustumPoints[FrustumConstants::FRUSTUM_POINT_LBF].setXYZW(minX, minY, minZ, 1.0f);

			// Transform into world space.
			MathLib::matrix4x4_vectorBatchMul(viewWorldMatrix, worldSpaceFrustumPoints, 8, worldSpaceFrustumPoints);
		}
	}
}

}