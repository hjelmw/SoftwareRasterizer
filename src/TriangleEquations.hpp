#pragma once

#include "EdgeEquation.hpp"
#include "ParameterEquation.hpp"

class TriangleEquations
{
public:
	// The area the triangle covers on screen
	float area;

	EdgeEquation e0;
	EdgeEquation e1;
	EdgeEquation e2;

	ParameterEquation r;
	ParameterEquation g;
	ParameterEquation b;

	// Given 3 vertices we calculate 
	TriangleEquations(const Vertex &v0, const Vertex &v1, const Vertex &v2);
};