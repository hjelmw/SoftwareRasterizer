//////////////////////////////////////////////////////////////////
///							ParameterEquation
///
/// Interpolates color on point in triangle
///
//////////////////////////////////////////////////////////////////


#pragma once

#include "EdgeEquation.hpp"

class ParameterEquation
{
public:
	float a;
	float b;
	float c;

	ParameterEquation() = default;
	ParameterEquation(
		float p0,
		float p1,
		float p2,
		const EdgeEquation& e0,
		const EdgeEquation& e1,
		const EdgeEquation& e2,
		float area);

	/// Evaluate the parameter equation for the given point.
	float evaluate(float x, float y) const;

	/// Step the equation value v to the x direction.
	float stepX(float v) const;
	float stepX(float v, float stepSize) const;

	/// Step the equation value v to the y direction.
	float stepY(float v) const;
	float stepY(float v, float stepSize) const;
};