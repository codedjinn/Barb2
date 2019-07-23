#ifndef GIZMOMESH_H_INCLUDED
#define GIZMOMESH_H_INCLUDED

#include "RendererLibCommon.h"

namespace RendererLib
{

/**
* GizmoMesh:
* Mesh class that contains the required information necessary to render a gizmo on the screen.
*/
class GizmoMesh
{
	public:

		virtual ~GizmoMesh();

		RENDERER_INLINE GLuint GetVBOHandle() const { return vboHandle; }
		RENDERER_INLINE GLuint GetIndicesHandle() const { return indicesHandle; }

		RENDERER_INLINE unsigned int GetNumVertices() const { return numVertices; }
		RENDERER_INLINE unsigned int GetNumIndices() const { return numIndices; }

		static const int GIZMO_TYPE_CUBE;
		static const int GIZMO_TYPE_SPHERE;
		static const int GIZMO_TYPE_ARROW;

	protected:

		GizmoMesh();

		GLuint vboHandle;
		GLuint indicesHandle;

		unsigned int numVertices;
		unsigned int numIndices;
};

}

#endif // GIZMO_MESH_H_INCLUDED