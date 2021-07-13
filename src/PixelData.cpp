
#include "PixelData.hpp"


PixelData::PixelData()
{
	this->x = 0.0f;
	this->y = 0.0f;
	this->z = 0.0f;
	this->w = 0.0f;

	this->invw = 0.0f;


}

/// Initialize pixel data for the given pixel coordinates.
PixelData::PixelData(const TriangleEquations& teqn, float x, float y, int varCount, bool interpolateZ, bool interpolateW)
{
	// Default values
	this->x = x;
	this->y = x;
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

/// Step all the pixel data in the x direction.
void PixelData::stepX(const TriangleEquations& teqn, int varCount, bool interpolateZ, bool interpolateW)
{
	if (interpolateZ)
		this->z = teqn.z.stepX(z);

	if (interpolateW || varCount > 0)
	{
		this->invw = teqn.invw.stepX(invw);
		this->w = 1.0f / invw;
	}

	for (int i = 0; i < varCount; ++i)
		this->var[i] = teqn.var[i].stepX(var[i]);
}

/// Step all the pixel data in the y direction.
void PixelData::stepY(const TriangleEquations& teqn, int varCount, bool interpolateZ, bool interpolateW)
{
	if (interpolateZ)
		this->z = teqn.z.stepY(z);

	if (interpolateW || varCount > 0)
	{
		this->invw = teqn.invw.stepY(invw);
		this->w = 1.0f / invw;
	}

	for (int i = 0; i < varCount; ++i)
		this->var[i] = teqn.var[i].stepY(var[i]);
}