#include "SDL.h"
#include <Rasterizer.hpp>



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

	Vertex v0, v1, v2;

	v0.x = 100.0f;
	v0.y = 200.0f;
	v0.z = 0.0f;
	v0.var[0] = 1.0f;
	v0.var[1] = 0.0f;
	v0.var[2] = 0.0f;

	v1.x = 250.0f;
	v1.y = 100.0f;
	v1.z = 0.0f;
	v1.var[0] = 0.0f;
	v1.var[1] = 1.0f;
	v1.var[2] = 0.0f;

	v2.x = 100.0f;
	v2.y = 420.0f;
	v2.z = 0.0f;
	v2.var[0] = 0.0f;
	v2.var[1] = 0.0f;
	v2.var[2] = 1.0f;


	Rasterizer r;

	r.setScissorRect(0, 0, 768, 640);
	r.setPixelShader<BasicPixelShader>();

	r.drawTriangle(v0, v1, v2);

	SDL_UpdateWindowSurface(window);

	SDL_Event e;
	while (SDL_WaitEvent(&e) && e.type != SDL_QUIT);

	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}