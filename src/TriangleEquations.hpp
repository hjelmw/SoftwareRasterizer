#pragma once

#include "Vertex.hpp"

/* TriangleEquations
*
* This class contains 3 classes that together encapsulates a mathematical representation of a triangle
* And functions to test points against them.
* It also implements interpolation of points inside them using barycentric coordinates for arbitrary vertex values
*
*/


class EdgeEquation
{
public:
	// An edge equation is a function that returns positive on one side and negative on the other
	// Essentially defined as a straight line between two points (vertices)
	float a;
	float b;
	float c;

	bool tie;

	EdgeEquation() = default;
	EdgeEquation(const Vertex& v0, const Vertex& v1);

	float evaluate(float x, float y) const;
	bool test(float x, float y) const;
	float test(float v) const;
};

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

	float evaluate(float x, float y) const;
};

class TriangleEquations
{
public:


	// The area the triangle covers on screen
	float area;

	// Equations for each edge of triangle
	EdgeEquation e0;
	EdgeEquation e1;
	EdgeEquation e2;

	// Parameters to interpolate across the triangle
	ParameterEquation z;
	ParameterEquation invw;
	ParameterEquation var[16];

	// Given 3 vertices and per vertex variables we calculate
	TriangleEquations(const Vertex &v0, const Vertex &v1, const Vertex &v2, int varCount);
};



