
#include "Vertex.hpp"

Vertex::Vertex()
{
	this->x = 0;
	this->y = 0;
	this->z = 0;
	this->w = 0;

	this->r = 0; 
	this->g = 0; 
	this->b = 0;

	this->var[8] = { };
}

Vertex::Vertex(float x, float y, float z, float w, float r, float g, float b)
{
	this->x = x;
	this->y = y;
	this->z = z;
	this->w = w;

	this->r = r;
	this->g = g;
	this->b = b;

	// Filled out by vertex shader
	this->var[8] = { };
}

//Vertex Vertex::operator+(const Vertex& rhs)
//{
//	return Vertex(x + rhs.x, y + rhs.y, z + rhs.z);
//}

//Vertex Vertex::operator-(const Vertex& rhs)
//{
//	return Vertex(x - rhs.x, y - rhs.y, z - rhs.z);
//}

//Vertex Vertex::operator*(const Vertex& rhs)
//{
//	return Vertex(x * rhs.x, y * rhs.y, z * rhs.z);
//}

//Vertex Vertex::operator/(const double& rhs)
//{
//	return Vertex(x / rhs, y / rhs, z / rhs);
//}

//Vertex Vertex::operator*(const double& rhs)
//{
//	return Vertex(x * rhs, y * rhs, z * rhs);
//}

//bool Vertex::operator==(const Vertex& rhs) const {
//	return (x == rhs.x && y == rhs.y && z == rhs.z);
//}

//float Vertex::dotProduct(const Vertex& rhs)
//{
//	return x * rhs.x + y * rhs.y + z * rhs.z;
//}