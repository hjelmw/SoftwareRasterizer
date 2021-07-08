#include "Vertex.hpp"
#include "TriangleEquations.hpp"
#include "EdgeData.hpp"
#include "PixelData.hpp"
#include "SDL.h"

#include <algorithm>


SDL_Surface* m_surface;

void putpixel(SDL_Surface* surface, int x, int y, Uint32 pixel)
{
	int bpp = surface->format->BytesPerPixel;

	/* Here p is the address to the pixel we want to set */
	Uint8* p = (Uint8*)surface->pixels + y * surface->pitch + x * bpp;

	switch (bpp) {
	case 1:
		*p = pixel;
		break;

	case 2:
		*(Uint16*)p = pixel;
		break;

	case 3:
		if (SDL_BYTEORDER == SDL_BIG_ENDIAN) {
			p[0] = (pixel >> 16) & 0xff;
			p[1] = (pixel >> 8) & 0xff;
			p[2] = pixel & 0xff;
		}
		else {
			p[0] = pixel & 0xff;
			p[1] = (pixel >> 8) & 0xff;
			p[2] = (pixel >> 16) & 0xff;
		}
		break;

	case 4:
		*(Uint32*)p = pixel;
		break;
	}
}

//void drawTriangle(const Vertex& v0, const Vertex& v1, const Vertex& v2)
//{
//	// Compute triangle bounding box.
//
//	int minX = std::min(std::min(v0.x, v1.x), v2.x);
//	int maxX = std::max(std::max(v0.x, v1.x), v2.x);
//	int minY = std::min(std::min(v0.y, v1.y), v2.y);
//	int maxY = std::max(std::max(v0.y, v1.y), v2.y);
//
//	// Clip to scissor rect.
//
//	minX = std::max(minX, 0);
//	maxX = std::min(maxX, 768);
//	minY = std::max(minY, 0);
//	maxY = std::min(maxY, 640);
//
//	// Compute edge equations.
//
//	EdgeEquation e0(v0, v1);
//	EdgeEquation e1(v1, v2);
//	EdgeEquation e2(v2, v0);
//
//	float area = 0.5 * (e0.c + e1.c + e2.c);
//
//	ParameterEquation r(v0.r, v1.r, v2.r, e0, e1, e2, area);
//	ParameterEquation g(v0.g, v1.g, v2.g, e0, e1, e2, area);
//	ParameterEquation b(v0.b, v1.b, v2.b, e0, e1, e2, area);
//
//	// Check if triangle is backfacing.
//
//	if (area < 0)
//		return;
//
//	// Add 0.5 to sample at pixel centers.
//
//	for (float x = minX + 0.5f, xm = maxX + 0.5f; x <= xm; x += 1.0f)
//		for (float y = minY + 0.5f, ym = maxY + 0.5f; y <= ym; y += 1.0f)
//		{
//			if (e0.test(x, y) && e1.test(x, y) && e2.test(x, y))
//			{
//				int rint = r.evaluate(x, y) * 255;
//				int gint = g.evaluate(x, y) * 255;
//				int bint = b.evaluate(x, y) * 255;
//				Uint32 color = SDL_MapRGB(m_surface->format, rint, gint, bint);
//				putpixel(m_surface, x, y, color);
//			}
//		}
//}

template <bool TestEdges>
void rasterizeBlock(const TriangleEquations& teqn, float x, float y)
{
	PixelData po;
	po.init(teqn, x, y);

	EdgeData eo;
	if (TestEdges)
		eo.init(teqn, x, y);

	int blockSize = 8;

	for (float yy = y; yy < y + blockSize; yy += 1.0f)
	{
		PixelData pi = po;

		EdgeData ei;
		if (TestEdges)
			ei = eo;

		for (float xx = x; xx < x + blockSize; xx += 1.0f)
		{
			if (!TestEdges || ei.test(teqn))
			{
				int rint = (int)(pi.r * 255);
				int gint = (int)(pi.g * 255);
				int bint = (int)(pi.b * 255);
				Uint32 color = SDL_MapRGB(m_surface->format, rint, gint, bint);
				putpixel(m_surface, (int)xx, (int)yy, color);
			}

			pi.stepX(teqn);
			if (TestEdges)
				ei.stepX(teqn);
		}

		po.stepY(teqn);
		if (TestEdges)
			eo.stepY(teqn);
	}
}

