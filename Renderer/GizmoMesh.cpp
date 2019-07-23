#include "GizmoMesh.h"
#include "RendererUtils.h"
#include <MathLib.h>

namespace RendererLib
{

const int GizmoMesh::GIZMO_TYPE_CUBE	= 0;
const int GizmoMesh::GIZMO_TYPE_SPHERE	= 1;
const int GizmoMesh::GIZMO_TYPE_ARROW	= 2;

GizmoMesh::GizmoMesh()
{

}

GizmoMesh::~GizmoMesh()
{
	RendererUtils::DeleteVBO(&vboHandle);
	RendererUtils::DeleteVBO(&indicesHandle);
}

}