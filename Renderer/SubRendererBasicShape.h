#ifndef SUBRENDERERBASICSHAPE_H_INCLUDED
#define SUBRENDERERBASICSHAPE_H_INCLUDED

#include "SubRenderer.h"
#include "RenderToken.h"
#include "ShaderProgram.h"
#include <vector>
#include <string>

namespace RendererLib
{

class SubRendererBasicShape :
	public SubRenderer
{
	public:

		SubRendererBasicShape(Renderer& renderer, const std::vector<RenderToken*>& set);

		virtual void SetContextState();

		virtual void Render();

		virtual void RestoreContextState();

	private:

		const std::vector<RenderToken*>& renderableSet;

		float matrixArray[16];

		ShaderProgram* shapeShader;
		GLuint attributeTexCoord0Index; 
		GLuint attributeNormalIndex;

		void RenderExternalSphere(std::vector<std::string>& faceTextures);
		void RenderInternalSphere(std::vector<std::string>& faceTextures);
		void RenderExternalBox(std::vector<std::string>& faceTextures);
		void RenderInternalBox(std::vector<std::string>& faceTextures);
		void RenderCapsule(const MathLib::vector4& position, const MathLib::quaternion& orientation, float halfHeight, float radius);
};

}

#endif // SUBRENDERERBASICSHAPE_H_INCLUDED