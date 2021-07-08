#include "EdgeEquation.hpp"
#include "EdgeData.hpp"


EdgeEquation::EdgeEquation(const Vertex& v0, const Vertex& v1)
{
	this->a = v0.y - v1.y;
	this->b = v1.x - v0.x;
	this->c = -(a * (v0.x + v1.x) + b * (v0.y + v1.y)) / 2;
	this->tie = a != 0 ? a > 0 : b > 0;
}

float EdgeEquation::evaluate(float x, float y) const
{
	return this->a * x + this->b * y + c;
}

bool EdgeEquation::test(float x, float y) const
{
	return test(evaluate(x, y));
}

float EdgeEquation::test(float v) const
{
	return (v > 0 || v == 0 && this->tie);
}


/// Step the equation value v to the x direction.

float EdgeEquation::stepX(float v) const
{
	return v + a;
}

/// Step the equation value v to the x direction.

float EdgeEquation::stepX(float v, float stepSize) const
{
	return v + a * stepSize;
}

/// Step the equation value v to the y direction.

float EdgeEquation::stepY(float v) const
{
	return v + b;
}

/// Step the equation value vto the y direction.

float EdgeEquation::stepY(float v, float stepSize) const
{
	return v + b * stepSize;
}