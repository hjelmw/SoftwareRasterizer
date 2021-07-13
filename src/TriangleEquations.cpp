#include "TriangleEquations.hpp"

TriangleEquations::TriangleEquations(const Vertex& v0, const Vertex& v1, const Vertex& v2, int varCount)
{
	this->e0 = EdgeEquation(v0, v1);
	this->e1 = EdgeEquation(v1, v2);
	this->e2 = EdgeEquation(v2, v0);

	this->area = (e0.c + e1.c + e2.c);

	if (this->area < 0.0f)
		return;

	float factor = 1.0f / area;

	this->z = ParameterEquation(v0.z, v1.z, v2.z, e0, e1, e2, factor);

	float invw0 = 1.0f / v0.w;
	float invw1 = 1.0f / v1.w;
	float invw2 = 1.0f / v2.w;

	this->invw = ParameterEquation(invw0, invw1, invw2, e0, e1, e2, factor);

	for (int i = 0; i < varCount; ++i)
		this->var[i] = ParameterEquation(v0.var[i], v1.var[i], v2.var[i], e0, e1, e2, factor);
}