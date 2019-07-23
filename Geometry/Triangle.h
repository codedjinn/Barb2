#ifndef TRIANGLE_H_INCLUDED
#define TRIANGLE_H_INCLUDED

#include "GeometryLibCommon.h"
#include <MathLib.h>
#include <vector>

namespace GeometryLib
{

class TriangleVertex
{
	public:

		MathLib::vector4 position;
		MathLib::vector4 texCoord;
		MathLib::vector4 normal;

		TriangleVertex();

		TriangleVertex(const MathLib::vector4& position,
					   const MathLib::vector4& texCoord);

		TriangleVertex(const MathLib::vector4& position,
					   const MathLib::vector4& texCoord,
					   const MathLib::vector4& normal);

		TriangleVertex(const TriangleVertex& vertex);

		TriangleVertex& operator=(const TriangleVertex& vertex);
};

class Triangle
{
	public:

		TriangleVertex points[3];

		Triangle(const TriangleVertex& point0,
				 const TriangleVertex& point1,
				 const TriangleVertex& point2);

		void Subdivide(std::vector<Triangle>& outputVector);
		/// Subdivides this triangle using midpoint subdivision and outputs the resulting four
		/// triangles in the output vector.
};

}

#endif // TRIANGLE_H_INCLUDED