void drawTriangle(const Vertex& v0, const Vertex& v1, const Vertex& v2)
{
	TriangleEquations teqn(v0, v1, v2);

	// Compute triangle bounding box.
	
	int minX = std::min(std::min(v0.x, v1.x), v2.x);
	int maxX = std::max(std::max(v0.x, v1.x), v2.x);
	int minY = std::min(std::min(v0.y, v1.y), v2.y);
	int maxY = std::max(std::max(v0.y, v1.y), v2.y);
	
	// Clip to scissor rect.
	
	minX = std::max(minX, 0);
	maxX = std::min(maxX, 768);
	minY = std::max(minY, 0);
	maxY = std::min(maxY, 640);
	
	float area = 0.5f * (teqn.e0.c + teqn.e1.c + teqn.e2.c);
	
	// Check if triangle is backfacing.	
	if (area < 0)
		return;

	// Round to block grid.
	int blockSize = 8;
	
	minX = minX & ~(blockSize - 1);
	maxX = maxX & ~(blockSize - 1);
	minY = minY & ~(blockSize - 1);
	maxY = maxY & ~(blockSize - 1);

	float s = (float)blockSize - 1;

	// Add 0.5 to sample at pixel centers
	for (float x = minX + 0.5f, xm = maxX + 0.5f; x <= xm; x += blockSize)
	for (float y = minY + 0.5f, ym = maxY + 0.5f; y <= ym; y += blockSize)
	{
		// Test if block is inside or outside triangle or touches it
		EdgeData e00; e00.init(teqn, x, y);
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

			// TODO
			//if (!e00Same || !e01Same || !e10Same || !e11Same)
			//	PixelShader::template drawBlock<true>(eqn, x, y);
		}
		else if (result == 4)
		{
			// Fully Covered
			rasterizeBlock<false>(teqn, x, y);
		}
		else
		{
			// Partially Covered
			rasterizeBlock<true>(teqn, x, y);
		}
	}
}

int main(int argc, char* argv[])
{
	SDL_Init(SDL_INIT_VIDEO);

	SDL_Window* window = SDL_CreateWindow(
		"Software Rasterizer",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		768,
		640,
		0
	);

	m_surface = SDL_GetWindowSurface(window);

	Vertex v0(100.0f, 200.0f, 0.0f, 1.0f, 0.0f, 0.0f);
	Vertex v1(250.0f, 400.0f, 0.0f, 0.0f, 1.0f, 0.0f);
	Vertex v2(100.0f, 420.0f, 0.0f, 0.0f, 0.0f, 1.0f);

	Vertex v3(650.0f, 100.0f, 0.0f, 1.0f, 0.0f, 0.0f);
	Vertex v4(740.0f, 200.0f, 0.0f, 0.0f, 1.0f, 0.0f);
	Vertex v5(590.0f, 200.0f, 0.0f, 0.0f, 0.0f, 1.0f);

	Vertex v6(400.0f, 300.0f, 0.0f, 1.0f, 0.0f, 0.0f);
	Vertex v7(440.0f, 600.0f, 0.0f, 0.0f, 1.0f, 0.0f);
	Vertex v8(290.0f, 640.0f, 0.0f, 0.0f, 0.0f, 1.0f);

	drawTriangle(v0, v1, v2);
	drawTriangle(v3, v4, v5);
	drawTriangle(v6, v7, v8);

	SDL_UpdateWindowSurface(window);

	SDL_Event e;
	while (SDL_WaitEvent(&e) && e.type != SDL_QUIT);

	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}