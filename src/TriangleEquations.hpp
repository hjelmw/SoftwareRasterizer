#pragma once

#include "EdgeEquation.hpp"
#include "ParameterEquation.hpp"

class TriangleEquations
{
public:
	// The area the triangle covers on screen
	float area;

	// Equations for each edge of triangle
	EdgeEquation e0;
	EdgeEquation e1;
	EdgeEquation e2;

	// Parameters to interpolate colors of triangle
	ParameterEquation z;
	ParameterEquation invw;
	ParameterEquation var[16];

	// Given 3 vertices and per vertex variables we calculate
	TriangleEquations(const Vertex &v0, const Vertex &v1, const Vertex &v2, int varCount);
};