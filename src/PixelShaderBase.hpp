#pragma once

#include "TriangleEquations.hpp"
#include "PixelData.hpp"

#pragma warning (disable: 6201 6294) 

/// Pixel shader base class.
/** Derive your own pixel shaders from this class and redefine the static
	variables to match your pixel shader requirements. */
template <class Derived>
class PixelShaderBase {
public:
	/// Tells the rasterizer to interpolate the z component.
	static const int interpolateZ = false;

	/// Tells the rasterizer to interpolate the w component.
	static const int interpolateW = false;

	/// Tells the rasterizer how many per vertex vars to interpolate.
	static const int varCount = 3;

	template <bool TestEdges>
	static void drawBlock(const TriangleEquations& teqn, int x, int y)
	{
		float xf = x + 0.5f;
		float yf = y + 0.5f;

		PixelData po(teqn, xf, yf, Derived::varCount, Derived::interpolateZ, Derived::interpolateW);

		EdgeData eo;
		if (TestEdges)
			eo.init(teqn, xf, yf);

		int blockSize = 8;

		for (int yy = y; yy < y + blockSize; yy++)
		{
			PixelData pi = copyPixelData(po);

			EdgeData ei;
			if (TestEdges)
				ei = eo;

			for (int xx = x; xx < x + blockSize; xx++)
			{
				if (!TestEdges || ei.test(teqn))
				{
					pi.x = (float) xx;
					pi.y = (float) yy;
					Derived::drawPixel(pi);
				}

				pi.stepX(teqn, Derived::varCount, Derived::interpolateZ, Derived::interpolateW);
				if (TestEdges)
					ei.stepX(teqn);
			}

			po.stepY(teqn, Derived::varCount, Derived::interpolateZ, Derived::interpolateW);
			if (TestEdges)
				eo.stepY(teqn);
		}
	}
	/// This is called per pixel. 
	/** Implement this in your derived class to display single pixels. */
	static void drawPixel(const PixelData& p)
	{

	}

protected:
	static PixelData copyPixelData(PixelData& po)
	{
		PixelData pi;
		if (Derived::interpolateZ) pi.z = po.z;
		if (Derived::interpolateW) { pi.w = po.w; pi.invw = po.invw; }
		for (int i = 0; i < Derived::varCount; ++i)
			pi.var[i] = po.var[i];
		return pi;
	}

	class NullPixelShader : public PixelShaderBase<NullPixelShader> {};
};