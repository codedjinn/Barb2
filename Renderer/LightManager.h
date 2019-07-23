#ifndef LIGHTMANAGER_H_INCLUDED
#define LIGHTMANAGER_H_INCLUDED

#include "RendererLibCommon.h"
#include "DirectionalLightShadowBasis.h"
#include <vector>

#define LIGHTMANAGER_DEBUG	true

namespace RendererLib
{

class Light;
class DirectionalLight;

class LightManager
{
	public:

		static LightManager& GetInstance();

		std::vector<DirectionalLight*>& GetDirectionalLightList()
		{
			return directionalLightList;
		}
		
		void Initialize(int quality);
		void FreeMemory();

		void BindForCascadeMap0();
		void BindForCascadeMap1();
		void BindForCascadeMap2();
		void BindForCascadeMap3();

		void Unbind();

		int GetShadowMapsResolution() const;

		RENDERER_INLINE GLuint GetShadowMap0Handle() const { return shadowMap0; }
		RENDERER_INLINE GLuint GetShadowMap1Handle() const { return shadowMap1; }
		RENDERER_INLINE GLuint GetShadowMap2Handle() const { return shadowMap2; }
		RENDERER_INLINE GLuint GetShadowMap3Handle() const { return shadowMap3; }

		DirectionalLightShadowBasis& GetDirectionalLightShadowBasisCascade0() { return directionalLightShadowBasisCascade0; }
		DirectionalLightShadowBasis& GetDirectionalLightShadowBasisCascade1() { return directionalLightShadowBasisCascade1; }
		DirectionalLightShadowBasis& GetDirectionalLightShadowBasisCascade2() { return directionalLightShadowBasisCascade2; }
		DirectionalLightShadowBasis& GetDirectionalLightShadowBasisCascade3() { return directionalLightShadowBasisCascade3; }

		static const int QUALITY_LOW;
		static const int QUALITY_MEDIUM;
		static const int QUALITY_HIGH;
		static const int QUALITY_VERY_HIGH;

	protected:

		GLuint fboHandle;

		GLuint shadowMap0;
		GLuint shadowMap1;
		GLuint shadowMap2;
		GLuint shadowMap3;

		int shadowMapResolution;

		std::vector<DirectionalLight*> directionalLightList;

		DirectionalLightShadowBasis directionalLightShadowBasisCascade0;
		DirectionalLightShadowBasis directionalLightShadowBasisCascade1;
		DirectionalLightShadowBasis directionalLightShadowBasisCascade2;
		DirectionalLightShadowBasis directionalLightShadowBasisCascade3;
		
		LightManager();

		static LightManager* lightManager;
		
		static const int SHADOW_MAP_RESOLUTION_LOW;
		static const int SHADOW_MAP_RESOLUTION_MEDIUM;
		static const int SHADOW_MAP_RESOLUTION_HIGH;
		static const int SHADOW_MAP_RESOLUTION_VERY_HIGH;
};

}

#endif // LIGHTMANAGER_H_INCLUDED
