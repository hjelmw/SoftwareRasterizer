#pragma once


class Vertex
{
public:
	float x;
	float y;
	float z;
	float w;

	/// Maximum per vertex variables used for interpolation across the triangle.
	float var[16];

	Vertex();
	Vertex(float x, float y, float z, float w, float r, float g, float b);

	// Vertex operations
	//Vertex operator+(const Vertex& rhs);
	//Vertex operator-(const Vertex& rhs);
	//Vertex operator*(const Vertex& rhs);

	//Vertex operator/(const double& rhs);
	//Vertex operator*(const double& rhs);
	//bool operator==(const Vertex& rhs)const;
	//float dotProduct(const Vertex& rhs);
};