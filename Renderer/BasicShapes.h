#ifndef BASICSHAPES_H_INCLUDED
#define BASICSHAPES_H_INCLUDED

#include "RendererLibCommon.h"

namespace RendererLib
{

class Renderer;

class BasicShapes
{
	public:

		// External Sphere.
		GLuint externalSphereFrontFace;
		GLuint externalSphereRightFace;
		GLuint externalSphereBackFace;
		GLuint externalSphereLeftFace;
		GLuint externalSphereTopFace;
		GLuint externalSphereBottomFace;

		unsigned int externalSphereFaceNumTriangles;
		unsigned int externalSpherePositionArraySizeBytes;
		unsigned int externalSphereTexCoordArraySizeBytes;
		unsigned int externalSphereNormalArraySizeBytes;

		// Internal Sphere.
		GLuint internalSphereFrontFace;
		GLuint internalSphereRightFace;
		GLuint internalSphereBackFace;
		GLuint internalSphereLeftFace;
		GLuint internalSphereTopFace;
		GLuint internalSphereBottomFace;

		unsigned int internalSphereFaceNumTriangles;
		unsigned int internalSpherePositionArraySizeBytes;
		unsigned int internalSphereTexCoordArraySizeBytes;
		unsigned int internalSphereNormalArraySizeBytes;

		// External Box.
		GLuint externalBoxFrontFace;
		GLuint externalBoxRightFace;
		GLuint externalBoxBackFace;
		GLuint externalBoxLeftFace;
		GLuint externalBoxTopFace;
		GLuint externalBoxBottomFace;

		unsigned int externalBoxFaceNumTriangles;
		unsigned int externalBoxPositionArraySizeBytes;
		unsigned int externalBoxTexCoordArraySizeBytes;
		unsigned int externalBoxNormalArraySizeBytes;

		// Internal Box.
		GLuint internalBoxFrontFace;
		GLuint internalBoxRightFace;
		GLuint internalBoxBackFace;
		GLuint internalBoxLeftFace;
		GLuint internalBoxTopFace;
		GLuint internalBoxBottomFace;

		unsigned int internalBoxFaceNumTriangles;
		unsigned int internalBoxPositionArraySizeBytes;
		unsigned int internalBoxTexCoordArraySizeBytes;
		unsigned int internalBoxNormalArraySizeBytes;

		// Capsule.
		GLuint capsuleFrontFace;
		GLuint capsuleTopFace;
		GLuint capsuleBackFace;
		GLuint capsuleBottomFace;

		unsigned int capsuleFaceNumTriangles;
		unsigned int capsulePositionArraySizeBytes;
		unsigned int capsuleTexCoordArraySizeBytes;
		unsigned int capsuleNormalArraySizeBytes;

		static const int BASIC_SHAPE_EXTERNAL_SPHERE;
		static const int BASIC_SHAPE_INTERNAL_SPHERE;
		static const int BASIC_SHAPE_EXTERNAL_BOX;
		static const int BASIC_SHAPE_INTERNAL_BOX;
		static const int BASIC_SHAPE_CAPSULE;

		virtual ~BasicShapes();

		static BasicShapes& GetInstance();

	protected:

		static BasicShapes* instance;

		BasicShapes();

		friend class Renderer;
};

}

#endif // BASICSHAPES_H_INCLUDED