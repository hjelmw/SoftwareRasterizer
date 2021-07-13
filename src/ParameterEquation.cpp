#include "ParameterEquation.hpp"

ParameterEquation::ParameterEquation(
	float p0,
	float p1,
	float p2,
	const EdgeEquation& e0,
	const EdgeEquation& e1,
	const EdgeEquation& e2,
	float factor)
{
	//float factor = 1.0f / (2.0f * area);

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