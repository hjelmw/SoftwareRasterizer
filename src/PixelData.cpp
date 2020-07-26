
#include "PixelData.hpp"


/// Initialize pixel data for the given pixel coordinates.
void PixelData::init(const TriangleEquations &eqn, float x, float y)
{
	this->r = eqn.r.evaluate(x, y);
	this->g = eqn.g.evaluate(x, y);
	this->b = eqn.b.evaluate(x, y);
}

/// Step all the pixel data in the x direction.
void PixelData::stepX(const TriangleEquations& eqn)
{
	this->r = eqn.r.stepX(this->r);
	this->g = eqn.g.stepX(this->g);
	this->b = eqn.b.stepX(this->b);
}

/// Step all the pixel data in the y direction.
void PixelData::stepY(const TriangleEquations& eqn)
{
	this->r = eqn.r.stepY(this->r);
	this->g = eqn.g.stepY(this->g);
	this->b = eqn.b.stepY(this->b);
}