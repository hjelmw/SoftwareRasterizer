//////////////////////////////////////////////////////////////////
///							EdgeData
///
/// Stores the computed edge values for a given triangle
///
//////////////////////////////////////////////////////////////////


#pragma once

#include "TriangleEquations.hpp"

class EdgeData
{
public:
	float ev0;
	float ev1;
	float ev2;

	/// Initialize the edge data values.
	void init(const TriangleEquations& eqn, float x, float y);

	/// Step the edge values in the x direction.
	void stepX(const TriangleEquations& eqn);
	void stepX(const TriangleEquations& eqn, float stepSize);

	/// Step the edge values in the y direction.
	void stepY(const TriangleEquations& eqn);
	void stepY(const TriangleEquations& eqn, float stepSize);
	
	/// Test for triangle intersection.
	bool test(const TriangleEquations& eqn);
};