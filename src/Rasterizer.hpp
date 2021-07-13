#pragma once


#include <algorithm>

#include "EdgeEquation.hpp"
#include "ParameterEquation.hpp"
#include "TriangleEquations.hpp"
#include "PixelData.hpp"
#include "EdgeData.hpp"
#include "PixelShaderBase.hpp"

#pragma warning (disable: 6294 6201)

// Rasterizer main class.
class Rasterizer
{
private:
	int m_minX;
	int m_maxX;
	int m_minY;
	int m_maxY;

	void (Rasterizer::*m_triangleFunc)(const Vertex& v0, const Vertex& v1, const Vertex& v2) const;

public:
	/// Constructor.
	Rasterizer() {};

	/// Set the scissor rectangle.
	void setScissorRect(int x, int y, int width, int height)
	{
		this->m_minX = x;
		this->m_minY = y;
		this->m_maxX = x + width;
		this->m_maxY = y + height;
	}

	/// Set the pixel shader.
	template <class PixelShader>
	void setPixelShader()	
	{
		this->m_triangleFunc = &Rasterizer::drawTriangleBlockTemplate<PixelShader>;
	}

	// Draw a single triangle.
	void drawTriangle(const Vertex& v0, const Vertex& v1, const Vertex& v2) const
	{
		(this->*m_triangleFunc)(v0, v1, v2);
	}

private:
	bool scissorTest(float x, float y) const
	{
		return (x >= m_minX && x < m_maxX&& y >= m_minY && y < m_maxY);
	}

	template <class PixelShader>
	void drawTriangleBlockTemplate(const Vertex& v0, const Vertex& v1, const Vertex& v2) const
	{
		// Compute triangle equations.
		TriangleEquations teqn(v0, v1, v2, PixelShader::varCount);

		// Check if triangle is backfacing.
		if (teqn.area <= 0)
			return;

		// Compute triangle bounding box.
		int minX = (int)std::min(std::min(v0.x, v1.x), v2.x);
		int maxX = (int)std::max(std::max(v0.x, v1.x), v2.x);
		int minY = (int)std::min(std::min(v0.y, v1.y), v2.y);
		int maxY = (int)std::max(std::max(v0.y, v1.y), v2.y);

		// Clip to scissor rect.
		minX = std::max(minX, m_minX);
		maxX = std::min(maxX, m_maxX);
		minY = std::max(minY, m_minY);
		maxY = std::min(maxY, m_maxY);

		int blockSize = 8;

		// Round to block grid.
		minX = minX & ~(blockSize - 1);
		maxX = maxX & ~(blockSize - 1);
		minY = minY & ~(blockSize - 1);
		maxY = maxY & ~(blockSize - 1);

		float s = (float) blockSize - 1;

		int stepsX = (maxX - minX) / blockSize + 1;
		int stepsY = (maxY - minY) / blockSize + 1;

		//TODO
		//#pragma omp parallel for
		for (int i = 0; i < stepsX * stepsY; ++i)
		{
			int sx = i % stepsX;
			int sy = i / stepsX;

			// Add 0.5 to sample at pixel centers.
			int x = minX + sx * blockSize;
			int y = minY + sy * blockSize;

			float xf = x + 0.5f;
			float yf = y + 0.5f;

			// Test if block is inside or outside triangle or touches it.
			EdgeData e00; e00.init(teqn, xf, yf);
			EdgeData e01 = e00; e01.stepY(teqn, s);
			EdgeData e10 = e00; e10.stepX(teqn, s);
			EdgeData e11 = e01; e11.stepX(teqn, s);

			bool e00_0 = teqn.e0.test(e00.ev0), e00_1 = teqn.e1.test(e00.ev1), e00_2 = teqn.e2.test(e00.ev2), e00_all = e00_0 && e00_1 && e00_2;
			bool e01_0 = teqn.e0.test(e01.ev0), e01_1 = teqn.e1.test(e01.ev1), e01_2 = teqn.e2.test(e01.ev2), e01_all = e01_0 && e01_1 && e01_2;
			bool e10_0 = teqn.e0.test(e10.ev0), e10_1 = teqn.e1.test(e10.ev1), e10_2 = teqn.e2.test(e10.ev2), e10_all = e10_0 && e10_1 && e10_2;
			bool e11_0 = teqn.e0.test(e11.ev0), e11_1 = teqn.e1.test(e11.ev1), e11_2 = teqn.e2.test(e11.ev2), e11_all = e11_0 && e11_1 && e11_2;

			int result = e00_all + e01_all + e10_all + e11_all;

			// Potentially all out.
			if (result == 0)
			{
				// Test for special case.
				bool e00Same = e00_0 == e00_1 == e00_2;
				bool e01Same = e01_0 == e01_1 == e01_2;
				bool e10Same = e10_0 == e10_1 == e10_2;
				bool e11Same = e11_0 == e11_1 == e11_2;

				if (!e00Same || !e01Same || !e10Same || !e11Same)
					PixelShader::template drawBlock<true>(teqn, x, y);
			}
			else if (result == 4)
			{
				// Fully Covered.
				PixelShader::template drawBlock<false>(teqn, x, y);
			}
			else
			{
				// Partially Covered.
				PixelShader::template drawBlock<true>(teqn, x, y);
			}
		}
	}
};
