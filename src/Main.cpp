#include "Vertex.hpp"

#include <Rasterizer.hpp>
#include "PixelShaderBase.hpp"

#include "TriangleEquations.hpp"
#include "EdgeData.hpp"
#include "PixelData.hpp"

#include <algorithm>
#include "SDL.h"



static SDL_Surface* m_surface;

class BasicPixelShader : public PixelShaderBase<BasicPixelShader> {
public:
	static const bool interpolateZ = false;
	static const bool interpolateW = false;
	static const int varCount = 3;

	static SDL_Surface* surface;

	static void drawPixel(const PixelData& p)
	{
		int rint = (int)(p.var[0] * 255);
		int gint = (int)(p.var[1] * 255);
		int bint = (int)(p.var[2] * 255);

		Uint32 color = rint << 16 | gint << 8 | bint;

		Uint32* buffer = (Uint32*)((Uint8*)surface->pixels + (int)p.y * surface->pitch + (int)p.x * 4);
		*buffer = color;
	}
};

SDL_Surface* BasicPixelShader::surface;

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
	BasicPixelShader::surface = m_surface;

	Vertex v0(100.0f, 200.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f);
	Vertex v1(250.0f, 400.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
	Vertex v2(100.0f, 420.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f);

	Vertex v3(650.0f, 100.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f);
	Vertex v4(740.0f, 200.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
	Vertex v5(590.0f, 200.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f);

	Vertex v6(400.0f, 300.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f);
	Vertex v7(440.0f, 600.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
	Vertex v8(290.0f, 640.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f);

	Rasterizer r;

	r.setScissorRect(0, 0, 768, 640);
	r.setPixelShader<BasicPixelShader>();


	r.drawTriangle(v0, v1, v2);
	r.drawTriangle(v3, v4, v5);
	r.drawTriangle(v6, v7, v8);


	SDL_UpdateWindowSurface(window);

	SDL_Event e;
	while (SDL_WaitEvent(&e) && e.type != SDL_QUIT);

	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}