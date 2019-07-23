#include "Triangle.h"

namespace GeometryLib
{

TriangleVertex::TriangleVertex()
{
}

TriangleVertex::TriangleVertex(const MathLib::vector4& position, 
							   const MathLib::vector4& texCoord)
{
	MathLib::vector4_copy(this->position, position);
	MathLib::vector4_copy(this->texCoord, texCoord);
}

TriangleVertex::TriangleVertex(const MathLib::vector4& position, 
							   const MathLib::vector4& texCoord, 
							   const MathLib::vector4& normal) : 
	TriangleVertex(position, texCoord)
{
	MathLib::vector4_copy(this->normal, normal);
}

TriangleVertex::TriangleVertex(const TriangleVertex& vertex)
{
	MathLib::vector4_copy(position, vertex.position);
	MathLib::vector4_copy(texCoord, vertex.texCoord);
	MathLib::vector4_copy(normal, vertex.normal);
}

TriangleVertex& TriangleVertex::operator=(const TriangleVertex& vertex)
{
	MathLib::vector4_copy(position, vertex.position);
	MathLib::vector4_copy(texCoord, vertex.texCoord);
	MathLib::vector4_copy(normal, vertex.normal);

	return *this;
}

Triangle::Triangle(const TriangleVertex& point0,
				   const TriangleVertex& point1,
				   const TriangleVertex& point2)
{
	points[0] = point0;
	points[1] = point1; 
	points[2] = point2;
}

void Triangle::Subdivide(std::vector<Triangle>& outputVector)
{
	MathLib::vector4 p0p1Position;
	MathLib::vector4 p0p1TexCoord;
	MathLib::vector4_sub(points[1].position, points[0].position, p0p1Position);
	MathLib::vector4_sub(points[1].texCoord, points[0].texCoord, p0p1TexCoord);

	MathLib::vector4 p1p2Position;
	MathLib::vector4 p1p2TexCoord;
	MathLib::vector4_sub(points[2].position, points[1].position, p1p2Position);
	MathLib::vector4_sub(points[2].texCoord, points[1].texCoord, p1p2TexCoord);

	MathLib::vector4 p2p0Position;
	MathLib::vector4 p2p0TexCoord;
	MathLib::vector4_sub(points[0].position, points[2].position, p2p0Position);
	MathLib::vector4_sub(points[0].texCoord, points[2].texCoord, p2p0TexCoord);

	// Calculate midpoints.
	MathLib::vector4 midpoint01Position;
	MathLib::vector4 midpoint01TexCoord;
	MathLib::vector4_addScaledVector(points[0].position, p0p1Position, 0.5f, midpoint01Position);
	MathLib::vector4_addScaledVector(points[0].texCoord, p0p1TexCoord, 0.5f, midpoint01TexCoord);

	MathLib::vector4 midpoint12Position;
	MathLib::vector4 midpoint12TexCoord;
	MathLib::vector4_addScaledVector(points[1].position, p1p2Position, 0.5f, midpoint12Position);
	MathLib::vector4_addScaledVector(points[1].texCoord, p1p2TexCoord, 0.5f, midpoint12TexCoord);

	MathLib::vector4 midpoint20Position;
	MathLib::vector4 midpoint20TexCoord;
	MathLib::vector4_addScaledVector(points[2].position, p2p0Position, 0.5f, midpoint20Position);
	MathLib::vector4_addScaledVector(points[2].texCoord, p2p0TexCoord, 0.5f, midpoint20TexCoord);

	// Generate new triangles.
	Triangle triangle0(TriangleVertex(points[0].position, points[0].texCoord),
					   TriangleVertex(midpoint01Position, midpoint01TexCoord),
					   TriangleVertex(midpoint20Position, midpoint20TexCoord));
	outputVector.push_back(triangle0);

	Triangle triangle1(TriangleVertex(midpoint01Position, midpoint01TexCoord),
					   TriangleVertex(points[1].position, points[1].texCoord),
					   TriangleVertex(midpoint12Position, midpoint12TexCoord));
	outputVector.push_back(triangle1);

	Triangle triangle2(TriangleVertex(points[2].position, points[2].texCoord),
					   TriangleVertex(midpoint20Position, midpoint20TexCoord),
					   TriangleVertex(midpoint12Position, midpoint12TexCoord));
	outputVector.push_back(triangle2);

	Triangle triangle3(TriangleVertex(midpoint01Position, midpoint01TexCoord),
					   TriangleVertex(midpoint12Position, midpoint12TexCoord),
					   TriangleVertex(midpoint20Position, midpoint20TexCoord));
	outputVector.push_back(triangle3);
}

}