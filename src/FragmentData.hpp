#pragma once

#include "TriangleEquations.hpp"

class FragmentData
{
public:
	// Class containing per pixel data used by fragment shader

	float x; /// pixel x coordinate in triangle.
	float y; /// pixel y coordinate in triangle.

	float z; // triangle interpolated z value.
	float w; // triangle interpolated w value.
	float invw; // triangle interpolated 1 / w value.

	// Per pixel variable
	float var[16] = {0.0f};

	FragmentData();
	FragmentData(const TriangleEquations &teqn, float x, float y, int varCount, bool interpolateZ, bool interpolateW);
	void stepX(const TriangleEquations &teqn, int varCount, bool interpolateZ, bool interpolateW);
	void stepY(const TriangleEquations &teqn, int varCount, bool interpolateZ, bool interpolateW);
};