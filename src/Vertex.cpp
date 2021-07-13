
#include "Vertex.hpp"

	Vertex::Vertex(float x, float y, float z, float w, float r, float g, float b)
	{
		this->x = x;
		this->y = y;
		this->z = z;
		this->w = w;

		this->var[0] = r;
		this->var[1] = g;
		this->var[2] = b;
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