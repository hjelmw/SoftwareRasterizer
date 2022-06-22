#pragma once

#include "TriangleEquations.hpp"


//
//					Fragment shader data
// 
//		Class containing vertex interpolated per pixel data.
//		Used by fragment shader to draw into main buffer
//



class FragmentData
{
public:

	float x; /// pixel x coordinate in triangle.
	float y; /// pixel y coordinate in triangle.

	float z; // triangle interpolated z value.
	float w; // triangle interpolated w value.
	float invw; // triangle interpolated 1 / w value.

	// Interpolated per pixel variables
	float var[16] = {0.0f};

	FragmentData();
	FragmentData(const TriangleEquations &teqn, float x, float y, int varCount, bool interpolateZ, bool interpolateW);
};