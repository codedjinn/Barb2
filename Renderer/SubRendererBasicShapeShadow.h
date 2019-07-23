#ifndef SUBRENDERERBASICSHAPESHADOW_H_INCLUDED
#define SUBRENDERERBASICSHAPESHADOW_H_INCLUDED

#include "SubRenderer.h"
#include "RenderToken.h"
#include "ShaderProgram.h"
#include <vector>
#include <string>

namespace RendererLib
{

class SubRendererBasicShapeShadow :
	public SubRenderer
{
	public:

		SubRendererBasicShapeShadow(Renderer& renderer, const std::vector<RenderToken*>& set, const MathLib::matrix4x4& lightMatrix);

		virtual void SetContextState();

		virtual void Render();

		virtual void RestoreContextState();

	private:

		const std::vector<RenderToken*>& renderableSet;

		MathLib::matrix4x4 shadowBasisWorldProjectionMatrix;

		float matrixArray[16];

		ShaderProgram* shader;

		void RenderExternalSphere();
		void RenderInternalSphere();
		void RenderExternalBox();
		void RenderInternalBox();
		void RenderCapsule(const MathLib::vector4& position, const MathLib::quaternion& orientation, float halfHeight, float radius);
};

}

#endif // SUBRENDERERBASICSHAPESHADOW_H_INCLUDED