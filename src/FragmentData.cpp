
#include "FragmentData.hpp"


FragmentData::FragmentData()
{
	this->x = 0.0f;
	this->y = 0.0f;
	this->z = 0.0f;
	this->w = 0.0f;

	this->invw = 0.0f;
}

/// Initialize fragment (pixel) data for the given pixel coordinates.
FragmentData::FragmentData(const TriangleEquations& teqn, float x, float y, int varCount, bool interpolateZ, bool interpolateW)
{
	// Default values
	this->x = x;
	this->y = y;
	this->z = 0.0f;
	this->w = 0.0f;
	this->invw = 0.0f;

	if (interpolateZ)
		this->z = teqn.z.evaluate(x, y);

	if (interpolateW || varCount > 0)
	{
		this->invw = teqn.invw.evaluate(x, y);
		this->w = 1.0f / invw;
	}

	for (int i = 0; i < varCount; ++i)
		this->var[i] = teqn.var[i].evaluate(x, y);
	
}