#ifndef SUBRENDERER_H_INCLUDED
#define SUBRENDERER_H_INCLUDED

#include "RendererLibCommon.h"
#include "Renderer.h"

namespace RendererLib
{


/**
* @brief SubRenderers are smaller renderer objects whose only focus is on drawing a particular rendering construct.
*
* There are various rendering constructs (things like sprites, meshes etc) that need to be rendered. Storing the rendering
* code for all of these in a single class leads to code-bloating and god-object anti-patterns. A SubRenderer is a class that
* is responsible for, and contains the rendering code for, rendering a particular construct.
*
* As an additional convention, SubRenderers contain no global or long lasting data. In fact, SubRenderers are transient objects,
* existing to execute rendering commands on given data, before expiring.
*/
class SubRenderer
{
	public:

		SubRenderer(Renderer& renderer);

		/**
		* Sets the GPU context state for this renderer.
		*/
		virtual void SetContextState() = 0;

		/**
		* Performs the rendering operation.
		*/
		virtual void Render() = 0;

		/**
		* Every functional block of code that modifies the OpenGL context state is required to restore that state after it is completed.
		* This is so that SubRenderers that perform many state changes don't complicate the state setting portions of following code.
		*/
		virtual void RestoreContextState() = 0;

	protected:

		Renderer& rendererHandle;
};

}

#endif // SUBRENDERER_H_INCLUDED
