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

	float evaluate(float x, float y);
	bool test(float x, float y);
	float test(float v);

	float stepX(float v) const;
	float stepX(float v, float stepSize) const;
	float stepY(float v) const;
	float stepY(float v, float stepSize) const;
};