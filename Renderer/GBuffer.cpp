#include "GBuffer.h"

#include <iostream>
#include <cstdlib>

using namespace RendererLib;

static const float GBUFFER_INTERNAL_SIZE = 1.0f;

GBuffer::GBuffer()
{
	if (GBUFFER_DEBUG)
	{
		std::cerr << "GBuffer::creating frame buffer object" << std::endl;
	}

	glGenFramebuffers(1, &FBOHandle);

	// Create normal buffer
	glGenTextures(1, &normalBuffer);
	glBindTexture(GL_TEXTURE_2D, normalBuffer);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, 1, 1, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

	// Create albedo buffer
	glGenTextures(1, &albedoBuffer);
	glBindTexture(GL_TEXTURE_2D, albedoBuffer);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

	// Create depth buffer
	glGenTextures(1, &depthBuffer);
    glBindTexture(GL_TEXTURE_2D, depthBuffer);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_DEPTH_TEXTURE_MODE_ARB, GL_INTENSITY);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_NONE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32F, 1, 1, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, 0);

	// Create the lighting result buffer
	glGenTextures(1, &lightingBuffer);
    glBindTexture(GL_TEXTURE_2D, lightingBuffer);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

	// Create the fog buffer.
	glGenTextures(1, &fogBuffer);
	glBindTexture(GL_TEXTURE_2D, fogBuffer);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

	// Create general buffers.
	glGenTextures(1, &generalBuffer0);
	glBindTexture(GL_TEXTURE_2D, generalBuffer0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

	glGenTextures(1, &generalBuffer1);
	glBindTexture(GL_TEXTURE_2D, generalBuffer1);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

	// Bind the FBO to attach the target buffers to it
	glBindFramebuffer(GL_FRAMEBUFFER, FBOHandle);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, albedoBuffer, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, normalBuffer, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthBuffer, 0);

	// Check FBO status
	if (GBUFFER_DEBUG)
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
	}

	// Unbind framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

GBuffer::~GBuffer()
{
	if (GBUFFER_DEBUG)
	{
		std::cerr << "GBuffer::freeing resources ... ";
	}

	// Bind FBO
	glBindFramebuffer(GL_FRAMEBUFFER, FBOHandle);

	// Delete buffers
	glDeleteTextures(1, &albedoBuffer);
	glDeleteTextures(1, &normalBuffer);
	glDeleteTextures(1, &depthBuffer);
	glDeleteTextures(1, &lightingBuffer);
	glDeleteTextures(1, &fogBuffer);
	glDeleteTextures(1, &generalBuffer0);
	glDeleteTextures(1, &generalBuffer1);

	// Delete FBO objects
	glDeleteFramebuffers(1, &FBOHandle);

	if (GBUFFER_DEBUG)
	{
		std::cerr << "done" << std::endl;
	}
}

void GBuffer::Resize(unsigned int width, unsigned int height)
{
	GBuffer::width = (unsigned int)(width * GBUFFER_INTERNAL_SIZE);
	GBuffer::height = (unsigned int)(height * GBUFFER_INTERNAL_SIZE);

	// Bind FBO.
	glBindFramebuffer(GL_FRAMEBUFFER, FBOHandle);

	// Delete buffers.
	glDeleteTextures(1, &albedoBuffer);
	glDeleteTextures(1, &normalBuffer);
	glDeleteTextures(1, &depthBuffer);
	glDeleteTextures(1, &lightingBuffer);

	// Create normal buffer.
	glGenTextures(1, &normalBuffer);
	glBindTexture(GL_TEXTURE_2D, normalBuffer);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, GBuffer::width, GBuffer::height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

	// Create albedo buffer.
	glGenTextures(1, &albedoBuffer);
	glBindTexture(GL_TEXTURE_2D, albedoBuffer);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, GBuffer::width, GBuffer::height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

	// Create depth buffer.
	glGenTextures(1, &depthBuffer);
    glBindTexture(GL_TEXTURE_2D, depthBuffer);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_DEPTH_TEXTURE_MODE_ARB, GL_INTENSITY);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_NONE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32F, GBuffer::width, GBuffer::height, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, 0);

	// Create the lighting result buffer.
	glGenTextures(1, &lightingBuffer);
    glBindTexture(GL_TEXTURE_2D, lightingBuffer);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, GBuffer::width, GBuffer::height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

	// Create the fog buffer.
	glGenTextures(1, &fogBuffer);
	glBindTexture(GL_TEXTURE_2D, fogBuffer);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, GBuffer::width, GBuffer::height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

	// Create general buffers.
	glGenTextures(1, &generalBuffer0);
	glBindTexture(GL_TEXTURE_2D, generalBuffer0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, GBuffer::width, GBuffer::height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

	glGenTextures(1, &generalBuffer1);
	glBindTexture(GL_TEXTURE_2D, generalBuffer1);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F,  GBuffer::width, GBuffer::height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

	// Unbind framebuffer.
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void GBuffer::Bind(GBufferBind mode) const
{
	Unbind();

	if (GBUFFER_MATERIAL_PASS == mode)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, FBOHandle);

		GLenum  drawBuffers[] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1};
		glDrawBuffers(2, drawBuffers);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, albedoBuffer, 0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, normalBuffer, 0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthBuffer, 0);
	}
	else if (GBUFFER_LIGHTING_PASS == mode)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, FBOHandle);

		glDrawBuffer(GL_COLOR_ATTACHMENT0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, lightingBuffer, 0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthBuffer, 0);
	}
	else if (GBUFFER_FOG_PASS == mode)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, FBOHandle);

		glDrawBuffer(GL_COLOR_ATTACHMENT0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, fogBuffer, 0);
	}
	else if (GBUFFER_MERGE_FOG_PASS == mode)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, FBOHandle);

		glDrawBuffer(GL_COLOR_ATTACHMENT0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, generalBuffer0, 0);
	}
	else if (GBUFFER_WRITE_TO_GENERAL_0 == mode)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, FBOHandle);
		
		glDrawBuffer(GL_COLOR_ATTACHMENT0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, generalBuffer0, 0);
	}
	else if (GBUFFER_WRITE_TO_GENERAL_1 == mode)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, FBOHandle);
		
		glDrawBuffer(GL_COLOR_ATTACHMENT0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, generalBuffer1, 0);
	}
	else
	{
		std::cerr << "GBuffer::Invalid bind mode!" << std::endl;
		exit(-1);
	}

	glViewport(0, 0, GBuffer::width, GBuffer::height);
}

void GBuffer::Unbind() const
{
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, 0, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, 0, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, 0, 0);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glDrawBuffer(GL_BACK);

	glViewport(0, 0, (GLsizei)(GBuffer::width / GBUFFER_INTERNAL_SIZE), (GLsizei)(GBuffer::height / GBUFFER_INTERNAL_SIZE));
}

GLuint GBuffer::GetAlbedoBufferHandle() const
{
	return albedoBuffer;
}

GLuint GBuffer::GetNormalBufferHandle() const
{
	return normalBuffer;
}

GLuint GBuffer::GetLightingBufferHandle() const
{
	return lightingBuffer;
}

GLuint GBuffer::GetDepthBufferHandle() const
{
	return depthBuffer;
}

GLuint GBuffer::GetFogBufferHandle() const
{
	return fogBuffer;
}

GLuint GBuffer::GetGeneralBuffer0Handle() const 
{
	return generalBuffer0;
}

GLuint GBuffer::GetGeneralBuffer1Handle() const 
{
	return generalBuffer1;
}