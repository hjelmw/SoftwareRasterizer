
#include "PixelData.hpp"


/// Initialize pixel data for the given pixel coordinates.
void PixelData::init(const TriangleEquations &eqn, float x, float y)
{
	r = eqn.r.evaluate(x, y);
	g = eqn.g.evaluate(x, y);
	b = eqn.b.evaluate(x, y);
}

/// Step all the pixel data in the x direction.
void PixelData::stepX(const TriangleEquations& eqn)
{
	r = eqn.r.stepX(r);
	g = eqn.g.stepX(g);
	b = eqn.b.stepX(b);
}

/// Step all the pixel data in the y direction.
void PixelData::stepY(const TriangleEquations& eqn)
{
	r = eqn.r.stepY(r);
	g = eqn.g.stepY(g);
	b = eqn.b.stepY(b);
}