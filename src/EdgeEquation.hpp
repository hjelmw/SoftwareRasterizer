//////////////////////////////////////////////////////////////////
///							EdgeEquation
///
/// 
///	 
//////////////////////////////////////////////////////////////////
///							EdgeEquation
///
/// Used to span a triangle 
///  and determine what pixels it should occupy on the screen
///	 
//////////////////////////////////////////////////////////////////



#pragma once

#include "Vertex.hpp"

class EdgeEquation
{
public:
	float a;
	float b;
	float c;

	bool tie;

	EdgeEquation() = default;
	EdgeEquation(const Vertex& v0, const Vertex& v1);

	/// Evaluate the edge equation for the given point.
	float evaluate(float x, float y) const;

	/// Test if the given point is inside the edge.
	bool test(float x, float y);

	/// Test for a given value.
	float test(float v);

	/// Step the equation value v to the x direction.
	float stepX(float v) const;
	float stepX(float v, float stepSize) const;

	/// Step the equation value v to the y direction.
	float stepY(float v) const;
	float stepY(float v, float stepSize) const;
};