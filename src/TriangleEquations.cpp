#include "TriangleEquations.hpp"


EdgeEquation::EdgeEquation(const Vertex& v0, const Vertex& v1)
{
	this->a = v0.y - v1.y;
	this->b = v1.x - v0.x;

	//v0.x * v1.y - v0.y * v1.x
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

ParameterEquation::ParameterEquation(
	float p0,
	float p1,
	float p2,
	const EdgeEquation& e0,
	const EdgeEquation& e1,
	const EdgeEquation& e2,
	float factor)
{

	// Solve interpolation equation
	this->a = factor * (p0 * e0.a + p1 * e1.a + p2 * e2.a);
	this->b = factor * (p0 * e0.b + p1 * e1.b + p2 * e2.b);
	this->c = factor * (p0 * e0.c + p1 * e1.c + p2 * e2.c);
}

float ParameterEquation::evaluate(float x, float y) const
{
	return this->a * x + this->b * y + this->c;
}

/// Step the equation value v to the x direction.
float ParameterEquation::stepX(float v) const
{
	return v + a;
}

/// Step the equation value v to the x direction.
float ParameterEquation::stepX(float v, float stepSize) const
{
	return v + a * stepSize;
}

/// Step the equation value v to the y direction.
float ParameterEquation::stepY(float v) const
{
	return v + b;
}

/// Step the equation value vto the y direction.
float ParameterEquation::stepY(float v, float stepSize) const
{
	return v + b * stepSize;
}

TriangleEquations::TriangleEquations(const Vertex& v0, const Vertex& v1, const Vertex& v2, int varCount)
{
	this->e0 = EdgeEquation(v1, v2);
	this->e1 = EdgeEquation(v2, v0);
	this->e2 = EdgeEquation(v0, v1);

	this->area = (e0.c + e1.c + e2.c);

	if (this->area < 0.0f)
		return;

	float factor = 1.0f / area;

	this->z = ParameterEquation(v0.z, v1.z, v2.z, e0, e1, e2, factor);

	float invw0 = 1.0f / v0.w;
	float invw1 = 1.0f / v1.w;
	float invw2 = 1.0f / v2.w;

	this->invw = ParameterEquation(invw0, invw1, invw2, e0, e1, e2, factor);

	for (int i = 0; i < varCount; ++i)
		this->var[i] = ParameterEquation(v0.var[i], v1.var[i], v2.var[i], e0, e1, e2, factor);
}

