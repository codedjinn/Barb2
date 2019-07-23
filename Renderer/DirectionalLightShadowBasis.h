#ifndef _DIRECTIONALLIGHTSHADOWBASIS_H_
#define _DIRECTIONALLIGHTSHADOWBASIS_H_

#include "RendererLibCommon.h"
#include <MathLib.h>

namespace RendererLib
{

class DirectionalLightShadowBasis
{
	public:

		DirectionalLightShadowBasis();
		DirectionalLightShadowBasis(const MathLib::vector4& lightDirection, float frustumEnclosingSphereRadius, const MathLib::vector4& frustumEnclosingSpherePosition);
		
		RENDERER_INLINE MathLib::vector4 const * GetWorldSpaceFrustumPoints() const { return worldSpaceFrustumPoints; }

		RENDERER_INLINE const MathLib::matrix4x4& GetWorldViewProjection() const { return worldViewProjection; }

		RENDERER_INLINE float GetNearClipPlaneDistance() const { return nearClipPlaneDistance; }
		RENDERER_INLINE float GetFarClipPlaneDistance() const { return farClipPlaneDistance; }

		RENDERER_INLINE MathLib::plane const * GetFrustumPlanes() const { return worldSpacePlanes; }

		RENDERER_INLINE const MathLib::vector4& GetLightCameraPosition() const { return lightCameraPosition; }
		RENDERER_INLINE const MathLib::vector4& GetLightDirection() const { return lightDirection; }

		/// <summary>
		/// Generate the world view projection matrix for the given paramters.
		/// </summary>
		void GenerateWvpMatrix(const MathLib::vector4& lightDirection, float frustumEnclosingSphereRadius, const MathLib::vector4& frustumEnclosingSpherePosition);

	protected:

		/// <summary>
		/// The model view projection matrix for the current cascade.
		/// </summary>
		MathLib::matrix4x4 worldViewProjection;

		/// <summary>
		/// World space planes.
		/// </summary>
		MathLib::plane worldSpacePlanes[6];

		/// <summary>
		/// World space frustum points. Useful for debugging.
		/// </summary>
		MathLib::vector4 worldSpaceFrustumPoints[8];

		float nearClipPlaneDistance;
		float farClipPlaneDistance;

		MathLib::vector4 lightCameraPosition;
		MathLib::vector4 lightDirection;
};

}

#endif 