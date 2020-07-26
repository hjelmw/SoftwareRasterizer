#include "ParameterEquation.hpp"

ParameterEquation::ParameterEquation(
	float p0,
	float p1,
	float p2,
	const EdgeEquation& e0,
	const EdgeEquation& e1,
	const EdgeEquation& e2,
	float area)
{
	float factor = 1.0f / (2.0f * area);

	a = factor * (p0 * e0.a + p1 * e1.a + p2 * e2.a);
	b = factor * (p0 * e0.b + p1 * e1.b + p2 * e2.b);
	c = factor * (p0 * e0.c + p1 * e1.c + p2 * e2.c);
}

float ParameterEquation::evaluate(float x, float y) const
{
	return a * x + b * y + c;
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