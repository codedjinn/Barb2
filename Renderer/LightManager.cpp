#include "LightManager.h"
#include "Light.h"
#include "DirectionalLight.h"
#include "Renderer.h"
#include <iostream>

namespace RendererLib
{

const int LightManager::QUALITY_LOW = 0;
const int LightManager::QUALITY_MEDIUM = 1;
const int LightManager::QUALITY_HIGH = 2;
const int LightManager::QUALITY_VERY_HIGH = 3;

const int LightManager::SHADOW_MAP_RESOLUTION_LOW = 512;
const int LightManager::SHADOW_MAP_RESOLUTION_MEDIUM = 1024;
const int LightManager::SHADOW_MAP_RESOLUTION_HIGH = 2048;
const int LightManager::SHADOW_MAP_RESOLUTION_VERY_HIGH = 4096;

LightManager* LightManager::lightManager = NULL;

LightManager& LightManager::GetInstance()
{
	if (NULL == LightManager::lightManager)
	{
		lightManager = new LightManager();
	}
	return *lightManager;
}

LightManager::LightManager()
{
	Initialize(LightManager::QUALITY_HIGH);
}

void LightManager::Initialize(int quality)
{
	if (LIGHTMANAGER_DEBUG)
	{
		std::cerr << "LightManager::creating frame buffer object" << std::endl;
	}

	shadowMapResolution = SHADOW_MAP_RESOLUTION_LOW;
	switch (quality)
	{
	case QUALITY_MEDIUM:
		shadowMapResolution = SHADOW_MAP_RESOLUTION_MEDIUM;
		break;

	case QUALITY_HIGH:
		shadowMapResolution = SHADOW_MAP_RESOLUTION_HIGH;
		break;

	case QUALITY_VERY_HIGH:
		shadowMapResolution = SHADOW_MAP_RESOLUTION_VERY_HIGH;
		break;
	}

	glGenFramebuffers(1, &fboHandle);

	// Create shadowMap0.
	glGenTextures(1, &shadowMap0);
	glBindTexture(GL_TEXTURE_2D, shadowMap0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32F, shadowMapResolution, shadowMapResolution, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

	// Create shadowMap1.
	glGenTextures(1, &shadowMap1);
	glBindTexture(GL_TEXTURE_2D, shadowMap1);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32F, shadowMapResolution, shadowMapResolution, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

	// Create shadowMap2.
	glGenTextures(1, &shadowMap2);
	glBindTexture(GL_TEXTURE_2D, shadowMap2);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32F, shadowMapResolution, shadowMapResolution, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

	// Create shadowMap3.
	glGenTextures(1, &shadowMap3);
	glBindTexture(GL_TEXTURE_2D, shadowMap3);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32F, shadowMapResolution, shadowMapResolution, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

	glBindFramebuffer(GL_FRAMEBUFFER, fboHandle);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, shadowMap0, 0);

	// Check FBO status.
	if (LIGHTMANAGER_DEBUG)
	{
		std::cerr << "Checking GBuffer FBO status: ";

		GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		switch (status)
		{
		case GL_FRAMEBUFFER_COMPLETE:
			std::cerr << "GL_FRAMEBUFFER_COMPLETE" << std::endl;
			break;

		case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
			std::cerr << "GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT" << std::endl;
			exit(1);
			break;

		case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT:
			std::cerr << "GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT" << std::endl;
			exit(1);
			break;

		case GL_FRAMEBUFFER_INCOMPLETE_FORMATS_EXT:
			std::cerr << "GL_FRAMEBUFFER_INCOMPLETE_FORMATS_EXT" << std::endl;
			exit(1);
			break;

		case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
			std::cerr << "GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER" << std::endl;
			exit(1);
			break;

		case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
			std::cerr << "GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER" << std::endl;
			exit(1);
			break;

		case GL_FRAMEBUFFER_UNSUPPORTED:
			std::cerr << "GL_FRAMEBUFFER_UNSUPPORTED" << std::endl;
			exit(1);
			break;

		case 0:
			std::cerr << "Function error" << std::endl;
			exit(1);
			break;
		}

		std::cerr << std::endl;
		std::cerr.flush();
	}

	// Unbind framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void LightManager::FreeMemory()
{
	if (LIGHTMANAGER_DEBUG)
	{
		std::cerr << "GBuffer::freeing resources ... ";
	}

	// Bind FBO
	glBindFramebuffer(GL_FRAMEBUFFER, fboHandle);

	// Delete buffers
	glDeleteTextures(1, &shadowMap0);
	glDeleteTextures(1, &shadowMap1);
	glDeleteTextures(1, &shadowMap2);
	glDeleteTextures(1, &shadowMap3);

	// Delete FBO objects
	glDeleteFramebuffers(1, &fboHandle);

	if (LIGHTMANAGER_DEBUG)
	{
		std::cerr << "done" << std::endl;
	}
}

void LightManager::BindForCascadeMap0()
{
	glBindFramebuffer(GL_FRAMEBUFFER, fboHandle);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);

	// Set viewport to shadow map resolution.
	glViewport(0,
			   0,
			   shadowMapResolution,
			   shadowMapResolution);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, shadowMap0, 0);
}

void LightManager::BindForCascadeMap1()
{
	glBindFramebuffer(GL_FRAMEBUFFER, fboHandle);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);

	// Set viewport to shadow map resolution.
	glViewport(0,
			   0,
			   shadowMapResolution,
			   shadowMapResolution);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, shadowMap1, 0);
}

void LightManager::BindForCascadeMap2()
{
	glBindFramebuffer(GL_FRAMEBUFFER, fboHandle);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);

	// Set viewport to shadow map resolution.
	glViewport(0,
			   0,
			   shadowMapResolution,
			   shadowMapResolution);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, shadowMap2, 0);
}
void LightManager::BindForCascadeMap3()
{
	glBindFramebuffer(GL_FRAMEBUFFER, fboHandle);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);

	// Set viewport to shadow map resolution.
	glViewport(0,
			   0,
			   shadowMapResolution,
			   shadowMapResolution);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, shadowMap3, 0);
}

void LightManager::Unbind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, fboHandle);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, 0, 0);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glDrawBuffer(GL_BACK);
	glReadBuffer(GL_BACK);

	// Restore renderer viewport.
	Renderer& rendererHandle = Renderer::GetInstance();
	glViewport(0,
			   0,
			   rendererHandle.GetViewportWidth(),
			   rendererHandle.GetViewportHeight());
}

int LightManager::GetShadowMapsResolution() const
{
	return shadowMapResolution;
}

}