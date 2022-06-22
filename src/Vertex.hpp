#pragma once



//
//									Vertex
//		A handy Vertex class definition with support for per vertex variables
//



class Vertex
{
public:
	float x;
	float y;
	float z;
	float w;

	float r;
	float g;
	float b;

	/// Maximum per vertex variables. Can be interpolated across the triangle (See TriangleEquations file)
	float var[16];

	Vertex();
	Vertex(float x, float y, float z, float w, float r, float g, float b);

	bool operator==(Vertex const& rhs) const { return this->x == rhs.x && this->y == rhs.y && this->z == rhs.z; }
};