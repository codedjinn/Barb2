#ifndef GBUFFER_H_INCLUDED
#define GBUFFER_H_INCLUDED

#include "RendererLibCommon.h"

#define GBUFFER_DEBUG true

namespace RendererLib
{

enum GBufferBind
{
	GBUFFER_MATERIAL_PASS		= 0,
    GBUFFER_LIGHTING_PASS		= 1, 
	GBUFFER_FOG_PASS			= 2, 
	GBUFFER_MERGE_FOG_PASS		= 3, 
	GBUFFER_WRITE_TO_GENERAL_0	= 4, 
	GBUFFER_WRITE_TO_GENERAL_1	= 5
};

class GBuffer
{
	public:

		GBuffer();
		~GBuffer();

		/**
		* Resizes the GBuffer to the provided resolution. This should be called every time the viewport is
		* changed.
		*/
		void Resize(unsigned int width, unsigned int height);

		/**
		* Binds the appropriate buffers according to @param mode.
		*/
        void Bind(GBufferBind mode) const;
        void Unbind() const;

		GLuint GetAlbedoBufferHandle() const;
		GLuint GetNormalBufferHandle() const;
		GLuint GetLightingBufferHandle() const;
		GLuint GetDepthBufferHandle() const;
		GLuint GetFogBufferHandle() const;
		GLuint GetGeneralBuffer0Handle() const;
		GLuint GetGeneralBuffer1Handle() const;

		RENDERER_INLINE unsigned int GetWidth() const
		{
			return width;
		}

		RENDERER_INLINE unsigned int GetHeight() const
		{
			return height;
		}

	protected:

		GLuint FBOHandle;

		/**
		* Material buffers.
		*/
		GLuint albedoBuffer;		/// Per-pixel diffuse albedo information.
		GLuint normalBuffer;		/// Per-pixel normal information
		GLuint depthBuffer;			/// Per-pixel depth information.

		/**
		* Lighting buffer.
		*/
		GLuint lightingBuffer;

		/**
		* Post Processing buffers.
		*/
		GLuint fogBuffer;

		/**
		* General buffers, more merging results and doing chain computations etc.
		*/
		GLuint generalBuffer0;
		GLuint generalBuffer1;

		unsigned int width;
		unsigned int height;
};

}

#endif // GBUFFER_H_INCLUDED
