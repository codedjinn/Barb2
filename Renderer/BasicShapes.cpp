#include "BasicShapes.h"
#include "RendererUtils.h"
#include <Triangle.h>
#include <vector>

namespace RendererLib
{

BasicShapes* BasicShapes::instance = NULL;
BasicShapes& BasicShapes::GetInstance()
{
	if (NULL == BasicShapes::instance)
	{
		BasicShapes::instance = new BasicShapes();
	}

	return *BasicShapes::instance;
}

const int BasicShapes::BASIC_SHAPE_EXTERNAL_SPHERE	= 1;
const int BasicShapes::BASIC_SHAPE_INTERNAL_SPHERE	= 2;
const int BasicShapes::BASIC_SHAPE_EXTERNAL_BOX		= 3;
const int BasicShapes::BASIC_SHAPE_INTERNAL_BOX		= 4;
const int BasicShapes::BASIC_SHAPE_CAPSULE			= 5;
	
std::vector<GeometryLib::Triangle> triangleBuffer0;
std::vector<GeometryLib::Triangle> triangleBuffer1;

void SubdivideTriangles(std::vector<GeometryLib::Triangle>& in, std::vector<GeometryLib::Triangle>& out)
{
	out.clear();
	for (unsigned int i = 0; i < in.size(); i++)
	{
		in[i].Subdivide(out);
	}
}

void CreateExternalSphere(BasicShapes& basicShapes, unsigned int tesselationFactor)
{
	// Create Front Face.
	{
		GeometryLib::Triangle frontTL(GeometryLib::TriangleVertex(MathLib::vector4(-1.0f, 1.0f, 1.0f, 1.0f), MathLib::vector4(0.0f, 1.0f, 0.0f, 0.0f)),
									  GeometryLib::TriangleVertex(MathLib::vector4(1.0f, 1.0f, 1.0f, 1.0f), MathLib::vector4(1.0f, 1.0f, 0.0f, 0.0f)),
									  GeometryLib::TriangleVertex(MathLib::vector4(-1.0f, -1.0f, 1.0f, 1.0f), MathLib::vector4(0.0f, 0.0f, 0.0f, 0.0f)));

		GeometryLib::Triangle frontBR(GeometryLib::TriangleVertex(MathLib::vector4(1.0f, 1.0f, 1.0f, 1.0f), MathLib::vector4(1.0f, 1.0f, 0.0f, 0.0f)),
									  GeometryLib::TriangleVertex(MathLib::vector4(1.0f, -1.0f, 1.0f, 1.0f), MathLib::vector4(1.0f, 0.0f, 0.0f, 0.0f)),
									  GeometryLib::TriangleVertex(MathLib::vector4(-1.0f, -1.0f, 1.0f, 1.0f), MathLib::vector4(0.0f, 0.0f, 0.0f, 0.0f)));

		triangleBuffer0.clear();
		triangleBuffer0.push_back(frontTL);
		triangleBuffer0.push_back(frontBR);

		std::vector<GeometryLib::Triangle>& inVector = triangleBuffer0;
		std::vector<GeometryLib::Triangle>& outVector = triangleBuffer1;

		for (unsigned int i = 0; i < tesselationFactor; i++)
		{
			SubdivideTriangles(inVector, outVector);

			// Swap in and out vectors.
			std::vector<GeometryLib::Triangle>& temp = inVector;
			inVector = outVector;
			outVector = temp;
		}
		
		// inVector contains the final triangles.
		// Need to normalize to sphere coordinates, and generate normals.
		for (unsigned int i = 0; i < inVector.size(); i++)
		{
			for (unsigned int j = 0; j < 3; j++)
			{
				MathLib::vector4_normalize(inVector[i].points[j].position);
				MathLib::vector4_copy(inVector[i].points[j].normal, inVector[i].points[j].position);
			}
		}

		// Do this for the first one, grab the attribute array sizes (for rendering later on).
		const auto numVertices = inVector.size() * 3;
		basicShapes.externalSphereFaceNumTriangles = (unsigned int)inVector.size();
		basicShapes.externalSpherePositionArraySizeBytes = (unsigned int)(numVertices * 3 * sizeof(float));
		basicShapes.externalSphereNormalArraySizeBytes = (unsigned int)(numVertices * 3 * sizeof(float));
		basicShapes.externalSphereTexCoordArraySizeBytes = (unsigned int)(numVertices * 2 * sizeof(float));

		RendererUtils::GenerateVBO(&basicShapes.externalSphereFrontFace, inVector);
	}

	// Create Right Face.
	{
		GeometryLib::Triangle rightTL(GeometryLib::TriangleVertex(MathLib::vector4(1.0f, 1.0f, 1.0f, 1.0f), MathLib::vector4(0.0f, 1.0f, 0.0f, 0.0f)),
									  GeometryLib::TriangleVertex(MathLib::vector4(1.0f, 1.0f, -1.0f, 1.0f), MathLib::vector4(1.0f, 1.0f, 0.0f, 0.0f)),
									  GeometryLib::TriangleVertex(MathLib::vector4(1.0f, -1.0f, 1.0f, 1.0f), MathLib::vector4(0.0f, 0.0f, 0.0f, 0.0f)));

		GeometryLib::Triangle rightBR(GeometryLib::TriangleVertex(MathLib::vector4(1.0f, 1.0f, -1.0f, 1.0f), MathLib::vector4(1.0f, 1.0f, 0.0f, 0.0f)),
									  GeometryLib::TriangleVertex(MathLib::vector4(1.0f, -1.0f, -1.0f, 1.0f), MathLib::vector4(1.0f, 0.0f, 0.0f, 0.0f)),
									  GeometryLib::TriangleVertex(MathLib::vector4(1.0f, -1.0f, 1.0f, 1.0f), MathLib::vector4(0.0f, 0.0f, 0.0f, 0.0f)));

		triangleBuffer0.clear();
		triangleBuffer0.push_back(rightTL);
		triangleBuffer0.push_back(rightBR);

		std::vector<GeometryLib::Triangle>& inVector = triangleBuffer0;
		std::vector<GeometryLib::Triangle>& outVector = triangleBuffer1;

		for (unsigned int i = 0; i < tesselationFactor; i++)
		{
			SubdivideTriangles(inVector, outVector);

			// Swap in and out vectors.
			std::vector<GeometryLib::Triangle>& temp = inVector;
			inVector = outVector;
			outVector = temp;
		}

		// inVector contains the final triangles.
		// Need to normalize to sphere coordinates, and generate normals.
		for (unsigned int i = 0; i < inVector.size(); i++)
		{
			for (unsigned int j = 0; j < 3; j++)
			{
				MathLib::vector4_normalize(inVector[i].points[j].position);
				MathLib::vector4_copy(inVector[i].points[j].normal, inVector[i].points[j].position);
			}
		}

		RendererUtils::GenerateVBO(&basicShapes.externalSphereRightFace, inVector);
	}

	// Create Back Face.
	{
		GeometryLib::Triangle backTL(GeometryLib::TriangleVertex(MathLib::vector4(1.0f, 1.0f, -1.0f, 1.0f), MathLib::vector4(0.0f, 1.0f, 0.0f, 0.0f)),
									 GeometryLib::TriangleVertex(MathLib::vector4(-1.0f, 1.0f, -1.0f, 1.0f), MathLib::vector4(1.0f, 1.0f, 0.0f, 0.0f)),
									 GeometryLib::TriangleVertex(MathLib::vector4(1.0f, -1.0f, -1.0f, 1.0f), MathLib::vector4(0.0f, 0.0f, 0.0f, 0.0f)));

		GeometryLib::Triangle backBR(GeometryLib::TriangleVertex(MathLib::vector4(-1.0f, 1.0f, -1.0f, 1.0f), MathLib::vector4(1.0f, 1.0f, 0.0f, 0.0f)),
									 GeometryLib::TriangleVertex(MathLib::vector4(-1.0f, -1.0f, -1.0f, 1.0f), MathLib::vector4(1.0f, 0.0f, 0.0f, 0.0f)),
									 GeometryLib::TriangleVertex(MathLib::vector4(1.0f, -1.0f, -1.0f, 1.0f), MathLib::vector4(0.0f, 0.0f, 0.0f, 0.0f)));

		triangleBuffer0.clear();
		triangleBuffer0.push_back(backTL);
		triangleBuffer0.push_back(backBR);

		std::vector<GeometryLib::Triangle>& inVector = triangleBuffer0;
		std::vector<GeometryLib::Triangle>& outVector = triangleBuffer1;

		for (unsigned int i = 0; i < tesselationFactor; i++)
		{
			SubdivideTriangles(inVector, outVector);

			// Swap in and out vectors.
			std::vector<GeometryLib::Triangle>& temp = inVector;
			inVector = outVector;
			outVector = temp;
		}

		// inVector contains the final triangles.
		// Need to normalize to sphere coordinates, and generate normals.
		for (unsigned int i = 0; i < inVector.size(); i++)
		{
			for (unsigned int j = 0; j < 3; j++)
			{
				MathLib::vector4_normalize(inVector[i].points[j].position);
				MathLib::vector4_copy(inVector[i].points[j].normal, inVector[i].points[j].position);
			}
		}

		RendererUtils::GenerateVBO(&basicShapes.externalSphereBackFace, inVector);
	}

	// Create Left Face.
	{
		GeometryLib::Triangle leftTL(GeometryLib::TriangleVertex(MathLib::vector4(-1.0f, 1.0f, -1.0f, 1.0f), MathLib::vector4(0.0f, 1.0f, 0.0f, 0.0f)),
									 GeometryLib::TriangleVertex(MathLib::vector4(-1.0f, 1.0f, 1.0f, 1.0f), MathLib::vector4(1.0f, 1.0f, 0.0f, 0.0f)),
									 GeometryLib::TriangleVertex(MathLib::vector4(-1.0f, -1.0f, -1.0f, 1.0f), MathLib::vector4(0.0f, 0.0f, 0.0f, 0.0f)));

		GeometryLib::Triangle leftBR(GeometryLib::TriangleVertex(MathLib::vector4(-1.0f, 1.0f, 1.0f, 1.0f), MathLib::vector4(1.0f, 1.0f, 0.0f, 0.0f)),
									 GeometryLib::TriangleVertex(MathLib::vector4(-1.0f, -1.0f, 1.0f, 1.0f), MathLib::vector4(1.0f, 0.0f, 0.0f, 0.0f)),
									 GeometryLib::TriangleVertex(MathLib::vector4(-1.0f, -1.0f, -1.0f, 1.0f), MathLib::vector4(0.0f, 0.0f, 0.0f, 0.0f)));

		triangleBuffer0.clear();
		triangleBuffer0.push_back(leftTL);
		triangleBuffer0.push_back(leftBR);

		std::vector<GeometryLib::Triangle>& inVector = triangleBuffer0;
		std::vector<GeometryLib::Triangle>& outVector = triangleBuffer1;

		for (unsigned int i = 0; i < tesselationFactor; i++)
		{
			SubdivideTriangles(inVector, outVector);

			// Swap in and out vectors.
			std::vector<GeometryLib::Triangle>& temp = inVector;
			inVector = outVector;
			outVector = temp;
		}

		// inVector contains the final triangles.
		// Need to normalize to sphere coordinates, and generate normals.
		for (unsigned int i = 0; i < inVector.size(); i++)
		{
			for (unsigned int j = 0; j < 3; j++)
			{
				MathLib::vector4_normalize(inVector[i].points[j].position);
				MathLib::vector4_copy(inVector[i].points[j].normal, inVector[i].points[j].position);
			}
		}

		RendererUtils::GenerateVBO(&basicShapes.externalSphereLeftFace, inVector);
	}

	// Create Top Face.
	{
		GeometryLib::Triangle topTL(GeometryLib::TriangleVertex(MathLib::vector4(-1.0f, 1.0f, -1.0f, 1.0f), MathLib::vector4(0.0f, 1.0f, 0.0f, 0.0f)),
									GeometryLib::TriangleVertex(MathLib::vector4(1.0f, 1.0f, -1.0f, 1.0f), MathLib::vector4(1.0f, 1.0f, 0.0f, 0.0f)),
									GeometryLib::TriangleVertex(MathLib::vector4(-1.0f, 1.0f, 1.0f, 1.0f), MathLib::vector4(0.0f, 0.0f, 0.0f, 0.0f)));

		GeometryLib::Triangle topBR(GeometryLib::TriangleVertex(MathLib::vector4(1.0f, 1.0f, -1.0f, 1.0f), MathLib::vector4(1.0f, 1.0f, 0.0f, 0.0f)),
									GeometryLib::TriangleVertex(MathLib::vector4(1.0f, 1.0f, 1.0f, 1.0f), MathLib::vector4(1.0f, 0.0f, 0.0f, 0.0f)),
									GeometryLib::TriangleVertex(MathLib::vector4(-1.0f, 1.0f, 1.0f, 1.0f), MathLib::vector4(0.0f, 0.0f, 0.0f, 0.0f)));

		triangleBuffer0.clear();
		triangleBuffer0.push_back(topTL);
		triangleBuffer0.push_back(topBR);

		std::vector<GeometryLib::Triangle>& inVector = triangleBuffer0;
		std::vector<GeometryLib::Triangle>& outVector = triangleBuffer1;

		for (unsigned int i = 0; i < tesselationFactor; i++)
		{
			SubdivideTriangles(inVector, outVector);

			// Swap in and out vectors.
			std::vector<GeometryLib::Triangle>& temp = inVector;
			inVector = outVector;
			outVector = temp;
		}

		// inVector contains the final triangles.
		// Need to normalize to sphere coordinates, and generate normals.
		for (unsigned int i = 0; i < inVector.size(); i++)
		{
			for (unsigned int j = 0; j < 3; j++)
			{
				MathLib::vector4_normalize(inVector[i].points[j].position);
				MathLib::vector4_copy(inVector[i].points[j].normal, inVector[i].points[j].position);
			}
		}

		RendererUtils::GenerateVBO(&basicShapes.externalSphereTopFace, inVector);
	}

	// Create Bottom Face.
	{
		GeometryLib::Triangle bottomTL(GeometryLib::TriangleVertex(MathLib::vector4(-1.0f, -1.0f, 1.0f, 1.0f), MathLib::vector4(0.0f, 1.0f, 0.0f, 0.0f)),
									   GeometryLib::TriangleVertex(MathLib::vector4(1.0f, -1.0f, 1.0f, 1.0f), MathLib::vector4(1.0f, 1.0f, 0.0f, 0.0f)),
									   GeometryLib::TriangleVertex(MathLib::vector4(-1.0f, -1.0f, -1.0f, 1.0f), MathLib::vector4(0.0f, 0.0f, 0.0f, 0.0f)));

		GeometryLib::Triangle bottomBR(GeometryLib::TriangleVertex(MathLib::vector4(1.0f, -1.0f, 1.0f, 1.0f), MathLib::vector4(1.0f, 1.0f, 0.0f, 0.0f)),
									   GeometryLib::TriangleVertex(MathLib::vector4(1.0f, -1.0f, -1.0f, 1.0f), MathLib::vector4(1.0f, 0.0f, 0.0f, 0.0f)),
									   GeometryLib::TriangleVertex(MathLib::vector4(-1.0f, -1.0f, -1.0f, 1.0f), MathLib::vector4(0.0f, 0.0f, 0.0f, 0.0f)));

		triangleBuffer0.clear();
		triangleBuffer0.push_back(bottomTL);
		triangleBuffer0.push_back(bottomBR);

		std::vector<GeometryLib::Triangle>& inVector = triangleBuffer0;
		std::vector<GeometryLib::Triangle>& outVector = triangleBuffer1;

		for (unsigned int i = 0; i < tesselationFactor; i++)
		{
			SubdivideTriangles(inVector, outVector);

			// Swap in and out vectors.
			std::vector<GeometryLib::Triangle>& temp = inVector;
			inVector = outVector;
			outVector = temp;
		}

		// inVector contains the final triangles.
		// Need to normalize to sphere coordinates, and generate normals.
		for (unsigned int i = 0; i < inVector.size(); i++)
		{
			for (unsigned int j = 0; j < 3; j++)
			{
				MathLib::vector4_normalize(inVector[i].points[j].position);
				MathLib::vector4_copy(inVector[i].points[j].normal, inVector[i].points[j].position);
			}
		}

		RendererUtils::GenerateVBO(&basicShapes.externalSphereBottomFace, inVector);
	}
}

void CreateInternalSphere(BasicShapes& basicShapes, unsigned int tesselationFactor)
{
	// Create Front Face.
	{
		GeometryLib::Triangle frontTL(GeometryLib::TriangleVertex(MathLib::vector4(1.0f, 1.0f, 1.0f, 1.0f), MathLib::vector4(0.0f, 1.0f, 0.0f, 0.0f)),
									  GeometryLib::TriangleVertex(MathLib::vector4(-1.0f, 1.0f, 1.0f, 1.0f), MathLib::vector4(1.0f, 1.0f, 0.0f, 0.0f)),
									  GeometryLib::TriangleVertex(MathLib::vector4(1.0f, -1.0f, 1.0f, 1.0f), MathLib::vector4(0.0f, 0.0f, 0.0f, 0.0f)));

		GeometryLib::Triangle frontBR(GeometryLib::TriangleVertex(MathLib::vector4(-1.0f, 1.0f, 1.0f, 1.0f), MathLib::vector4(1.0f, 1.0f, 0.0f, 0.0f)),
									  GeometryLib::TriangleVertex(MathLib::vector4(-1.0f, -1.0f, 1.0f, 1.0f), MathLib::vector4(1.0f, 0.0f, 0.0f, 0.0f)),
									  GeometryLib::TriangleVertex(MathLib::vector4(1.0f, -1.0f, 1.0f, 1.0f), MathLib::vector4(0.0f, 0.0f, 0.0f, 0.0f)));

		triangleBuffer0.clear();
		triangleBuffer0.push_back(frontTL);
		triangleBuffer0.push_back(frontBR);

		std::vector<GeometryLib::Triangle>& inVector = triangleBuffer0;
		std::vector<GeometryLib::Triangle>& outVector = triangleBuffer1;

		for (unsigned int i = 0; i < tesselationFactor; i++)
		{
			SubdivideTriangles(inVector, outVector);

			// Swap in and out vectors.
			std::vector<GeometryLib::Triangle>& temp = inVector;
			inVector = outVector;
			outVector = temp;
		}

		// inVector contains the final triangles.
		// Need to normalize to sphere coordinates, and generate normals.
		for (unsigned int i = 0; i < inVector.size(); i++)
		{
			for (unsigned int j = 0; j < 3; j++)
			{
				MathLib::vector4_normalize(inVector[i].points[j].position);
				MathLib::vector4_scale(inVector[i].points[j].position, -1.0f, inVector[i].points[j].normal);
			}
		}

		// Do this for the first one, grab the attribute array sizes (for rendering later on).
		const auto numVertices = inVector.size() * 3;
		basicShapes.internalSphereFaceNumTriangles = (unsigned int)inVector.size();
		basicShapes.internalSpherePositionArraySizeBytes = (unsigned int)(numVertices * 3 * sizeof(float));
		basicShapes.internalSphereTexCoordArraySizeBytes = (unsigned int)(numVertices * 2 * sizeof(float));
		basicShapes.internalSphereNormalArraySizeBytes = (unsigned int)(numVertices * 3 * sizeof(float));

		RendererUtils::GenerateVBO(&basicShapes.internalSphereFrontFace, inVector);
	}

	// Create Right Face.
	{
		GeometryLib::Triangle rightTL(GeometryLib::TriangleVertex(MathLib::vector4(1.0f, 1.0f, -1.0f, 1.0f), MathLib::vector4(0.0f, 1.0f, 0.0f, 0.0f)),
									  GeometryLib::TriangleVertex(MathLib::vector4(1.0f, 1.0f, 1.0f, 1.0f), MathLib::vector4(1.0f, 1.0f, 0.0f, 0.0f)),
									  GeometryLib::TriangleVertex(MathLib::vector4(1.0f, -1.0f, -1.0f, 1.0f), MathLib::vector4(0.0f, 0.0f, 0.0f, 0.0f)));

		GeometryLib::Triangle rightBR(GeometryLib::TriangleVertex(MathLib::vector4(1.0f, 1.0f, 1.0f, 1.0f), MathLib::vector4(1.0f, 1.0f, 0.0f, 0.0f)),
									  GeometryLib::TriangleVertex(MathLib::vector4(1.0f, -1.0f, 1.0f, 1.0f), MathLib::vector4(1.0f, 0.0f, 0.0f, 0.0f)),
									  GeometryLib::TriangleVertex(MathLib::vector4(1.0f, -1.0f, -1.0f, 1.0f), MathLib::vector4(0.0f, 0.0f, 0.0f, 0.0f)));

		triangleBuffer0.clear();
		triangleBuffer0.push_back(rightTL);
		triangleBuffer0.push_back(rightBR);

		std::vector<GeometryLib::Triangle>& inVector = triangleBuffer0;
		std::vector<GeometryLib::Triangle>& outVector = triangleBuffer1;

		for (unsigned int i = 0; i < tesselationFactor; i++)
		{
			SubdivideTriangles(inVector, outVector);

			// Swap in and out vectors.
			std::vector<GeometryLib::Triangle>& temp = inVector;
			inVector = outVector;
			outVector = temp;
		}

		// inVector contains the final triangles.
		// Need to normalize to sphere coordinates, and generate normals.
		for (unsigned int i = 0; i < inVector.size(); i++)
		{
			for (unsigned int j = 0; j < 3; j++)
			{
				MathLib::vector4_normalize(inVector[i].points[j].position);
				MathLib::vector4_scale(inVector[i].points[j].position, -1.0f, inVector[i].points[j].normal);
			}
		}

		RendererUtils::GenerateVBO(&basicShapes.internalSphereRightFace, inVector);
	}

	// Create Back Face.
	{
		GeometryLib::Triangle backTL(GeometryLib::TriangleVertex(MathLib::vector4(-1.0f, 1.0f, -1.0f, 1.0f), MathLib::vector4(0.0f, 1.0f, 0.0f, 0.0f)),
									 GeometryLib::TriangleVertex(MathLib::vector4(1.0f, 1.0f, -1.0f, 1.0f), MathLib::vector4(1.0f, 1.0f, 0.0f, 0.0f)),
									 GeometryLib::TriangleVertex(MathLib::vector4(-1.0f, -1.0f, -1.0f, 1.0f), MathLib::vector4(0.0f, 0.0f, 0.0f, 0.0f)));

		GeometryLib::Triangle backBR(GeometryLib::TriangleVertex(MathLib::vector4(1.0f, 1.0f, -1.0f, 1.0f), MathLib::vector4(1.0f, 1.0f, 0.0f, 0.0f)),
									 GeometryLib::TriangleVertex(MathLib::vector4(1.0f, -1.0f, -1.0f, 1.0f), MathLib::vector4(1.0f, 0.0f, 0.0f, 0.0f)),
									 GeometryLib::TriangleVertex(MathLib::vector4(-1.0f, -1.0f, -1.0f, 1.0f), MathLib::vector4(0.0f, 0.0f, 0.0f, 0.0f)));

		triangleBuffer0.clear();
		triangleBuffer0.push_back(backTL);
		triangleBuffer0.push_back(backBR);

		std::vector<GeometryLib::Triangle>& inVector = triangleBuffer0;
		std::vector<GeometryLib::Triangle>& outVector = triangleBuffer1;

		for (unsigned int i = 0; i < tesselationFactor; i++)
		{
			SubdivideTriangles(inVector, outVector);

			// Swap in and out vectors.
			std::vector<GeometryLib::Triangle>& temp = inVector;
			inVector = outVector;
			outVector = temp;
		}

		// inVector contains the final triangles.
		// Need to normalize to sphere coordinates, and generate normals.
		for (unsigned int i = 0; i < inVector.size(); i++)
		{
			for (unsigned int j = 0; j < 3; j++)
			{
				MathLib::vector4_normalize(inVector[i].points[j].position);
				MathLib::vector4_scale(inVector[i].points[j].position, -1.0f, inVector[i].points[j].normal);
			}
		}

		RendererUtils::GenerateVBO(&basicShapes.internalSphereBackFace, inVector);
	}

	// Create Left Face.
	{
		GeometryLib::Triangle leftTL(GeometryLib::TriangleVertex(MathLib::vector4(-1.0f, 1.0f, 1.0f, 1.0f), MathLib::vector4(0.0f, 1.0f, 0.0f, 0.0f)),
									 GeometryLib::TriangleVertex(MathLib::vector4(-1.0f, 1.0f, -1.0f, 1.0f), MathLib::vector4(1.0f, 1.0f, 0.0f, 0.0f)),
									 GeometryLib::TriangleVertex(MathLib::vector4(-1.0f, -1.0f, 1.0f, 1.0f), MathLib::vector4(0.0f, 0.0f, 0.0f, 0.0f)));

		GeometryLib::Triangle leftBR(GeometryLib::TriangleVertex(MathLib::vector4(-1.0f, 1.0f, -1.0f, 1.0f), MathLib::vector4(1.0f, 1.0f, 0.0f, 0.0f)),
									 GeometryLib::TriangleVertex(MathLib::vector4(-1.0f, -1.0f, -1.0f, 1.0f), MathLib::vector4(1.0f, 0.0f, 0.0f, 0.0f)),
									 GeometryLib::TriangleVertex(MathLib::vector4(-1.0f, -1.0f, 1.0f, 1.0f), MathLib::vector4(0.0f, 0.0f, 0.0f, 0.0f)));

		triangleBuffer0.clear();
		triangleBuffer0.push_back(leftTL);
		triangleBuffer0.push_back(leftBR);

		std::vector<GeometryLib::Triangle>& inVector = triangleBuffer0;
		std::vector<GeometryLib::Triangle>& outVector = triangleBuffer1;

		for (unsigned int i = 0; i < tesselationFactor; i++)
		{
			SubdivideTriangles(inVector, outVector);

			// Swap in and out vectors.
			std::vector<GeometryLib::Triangle>& temp = inVector;
			inVector = outVector;
			outVector = temp;
		}

		// inVector contains the final triangles.
		// Need to normalize to sphere coordinates, and generate normals.
		for (unsigned int i = 0; i < inVector.size(); i++)
		{
			for (unsigned int j = 0; j < 3; j++)
			{
				MathLib::vector4_normalize(inVector[i].points[j].position);
				MathLib::vector4_scale(inVector[i].points[j].position, -1.0f, inVector[i].points[j].normal);
			}
		}

		RendererUtils::GenerateVBO(&basicShapes.internalSphereLeftFace, inVector);
	}

	// Create Top Face.
	{
		GeometryLib::Triangle topTL(GeometryLib::TriangleVertex(MathLib::vector4(1.0f, 1.0f, -1.0f, 1.0f), MathLib::vector4(0.0f, 1.0f, 0.0f, 0.0f)),
									GeometryLib::TriangleVertex(MathLib::vector4(-1.0f, 1.0f, -1.0f, 1.0f), MathLib::vector4(1.0f, 1.0f, 0.0f, 0.0f)),
									GeometryLib::TriangleVertex(MathLib::vector4(1.0f, 1.0f, 1.0f, 1.0f), MathLib::vector4(0.0f, 0.0f, 0.0f, 0.0f)));

		GeometryLib::Triangle topBR(GeometryLib::TriangleVertex(MathLib::vector4(-1.0f, 1.0f, -1.0f, 1.0f), MathLib::vector4(1.0f, 1.0f, 0.0f, 0.0f)),
									GeometryLib::TriangleVertex(MathLib::vector4(-1.0f, 1.0f, 1.0f, 1.0f), MathLib::vector4(1.0f, 0.0f, 0.0f, 0.0f)),
									GeometryLib::TriangleVertex(MathLib::vector4(1.0f, 1.0f, 1.0f, 1.0f), MathLib::vector4(0.0f, 0.0f, 0.0f, 0.0f)));

		triangleBuffer0.clear();
		triangleBuffer0.push_back(topTL);
		triangleBuffer0.push_back(topBR);

		std::vector<GeometryLib::Triangle>& inVector = triangleBuffer0;
		std::vector<GeometryLib::Triangle>& outVector = triangleBuffer1;

		for (unsigned int i = 0; i < tesselationFactor; i++)
		{
			SubdivideTriangles(inVector, outVector);

			// Swap in and out vectors.
			std::vector<GeometryLib::Triangle>& temp = inVector;
			inVector = outVector;
			outVector = temp;
		}

		// inVector contains the final triangles.
		// Need to normalize to sphere coordinates, and generate normals.
		for (unsigned int i = 0; i < inVector.size(); i++)
		{
			for (unsigned int j = 0; j < 3; j++)
			{
				MathLib::vector4_normalize(inVector[i].points[j].position);
				MathLib::vector4_scale(inVector[i].points[j].position, -1.0f, inVector[i].points[j].normal);
			}
		}

		RendererUtils::GenerateVBO(&basicShapes.internalSphereTopFace, inVector);
	}

	// Create Bottom Face.
	{
		GeometryLib::Triangle bottomTL(GeometryLib::TriangleVertex(MathLib::vector4(1.0f, -1.0f, 1.0f, 1.0f), MathLib::vector4(0.0f, 1.0f, 0.0f, 0.0f)),
									   GeometryLib::TriangleVertex(MathLib::vector4(-1.0f, -1.0f, 1.0f, 1.0f), MathLib::vector4(1.0f, 1.0f, 0.0f, 0.0f)),
									   GeometryLib::TriangleVertex(MathLib::vector4(1.0f, -1.0f, -1.0f, 1.0f), MathLib::vector4(0.0f, 0.0f, 0.0f, 0.0f)));

		GeometryLib::Triangle bottomBR(GeometryLib::TriangleVertex(MathLib::vector4(-1.0f, -1.0f, 1.0f, 1.0f), MathLib::vector4(1.0f, 1.0f, 0.0f, 0.0f)),
									   GeometryLib::TriangleVertex(MathLib::vector4(-1.0f, -1.0f, -1.0f, 1.0f), MathLib::vector4(1.0f, 0.0f, 0.0f, 0.0f)),
									   GeometryLib::TriangleVertex(MathLib::vector4(1.0f, -1.0f, -1.0f, 1.0f), MathLib::vector4(0.0f, 0.0f, 0.0f, 0.0f)));

		triangleBuffer0.clear();
		triangleBuffer0.push_back(bottomTL);
		triangleBuffer0.push_back(bottomBR);

		std::vector<GeometryLib::Triangle>& inVector = triangleBuffer0;
		std::vector<GeometryLib::Triangle>& outVector = triangleBuffer1;

		for (unsigned int i = 0; i < tesselationFactor; i++)
		{
			SubdivideTriangles(inVector, outVector);

			// Swap in and out vectors.
			std::vector<GeometryLib::Triangle>& temp = inVector;
			inVector = outVector;
			outVector = temp;
		}

		// inVector contains the final triangles.
		// Need to normalize to sphere coordinates, and generate normals.
		for (int i = 0; i < inVector.size(); i++)
		{
			for (int j = 0; j < 3; j++)
			{
				MathLib::vector4_normalize(inVector[i].points[j].position);
				MathLib::vector4_scale(inVector[i].points[j].position, -1.0f, inVector[i].points[j].normal);
			}
		}

		RendererUtils::GenerateVBO(&basicShapes.internalSphereBottomFace, inVector);
	}
}

void CreateExternalBox(BasicShapes& basicShapes)
{
	// Do this for the first one, grab the attribute array sizes (for rendering later on).
	const unsigned int numVertices = 2 * 3;
	basicShapes.externalBoxFaceNumTriangles = 2;
	basicShapes.externalBoxPositionArraySizeBytes = numVertices * 3 * sizeof(float);
	basicShapes.externalBoxNormalArraySizeBytes = numVertices * 3 * sizeof(float);
	basicShapes.externalBoxTexCoordArraySizeBytes = numVertices * 2 * sizeof(float);

	// Create Front Face.
	{
		GeometryLib::Triangle frontTL(GeometryLib::TriangleVertex(MathLib::vector4(-1.0f, 1.0f, 1.0f, 1.0f), MathLib::vector4(0.0f, 1.0f, 0.0f, 0.0f)),
									  GeometryLib::TriangleVertex(MathLib::vector4(1.0f, 1.0f, 1.0f, 1.0f), MathLib::vector4(1.0f, 1.0f, 0.0f, 0.0f)),
									  GeometryLib::TriangleVertex(MathLib::vector4(-1.0f, -1.0f, 1.0f, 1.0f), MathLib::vector4(0.0f, 0.0f, 0.0f, 0.0f)));

		GeometryLib::Triangle frontBR(GeometryLib::TriangleVertex(MathLib::vector4(1.0f, 1.0f, 1.0f, 1.0f), MathLib::vector4(1.0f, 1.0f, 0.0f, 0.0f)),
									  GeometryLib::TriangleVertex(MathLib::vector4(1.0f, -1.0f, 1.0f, 1.0f), MathLib::vector4(1.0f, 0.0f, 0.0f, 0.0f)),
									  GeometryLib::TriangleVertex(MathLib::vector4(-1.0f, -1.0f, 1.0f, 1.0f), MathLib::vector4(0.0f, 0.0f, 0.0f, 0.0f)));

		triangleBuffer0.clear();
		triangleBuffer0.push_back(frontTL);
		triangleBuffer0.push_back(frontBR);

		// Need to normalize to sphere coordinates, and generate normals.
		for (int i = 0; i < triangleBuffer0.size(); i++)
		{
			for (int j = 0; j < 3; j++)
			{
				MathLib::vector4_copy(triangleBuffer0[i].points[j].normal, MathLib::vector4(0.0f, 0.0f, 1.0f, 0.0f));
			}
		}

		RendererUtils::GenerateVBO(&basicShapes.externalBoxFrontFace, triangleBuffer0);
	}

	// Create Right Face.
	{
		GeometryLib::Triangle rightTL(GeometryLib::TriangleVertex(MathLib::vector4(1.0f, 1.0f, 1.0f, 1.0f), MathLib::vector4(0.0f, 1.0f, 0.0f, 0.0f)),
									  GeometryLib::TriangleVertex(MathLib::vector4(1.0f, 1.0f, -1.0f, 1.0f), MathLib::vector4(1.0f, 1.0f, 0.0f, 0.0f)),
									  GeometryLib::TriangleVertex(MathLib::vector4(1.0f, -1.0f, 1.0f, 1.0f), MathLib::vector4(0.0f, 0.0f, 0.0f, 0.0f)));

		GeometryLib::Triangle rightBR(GeometryLib::TriangleVertex(MathLib::vector4(1.0f, 1.0f, -1.0f, 1.0f), MathLib::vector4(1.0f, 1.0f, 0.0f, 0.0f)),
									  GeometryLib::TriangleVertex(MathLib::vector4(1.0f, -1.0f, -1.0f, 1.0f), MathLib::vector4(1.0f, 0.0f, 0.0f, 0.0f)),
									  GeometryLib::TriangleVertex(MathLib::vector4(1.0f, -1.0f, 1.0f, 1.0f), MathLib::vector4(0.0f, 0.0f, 0.0f, 0.0f)));

		triangleBuffer0.clear();
		triangleBuffer0.push_back(rightTL);
		triangleBuffer0.push_back(rightBR);

		// Need to normalize to sphere coordinates, and generate normals.
		for (int i = 0; i < triangleBuffer0.size(); i++)
		{
			for (int j = 0; j < 3; j++)
			{
				MathLib::vector4_copy(triangleBuffer0[i].points[j].normal, MathLib::vector4(1.0f, 0.0f, 0.0f, 0.0f));
			}
		}

		RendererUtils::GenerateVBO(&basicShapes.externalBoxRightFace, triangleBuffer0);
	}

	// Create Back Face.
	{
		GeometryLib::Triangle backTL(GeometryLib::TriangleVertex(MathLib::vector4(1.0f, 1.0f, -1.0f, 1.0f), MathLib::vector4(0.0f, 1.0f, 0.0f, 0.0f)),
									 GeometryLib::TriangleVertex(MathLib::vector4(-1.0f, 1.0f, -1.0f, 1.0f), MathLib::vector4(1.0f, 1.0f, 0.0f, 0.0f)),
									 GeometryLib::TriangleVertex(MathLib::vector4(1.0f, -1.0f, -1.0f, 1.0f), MathLib::vector4(0.0f, 0.0f, 0.0f, 0.0f)));

		GeometryLib::Triangle backBR(GeometryLib::TriangleVertex(MathLib::vector4(-1.0f, 1.0f, -1.0f, 1.0f), MathLib::vector4(1.0f, 1.0f, 0.0f, 0.0f)),
									 GeometryLib::TriangleVertex(MathLib::vector4(-1.0f, -1.0f, -1.0f, 1.0f), MathLib::vector4(1.0f, 0.0f, 0.0f, 0.0f)),
									 GeometryLib::TriangleVertex(MathLib::vector4(1.0f, -1.0f, -1.0f, 1.0f), MathLib::vector4(0.0f, 0.0f, 0.0f, 0.0f)));

		triangleBuffer0.clear();
		triangleBuffer0.push_back(backTL);
		triangleBuffer0.push_back(backBR);

		// inVector contains the final triangles.
		// Need to normalize to sphere coordinates, and generate normals.
		for (int i = 0; i < triangleBuffer0.size(); i++)
		{
			for (int j = 0; j < 3; j++)
			{
				MathLib::vector4_copy(triangleBuffer0[i].points[j].normal, MathLib::vector4(0.0f, 0.0f, -1.0f, 0.0f));
			}
		}

		RendererUtils::GenerateVBO(&basicShapes.externalBoxBackFace, triangleBuffer0);
	}

	// Create Left Face.
	{
		GeometryLib::Triangle leftTL(GeometryLib::TriangleVertex(MathLib::vector4(-1.0f, 1.0f, -1.0f, 1.0f), MathLib::vector4(0.0f, 1.0f, 0.0f, 0.0f)),
									 GeometryLib::TriangleVertex(MathLib::vector4(-1.0f, 1.0f, 1.0f, 1.0f), MathLib::vector4(1.0f, 1.0f, 0.0f, 0.0f)),
									 GeometryLib::TriangleVertex(MathLib::vector4(-1.0f, -1.0f, -1.0f, 1.0f), MathLib::vector4(0.0f, 0.0f, 0.0f, 0.0f)));

		GeometryLib::Triangle leftBR(GeometryLib::TriangleVertex(MathLib::vector4(-1.0f, 1.0f, 1.0f, 1.0f), MathLib::vector4(1.0f, 1.0f, 0.0f, 0.0f)),
									 GeometryLib::TriangleVertex(MathLib::vector4(-1.0f, -1.0f, 1.0f, 1.0f), MathLib::vector4(1.0f, 0.0f, 0.0f, 0.0f)),
									 GeometryLib::TriangleVertex(MathLib::vector4(-1.0f, -1.0f, -1.0f, 1.0f), MathLib::vector4(0.0f, 0.0f, 0.0f, 0.0f)));

		triangleBuffer0.clear();
		triangleBuffer0.push_back(leftTL);
		triangleBuffer0.push_back(leftBR);

		// inVector contains the final triangles.
		// Need to normalize to sphere coordinates, and generate normals.
		for (int i = 0; i < triangleBuffer0.size(); i++)
		{
			for (int j = 0; j < 3; j++)
			{
				MathLib::vector4_copy(triangleBuffer0[i].points[j].normal, MathLib::vector4(-1.0f, 0.0f, 0.0f, 0.0f));
			}
		}

		RendererUtils::GenerateVBO(&basicShapes.externalBoxLeftFace, triangleBuffer0);
	}

	// Create Top Face.
	{
		GeometryLib::Triangle topTL(GeometryLib::TriangleVertex(MathLib::vector4(-1.0f, 1.0f, -1.0f, 1.0f), MathLib::vector4(0.0f, 1.0f, 0.0f, 0.0f)),
									GeometryLib::TriangleVertex(MathLib::vector4(1.0f, 1.0f, -1.0f, 1.0f), MathLib::vector4(1.0f, 1.0f, 0.0f, 0.0f)),
									GeometryLib::TriangleVertex(MathLib::vector4(-1.0f, 1.0f, 1.0f, 1.0f), MathLib::vector4(0.0f, 0.0f, 0.0f, 0.0f)));

		GeometryLib::Triangle topBR(GeometryLib::TriangleVertex(MathLib::vector4(1.0f, 1.0f, -1.0f, 1.0f), MathLib::vector4(1.0f, 1.0f, 0.0f, 0.0f)),
									GeometryLib::TriangleVertex(MathLib::vector4(1.0f, 1.0f, 1.0f, 1.0f), MathLib::vector4(1.0f, 0.0f, 0.0f, 0.0f)),
									GeometryLib::TriangleVertex(MathLib::vector4(-1.0f, 1.0f, 1.0f, 1.0f), MathLib::vector4(0.0f, 0.0f, 0.0f, 0.0f)));

		triangleBuffer0.clear();
		triangleBuffer0.push_back(topTL);
		triangleBuffer0.push_back(topBR);

		// inVector contains the final triangles.
		// Need to normalize to sphere coordinates, and generate normals.
		for (int i = 0; i < triangleBuffer0.size(); i++)
		{
			for (int j = 0; j < 3; j++)
			{
				MathLib::vector4_copy(triangleBuffer0[i].points[j].normal, MathLib::vector4(0.0f, 1.0f, 0.0f, 0.0f));
			}
		}

		RendererUtils::GenerateVBO(&basicShapes.externalBoxTopFace, triangleBuffer0);
	}

	// Create Bottom Face.
	{
		GeometryLib::Triangle bottomTL(GeometryLib::TriangleVertex(MathLib::vector4(-1.0f, -1.0f, 1.0f, 1.0f), MathLib::vector4(0.0f, 1.0f, 0.0f, 0.0f)),
									   GeometryLib::TriangleVertex(MathLib::vector4(1.0f, -1.0f, 1.0f, 1.0f), MathLib::vector4(1.0f, 1.0f, 0.0f, 0.0f)),
									   GeometryLib::TriangleVertex(MathLib::vector4(-1.0f, -1.0f, -1.0f, 1.0f), MathLib::vector4(0.0f, 0.0f, 0.0f, 0.0f)));

		GeometryLib::Triangle bottomBR(GeometryLib::TriangleVertex(MathLib::vector4(1.0f, -1.0f, 1.0f, 1.0f), MathLib::vector4(1.0f, 1.0f, 0.0f, 0.0f)),
									   GeometryLib::TriangleVertex(MathLib::vector4(1.0f, -1.0f, -1.0f, 1.0f), MathLib::vector4(1.0f, 0.0f, 0.0f, 0.0f)),
									   GeometryLib::TriangleVertex(MathLib::vector4(-1.0f, -1.0f, -1.0f, 1.0f), MathLib::vector4(0.0f, 0.0f, 0.0f, 0.0f)));

		triangleBuffer0.clear();
		triangleBuffer0.push_back(bottomTL);
		triangleBuffer0.push_back(bottomBR);

		// inVector contains the final triangles.
		// Need to normalize to sphere coordinates, and generate normals.
		for (int i = 0; i < triangleBuffer0.size(); i++)
		{
			for (int j = 0; j < 3; j++)
			{
				MathLib::vector4_copy(triangleBuffer0[i].points[j].normal, MathLib::vector4(0.0f, -1.0f, 0.0f, 0.0f));
			}
		}

		RendererUtils::GenerateVBO(&basicShapes.externalBoxBottomFace, triangleBuffer0);
	}
}

void CreateInternalBox(BasicShapes& basicShapes)
{
	const unsigned int numVertices = 2 * 3;
	basicShapes.internalBoxFaceNumTriangles = 2;
	basicShapes.internalBoxPositionArraySizeBytes = numVertices * 3 * sizeof(float);
	basicShapes.internalBoxTexCoordArraySizeBytes = numVertices * 2 * sizeof(float);
	basicShapes.internalBoxNormalArraySizeBytes = numVertices * 3 * sizeof(float);

	// Create Front Face.
	{
		GeometryLib::Triangle frontTL(GeometryLib::TriangleVertex(MathLib::vector4(1.0f, 1.0f, 1.0f, 1.0f), MathLib::vector4(0.0f, 1.0f, 0.0f, 0.0f)),
									  GeometryLib::TriangleVertex(MathLib::vector4(-1.0f, 1.0f, 1.0f, 1.0f), MathLib::vector4(1.0f, 1.0f, 0.0f, 0.0f)),
									  GeometryLib::TriangleVertex(MathLib::vector4(1.0f, -1.0f, 1.0f, 1.0f), MathLib::vector4(0.0f, 0.0f, 0.0f, 0.0f)));

		GeometryLib::Triangle frontBR(GeometryLib::TriangleVertex(MathLib::vector4(-1.0f, 1.0f, 1.0f, 1.0f), MathLib::vector4(1.0f, 1.0f, 0.0f, 0.0f)),
									  GeometryLib::TriangleVertex(MathLib::vector4(-1.0f, -1.0f, 1.0f, 1.0f), MathLib::vector4(1.0f, 0.0f, 0.0f, 0.0f)),
									  GeometryLib::TriangleVertex(MathLib::vector4(1.0f, -1.0f, 1.0f, 1.0f), MathLib::vector4(0.0f, 0.0f, 0.0f, 0.0f)));

		triangleBuffer0.clear();
		triangleBuffer0.push_back(frontTL);
		triangleBuffer0.push_back(frontBR);

		for (int i = 0; i < triangleBuffer0.size(); i++)
		{
			for (int j = 0; j < 3; j++)
			{
				MathLib::vector4_copy(triangleBuffer0[i].points[j].normal, MathLib::vector4(0.0f, 0.0f, -1.0f, 0.0f));
			}
		}

		RendererUtils::GenerateVBO(&basicShapes.internalBoxFrontFace, triangleBuffer0);
	}

	// Create Right Face.
	{
		GeometryLib::Triangle rightTL(GeometryLib::TriangleVertex(MathLib::vector4(1.0f, 1.0f, -1.0f, 1.0f), MathLib::vector4(0.0f, 1.0f, 0.0f, 0.0f)),
									  GeometryLib::TriangleVertex(MathLib::vector4(1.0f, 1.0f, 1.0f, 1.0f), MathLib::vector4(1.0f, 1.0f, 0.0f, 0.0f)),
									  GeometryLib::TriangleVertex(MathLib::vector4(1.0f, -1.0f, -1.0f, 1.0f), MathLib::vector4(0.0f, 0.0f, 0.0f, 0.0f)));

		GeometryLib::Triangle rightBR(GeometryLib::TriangleVertex(MathLib::vector4(1.0f, 1.0f, 1.0f, 1.0f), MathLib::vector4(1.0f, 1.0f, 0.0f, 0.0f)),
									  GeometryLib::TriangleVertex(MathLib::vector4(1.0f, -1.0f, 1.0f, 1.0f), MathLib::vector4(1.0f, 0.0f, 0.0f, 0.0f)),
									  GeometryLib::TriangleVertex(MathLib::vector4(1.0f, -1.0f, -1.0f, 1.0f), MathLib::vector4(0.0f, 0.0f, 0.0f, 0.0f)));

		triangleBuffer0.clear();
		triangleBuffer0.push_back(rightTL);
		triangleBuffer0.push_back(rightBR);

		for (int i = 0; i < triangleBuffer0.size(); i++)
		{
			for (int j = 0; j < 3; j++)
			{
				MathLib::vector4_copy(triangleBuffer0[i].points[j].normal, MathLib::vector4(-1.0f, 0.0f, 0.0f, 0.0f));
			}
		}

		RendererUtils::GenerateVBO(&basicShapes.internalBoxRightFace, triangleBuffer0);
	}

	// Create Back Face.
	{
		GeometryLib::Triangle backTL(GeometryLib::TriangleVertex(MathLib::vector4(-1.0f, 1.0f, -1.0f, 1.0f), MathLib::vector4(0.0f, 1.0f, 0.0f, 0.0f)),
									 GeometryLib::TriangleVertex(MathLib::vector4(1.0f, 1.0f, -1.0f, 1.0f), MathLib::vector4(1.0f, 1.0f, 0.0f, 0.0f)),
									 GeometryLib::TriangleVertex(MathLib::vector4(-1.0f, -1.0f, -1.0f, 1.0f), MathLib::vector4(0.0f, 0.0f, 0.0f, 0.0f)));

		GeometryLib::Triangle backBR(GeometryLib::TriangleVertex(MathLib::vector4(1.0f, 1.0f, -1.0f, 1.0f), MathLib::vector4(1.0f, 1.0f, 0.0f, 0.0f)),
									 GeometryLib::TriangleVertex(MathLib::vector4(1.0f, -1.0f, -1.0f, 1.0f), MathLib::vector4(1.0f, 0.0f, 0.0f, 0.0f)),
									 GeometryLib::TriangleVertex(MathLib::vector4(-1.0f, -1.0f, -1.0f, 1.0f), MathLib::vector4(0.0f, 0.0f, 0.0f, 0.0f)));

		triangleBuffer0.clear();
		triangleBuffer0.push_back(backTL);
		triangleBuffer0.push_back(backBR);

		for (int i = 0; i < triangleBuffer0.size(); i++)
		{
			for (int j = 0; j < 3; j++)
			{
				MathLib::vector4_copy(triangleBuffer0[i].points[j].normal, MathLib::vector4(0.0f, 0.0f, 1.0f, 0.0f));
			}
		}

		RendererUtils::GenerateVBO(&basicShapes.internalBoxBackFace, triangleBuffer0);
	}

	// Create Left Face.
	{
		GeometryLib::Triangle leftTL(GeometryLib::TriangleVertex(MathLib::vector4(-1.0f, 1.0f, 1.0f, 1.0f), MathLib::vector4(0.0f, 1.0f, 0.0f, 0.0f)),
									 GeometryLib::TriangleVertex(MathLib::vector4(-1.0f, 1.0f, -1.0f, 1.0f), MathLib::vector4(1.0f, 1.0f, 0.0f, 0.0f)),
									 GeometryLib::TriangleVertex(MathLib::vector4(-1.0f, -1.0f, 1.0f, 1.0f), MathLib::vector4(0.0f, 0.0f, 0.0f, 0.0f)));

		GeometryLib::Triangle leftBR(GeometryLib::TriangleVertex(MathLib::vector4(-1.0f, 1.0f, -1.0f, 1.0f), MathLib::vector4(1.0f, 1.0f, 0.0f, 0.0f)),
									 GeometryLib::TriangleVertex(MathLib::vector4(-1.0f, -1.0f, -1.0f, 1.0f), MathLib::vector4(1.0f, 0.0f, 0.0f, 0.0f)),
									 GeometryLib::TriangleVertex(MathLib::vector4(-1.0f, -1.0f, 1.0f, 1.0f), MathLib::vector4(0.0f, 0.0f, 0.0f, 0.0f)));

		triangleBuffer0.clear();
		triangleBuffer0.push_back(leftTL);
		triangleBuffer0.push_back(leftBR);

		for (int i = 0; i < triangleBuffer0.size(); i++)
		{
			for (int j = 0; j < 3; j++)
			{
				MathLib::vector4_copy(triangleBuffer0[i].points[j].normal, MathLib::vector4(1.0f, 0.0f, 0.0f, 0.0f));
			}
		}

		RendererUtils::GenerateVBO(&basicShapes.internalBoxLeftFace, triangleBuffer0);
	}

	// Create Top Face.
	{
		GeometryLib::Triangle topTL(GeometryLib::TriangleVertex(MathLib::vector4(1.0f, 1.0f, -1.0f, 1.0f), MathLib::vector4(0.0f, 1.0f, 0.0f, 0.0f)),
									GeometryLib::TriangleVertex(MathLib::vector4(-1.0f, 1.0f, -1.0f, 1.0f), MathLib::vector4(1.0f, 1.0f, 0.0f, 0.0f)),
									GeometryLib::TriangleVertex(MathLib::vector4(1.0f, 1.0f, 1.0f, 1.0f), MathLib::vector4(0.0f, 0.0f, 0.0f, 0.0f)));

		GeometryLib::Triangle topBR(GeometryLib::TriangleVertex(MathLib::vector4(-1.0f, 1.0f, -1.0f, 1.0f), MathLib::vector4(1.0f, 1.0f, 0.0f, 0.0f)),
									GeometryLib::TriangleVertex(MathLib::vector4(-1.0f, 1.0f, 1.0f, 1.0f), MathLib::vector4(1.0f, 0.0f, 0.0f, 0.0f)),
									GeometryLib::TriangleVertex(MathLib::vector4(1.0f, 1.0f, 1.0f, 1.0f), MathLib::vector4(0.0f, 0.0f, 0.0f, 0.0f)));

		triangleBuffer0.clear();
		triangleBuffer0.push_back(topTL);
		triangleBuffer0.push_back(topBR);

		for (int i = 0; i < triangleBuffer0.size(); i++)
		{
			for (int j = 0; j < 3; j++)
			{
				MathLib::vector4_copy(triangleBuffer0[i].points[j].normal, MathLib::vector4(0.0f, -1.0f, 0.0f, 0.0f));
			}
		}

		RendererUtils::GenerateVBO(&basicShapes.internalBoxTopFace, triangleBuffer0);
	}

	// Create Bottom Face.
	{
		GeometryLib::Triangle bottomTL(GeometryLib::TriangleVertex(MathLib::vector4(1.0f, -1.0f, 1.0f, 1.0f), MathLib::vector4(0.0f, 1.0f, 0.0f, 0.0f)),
									   GeometryLib::TriangleVertex(MathLib::vector4(-1.0f, -1.0f, 1.0f, 1.0f), MathLib::vector4(1.0f, 1.0f, 0.0f, 0.0f)),
									   GeometryLib::TriangleVertex(MathLib::vector4(1.0f, -1.0f, -1.0f, 1.0f), MathLib::vector4(0.0f, 0.0f, 0.0f, 0.0f)));

		GeometryLib::Triangle bottomBR(GeometryLib::TriangleVertex(MathLib::vector4(-1.0f, -1.0f, 1.0f, 1.0f), MathLib::vector4(1.0f, 1.0f, 0.0f, 0.0f)),
									   GeometryLib::TriangleVertex(MathLib::vector4(-1.0f, -1.0f, -1.0f, 1.0f), MathLib::vector4(1.0f, 0.0f, 0.0f, 0.0f)),
									   GeometryLib::TriangleVertex(MathLib::vector4(1.0f, -1.0f, -1.0f, 1.0f), MathLib::vector4(0.0f, 0.0f, 0.0f, 0.0f)));

		triangleBuffer0.clear();
		triangleBuffer0.push_back(bottomTL);
		triangleBuffer0.push_back(bottomBR);

		for (int i = 0; i < triangleBuffer0.size(); i++)
		{
			for (int j = 0; j < 3; j++)
			{
				MathLib::vector4_copy(triangleBuffer0[i].points[j].normal, MathLib::vector4(0.0f, 1.0f, 0.0f, 0.0f));
			}
		}

		RendererUtils::GenerateVBO(&basicShapes.internalBoxBottomFace, triangleBuffer0);
	}
}

void CreateCapsule(BasicShapes& basicShapes, unsigned int tesselationFactor)
{
	// Create Front Face.
	{
		GeometryLib::Triangle frontTL(GeometryLib::TriangleVertex(MathLib::vector4(-1.0f, 1.0f, 1.0f, 1.0f), MathLib::vector4(0.0f, 1.0f, 0.0f, 0.0f)),
									  GeometryLib::TriangleVertex(MathLib::vector4(1.0f, 1.0f, 1.0f, 1.0f), MathLib::vector4(1.0f, 1.0f, 0.0f, 0.0f)),
									  GeometryLib::TriangleVertex(MathLib::vector4(-1.0f, -1.0f, 1.0f, 1.0f), MathLib::vector4(0.0f, 0.0f, 0.0f, 0.0f)));

		GeometryLib::Triangle frontBR(GeometryLib::TriangleVertex(MathLib::vector4(1.0f, 1.0f, 1.0f, 1.0f), MathLib::vector4(1.0f, 1.0f, 0.0f, 0.0f)),
									  GeometryLib::TriangleVertex(MathLib::vector4(1.0f, -1.0f, 1.0f, 1.0f), MathLib::vector4(1.0f, 0.0f, 0.0f, 0.0f)),
									  GeometryLib::TriangleVertex(MathLib::vector4(-1.0f, -1.0f, 1.0f, 1.0f), MathLib::vector4(0.0f, 0.0f, 0.0f, 0.0f)));

		triangleBuffer0.clear();
		triangleBuffer0.push_back(frontTL);
		triangleBuffer0.push_back(frontBR);

		std::vector<GeometryLib::Triangle>& inVector = triangleBuffer0;
		std::vector<GeometryLib::Triangle>& outVector = triangleBuffer1;

		for (unsigned int i = 0; i < tesselationFactor; i++)
		{
			SubdivideTriangles(inVector, outVector);

			// Swap in and out vectors.
			std::vector<GeometryLib::Triangle>& temp = inVector;
			inVector = outVector;
			outVector = temp;
		}

		// inVector contains the final triangles.
		// Normalize around the x axis.
		for (unsigned int i = 0; i < inVector.size(); i++)
		{
			for (unsigned int j = 0; j < 3; j++)
			{
				MathLib::vector4 point = inVector[i].points[j].position;
				point.setX(0.0f);
				MathLib::vector4_normalize(point);

				inVector[i].points[j].position.setXYZ(inVector[i].points[j].position.extractX(),
													  point.extractY(),
													  point.extractZ());

				MathLib::vector4_copy(inVector[i].points[j].normal, point);
			}
		}

		// Do this for the first one, grab the attribute array sizes (for rendering later on).
		const auto numVertices = inVector.size() * 3;
		basicShapes.capsuleFaceNumTriangles = (unsigned int)inVector.size();
		basicShapes.capsulePositionArraySizeBytes = (unsigned int)(numVertices * 3 * sizeof(float));
		basicShapes.capsuleNormalArraySizeBytes = (unsigned int)(numVertices * 3 * sizeof(float));
		basicShapes.capsuleTexCoordArraySizeBytes = (unsigned int)(numVertices * 2 * sizeof(float));

		RendererUtils::GenerateVBO(&basicShapes.capsuleFrontFace, inVector);
	}

	// Create Back Face.
	{
		GeometryLib::Triangle backTL(GeometryLib::TriangleVertex(MathLib::vector4(1.0f, 1.0f, -1.0f, 1.0f), MathLib::vector4(0.0f, 1.0f, 0.0f, 0.0f)),
									 GeometryLib::TriangleVertex(MathLib::vector4(-1.0f, 1.0f, -1.0f, 1.0f), MathLib::vector4(1.0f, 1.0f, 0.0f, 0.0f)),
									 GeometryLib::TriangleVertex(MathLib::vector4(1.0f, -1.0f, -1.0f, 1.0f), MathLib::vector4(0.0f, 0.0f, 0.0f, 0.0f)));

		GeometryLib::Triangle backBR(GeometryLib::TriangleVertex(MathLib::vector4(-1.0f, 1.0f, -1.0f, 1.0f), MathLib::vector4(1.0f, 1.0f, 0.0f, 0.0f)),
									 GeometryLib::TriangleVertex(MathLib::vector4(-1.0f, -1.0f, -1.0f, 1.0f), MathLib::vector4(1.0f, 0.0f, 0.0f, 0.0f)),
									 GeometryLib::TriangleVertex(MathLib::vector4(1.0f, -1.0f, -1.0f, 1.0f), MathLib::vector4(0.0f, 0.0f, 0.0f, 0.0f)));

		triangleBuffer0.clear();
		triangleBuffer0.push_back(backTL);
		triangleBuffer0.push_back(backBR);

		std::vector<GeometryLib::Triangle>& inVector = triangleBuffer0;
		std::vector<GeometryLib::Triangle>& outVector = triangleBuffer1;

		for (unsigned int i = 0; i < tesselationFactor; i++)
		{
			SubdivideTriangles(inVector, outVector);

			// Swap in and out vectors.
			std::vector<GeometryLib::Triangle>& temp = inVector;
			inVector = outVector;
			outVector = temp;
		}

		// inVector contains the final triangles.
		// Normalize around the x axis.
		for (int i = 0; i < inVector.size(); i++)
		{
			for (int j = 0; j < 3; j++)
			{
				MathLib::vector4 point = inVector[i].points[j].position;
				point.setX(0.0f);
				MathLib::vector4_normalize(point);

				inVector[i].points[j].position.setXYZ(inVector[i].points[j].position.extractX(),
													  point.extractY(),
													  point.extractZ());

				MathLib::vector4_copy(inVector[i].points[j].normal, point);
			}
		}

		RendererUtils::GenerateVBO(&basicShapes.capsuleBackFace, inVector);
	}

	// Create Top Face.
	{
		GeometryLib::Triangle topTL(GeometryLib::TriangleVertex(MathLib::vector4(-1.0f, 1.0f, -1.0f, 1.0f), MathLib::vector4(0.0f, 1.0f, 0.0f, 0.0f)),
									GeometryLib::TriangleVertex(MathLib::vector4(1.0f, 1.0f, -1.0f, 1.0f), MathLib::vector4(1.0f, 1.0f, 0.0f, 0.0f)),
									GeometryLib::TriangleVertex(MathLib::vector4(-1.0f, 1.0f, 1.0f, 1.0f), MathLib::vector4(0.0f, 0.0f, 0.0f, 0.0f)));

		GeometryLib::Triangle topBR(GeometryLib::TriangleVertex(MathLib::vector4(1.0f, 1.0f, -1.0f, 1.0f), MathLib::vector4(1.0f, 1.0f, 0.0f, 0.0f)),
									GeometryLib::TriangleVertex(MathLib::vector4(1.0f, 1.0f, 1.0f, 1.0f), MathLib::vector4(1.0f, 0.0f, 0.0f, 0.0f)),
									GeometryLib::TriangleVertex(MathLib::vector4(-1.0f, 1.0f, 1.0f, 1.0f), MathLib::vector4(0.0f, 0.0f, 0.0f, 0.0f)));

		triangleBuffer0.clear();
		triangleBuffer0.push_back(topTL);
		triangleBuffer0.push_back(topBR);

		std::vector<GeometryLib::Triangle>& inVector = triangleBuffer0;
		std::vector<GeometryLib::Triangle>& outVector = triangleBuffer1;

		for (unsigned int i = 0; i < tesselationFactor; i++)
		{
			SubdivideTriangles(inVector, outVector);

			// Swap in and out vectors.
			std::vector<GeometryLib::Triangle>& temp = inVector;
			inVector = outVector;
			outVector = temp;
		}

		// inVector contains the final triangles.
		// Normalize around the x axis.
		for (unsigned int i = 0; i < inVector.size(); i++)
		{
			for (unsigned int j = 0; j < 3; j++)
			{
				MathLib::vector4 point = inVector[i].points[j].position;
				point.setX(0.0f);
				MathLib::vector4_normalize(point);

				inVector[i].points[j].position.setXYZ(inVector[i].points[j].position.extractX(),
													  point.extractY(),
													  point.extractZ());

				MathLib::vector4_copy(inVector[i].points[j].normal, point);
			}
		}


		RendererUtils::GenerateVBO(&basicShapes.capsuleTopFace, inVector);
	}

	// Create Bottom Face.
	{
		GeometryLib::Triangle bottomTL(GeometryLib::TriangleVertex(MathLib::vector4(-1.0f, -1.0f, 1.0f, 1.0f), MathLib::vector4(0.0f, 1.0f, 0.0f, 0.0f)),
									   GeometryLib::TriangleVertex(MathLib::vector4(1.0f, -1.0f, 1.0f, 1.0f), MathLib::vector4(1.0f, 1.0f, 0.0f, 0.0f)),
									   GeometryLib::TriangleVertex(MathLib::vector4(-1.0f, -1.0f, -1.0f, 1.0f), MathLib::vector4(0.0f, 0.0f, 0.0f, 0.0f)));

		GeometryLib::Triangle bottomBR(GeometryLib::TriangleVertex(MathLib::vector4(1.0f, -1.0f, 1.0f, 1.0f), MathLib::vector4(1.0f, 1.0f, 0.0f, 0.0f)),
									   GeometryLib::TriangleVertex(MathLib::vector4(1.0f, -1.0f, -1.0f, 1.0f), MathLib::vector4(1.0f, 0.0f, 0.0f, 0.0f)),
									   GeometryLib::TriangleVertex(MathLib::vector4(-1.0f, -1.0f, -1.0f, 1.0f), MathLib::vector4(0.0f, 0.0f, 0.0f, 0.0f)));

		triangleBuffer0.clear();
		triangleBuffer0.push_back(bottomTL);
		triangleBuffer0.push_back(bottomBR);

		std::vector<GeometryLib::Triangle>& inVector = triangleBuffer0;
		std::vector<GeometryLib::Triangle>& outVector = triangleBuffer1;

		for (unsigned int i = 0; i < tesselationFactor; i++)
		{
			SubdivideTriangles(inVector, outVector);

			// Swap in and out vectors.
			std::vector<GeometryLib::Triangle>& temp = inVector;
			inVector = outVector;
			outVector = temp;
		}

		// inVector contains the final triangles.
		// Normalize around the x axis.
		for (int i = 0; i < inVector.size(); i++)
		{
			for (int j = 0; j < 3; j++)
			{
				MathLib::vector4 point = inVector[i].points[j].position;
				point.setX(0.0f);
				MathLib::vector4_normalize(point);

				inVector[i].points[j].position.setXYZ(inVector[i].points[j].position.extractX(),
													  point.extractY(),
													  point.extractZ());

				MathLib::vector4_copy(inVector[i].points[j].normal, point);
			}
		}

		RendererUtils::GenerateVBO(&basicShapes.capsuleBottomFace, inVector);
	}
}

BasicShapes::BasicShapes()
{
	CreateExternalSphere(*this, 3);
	CreateInternalSphere(*this, 3);
	CreateExternalBox(*this);
	CreateInternalBox(*this);
	CreateCapsule(*this, 3);
}

BasicShapes::~BasicShapes()
{
	RendererUtils::DeleteVBO(&externalSphereFrontFace);
	RendererUtils::DeleteVBO(&externalSphereRightFace);
	RendererUtils::DeleteVBO(&externalSphereBackFace);
	RendererUtils::DeleteVBO(&externalSphereLeftFace);
	RendererUtils::DeleteVBO(&externalSphereTopFace);
	RendererUtils::DeleteVBO(&externalSphereBottomFace);

	RendererUtils::DeleteVBO(&internalSphereFrontFace);
	RendererUtils::DeleteVBO(&internalSphereRightFace);
	RendererUtils::DeleteVBO(&internalSphereBackFace);
	RendererUtils::DeleteVBO(&internalSphereLeftFace);
	RendererUtils::DeleteVBO(&internalSphereTopFace);
	RendererUtils::DeleteVBO(&internalSphereBottomFace);

	RendererUtils::DeleteVBO(&externalBoxFrontFace);
	RendererUtils::DeleteVBO(&externalBoxRightFace);
	RendererUtils::DeleteVBO(&externalBoxBackFace);
	RendererUtils::DeleteVBO(&externalBoxLeftFace);
	RendererUtils::DeleteVBO(&externalBoxTopFace);
	RendererUtils::DeleteVBO(&externalBoxBottomFace);

	RendererUtils::DeleteVBO(&internalBoxFrontFace);
	RendererUtils::DeleteVBO(&internalBoxRightFace);
	RendererUtils::DeleteVBO(&internalBoxBackFace);
	RendererUtils::DeleteVBO(&internalBoxLeftFace);
	RendererUtils::DeleteVBO(&internalBoxTopFace);
	RendererUtils::DeleteVBO(&internalBoxBottomFace);

	RendererUtils::DeleteVBO(&capsuleFrontFace);
	RendererUtils::DeleteVBO(&capsuleTopFace);
	RendererUtils::DeleteVBO(&capsuleBackFace);
	RendererUtils::DeleteVBO(&capsuleBottomFace);
}

}