#include "TriangleEquations.hpp"

TriangleEquations::TriangleEquations(const Vertex& v0, const Vertex& v1, const Vertex& v2)
{
	this->e0 = EdgeEquation(v0, v1);
	this->e1 = EdgeEquation(v1, v2);
	this->e2 = EdgeEquation(v2, v0);

	this->area = 0.5f * (e0.c + e1.c + e2.c);

	if (this->area < 0.0f)
		return;

	this->r = ParameterEquation(v0.r, v1.r, v2.r, e0, e1, e2, area);
	this->g = ParameterEquation(v0.g, v1.g, v2.g, e0, e1, e2, area);
	this->b = ParameterEquation(v0.b, v1.b, v2.b, e0, e1, e2, area);
}