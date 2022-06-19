#include "SDL.h"

#include "Rasterizer.hpp"
#include "Renderer.hpp"
#include <stdlib.h>     /* srand, rand */
#include<vector>

class BasicVertexShader : public VertexShaderBase<BasicVertexShader> {
public:
	static const int attribCount = 1;
	static const int varCount = 5;

	// Declare mvp matrix so we can use it
	static mat4f modelViewProjectionMatrix;

	static void processVertex(void* in[], Vertex* out)
	{
		const Rasterizer::VertexArrayData* vertexData = static_cast<const Rasterizer::VertexArrayData*>(in[0]);

		// Bring vertex into clip space
		vec4f position = modelViewProjectionMatrix * vec4f(vertexData->vertex, 1.0f);

		out->x = position.x;
		out->y = position.y;
		out->z = position.z;
		out->w = position.w;

		// Remap from [-1, 1] to [0, 1]
		vec3f remapped_normal = vertexData->normal * 0.5f + 0.5f;

		out->var[0] = remapped_normal.x;
		out->var[1] = remapped_normal.y;
		out->var[2] = remapped_normal.z;

		out->var[3] = vertexData->texcoord.x;
		out->var[4] = vertexData->texcoord.y;
	}
};

class BasicFragmentShader : public FragmentShaderBase<BasicFragmentShader> {
public:
	static const bool interpolateZ = true;
	static const bool interpolateW = true;
	static const int varCount = 5;

	static SDL_Surface* surface;
	//static SDL_Surface* texture;

	static void processFragment(const FragmentData& p)
	{
		vec3f normal = vec3f(p.var[0], p.var[1], p.var[2]);

		int rint = (int)(p.var[0] * 255);
		int gint = (int)(p.var[1] * 255);
		int bint = (int)(p.var[2] * 255);

		Uint32 color = rint << 16 | gint << 8 | bint;

		// Pointer to pixel on screen
		//Uint32* texBuffer = (Uint32*)((Uint8*)texture->pixels + (size_t)ty * (size_t)texture->pitch + (size_t)tx * 4);
		Uint32* buffer = (Uint32*)((Uint8 *)surface->pixels + (size_t)p.y * surface->pitch + (size_t)p.x * 4);

		if (color == 0)
			*buffer = (255 << 16 | 53 << 8 | 184);
		else
			*buffer = color;
	}
};

SDL_Surface* BasicFragmentShader::surface;

mat4f BasicVertexShader::modelViewProjectionMatrix;

int main(int argc, char* argv[])
{
	SDL_Init(SDL_INIT_VIDEO);

	SDL_Window* window = SDL_CreateWindow(
		"Software Rasterizer",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		800,
		600,
		0
	);

	//SDL_Surface* tmp = IMG_Load("data/box.png");

	SDL_Surface* viewport = SDL_GetWindowSurface(window);
	BasicFragmentShader::surface = viewport;
	Rasterizer::VertexData vdata[3];
	int idata[3];

	vdata[0].x = 0.0f;
	vdata[0].y = 0.5f;
	vdata[0].z = 0.0f;
	vdata[0].w = 1.0f;
	vdata[0].r = 1.0f;
	vdata[0].g = 0.0f;
	vdata[0].b = 0.0f;

	vdata[1].x = -1.5f;
	vdata[1].y = -0.5f;
	vdata[1].z = 0.0f;
	vdata[1].w = 1.0f;
	vdata[1].r = 0.0f;
	vdata[1].g = 1.0f;
	vdata[1].b = 0.0f;

	vdata[2].x = 1.5f;
	vdata[2].y = -0.5f;
	vdata[2].z = 0.0f;
	vdata[2].w = 1.0f;
	vdata[2].r = 0.0f;
	vdata[2].g = 0.0f;
	vdata[2].b = 1.0f;

	idata[0] = 0;
	idata[1] = 1;
	idata[2] = 2;


	Rasterizer::VertexArrayData vdata2[3];
	int idata2[3];

	vdata2[0].vertex = vec3f(0.0f, 0.5f, 0.0f);
	vdata2[1].vertex = vec3f(-1.5f, -0.5f, 0.0f);
	vdata2[2].vertex = vec3f(1.5f, -0.5f, 0.0f);
	
	vdata2[0].normal = vec3f(1.0f, 0.0f, 0.0f);
	vdata2[1].normal = vec3f(0.0f, 1.0f, 0.0f);
	vdata2[2].normal = vec3f(0.0f, 0.0f, 1.0f);

	idata2[0] = 0;
	idata2[1] = 1;
	idata2[2] = 2;

	Rasterizer r = Rasterizer();

	r.setScissorRect(0, 0, 800, 600);
	r.setViewport(0, 0, 800, 600);
	r.setDepthRange(0.0f, 1.0f);
	r.setVertexShader<BasicVertexShader>();
	r.setFragmentShader<BasicFragmentShader>();
	//r.setVertexAttribPointer(0, sizeof(Rasterizer::VertexData), vdata);

	mat4f lookAtMatrix = vmath::lookat_matrix(
		vec3f(2.0f, 1.0f, 1.5f), // Camera position XYZ
		vec3f(0.0f, 0.0f, 0.0f), // Look at origin
		vec3f(0.0f, 1.0f, 0.0f)); // Y-axis up
	mat4f perspectiveMatrix = vmath::perspective_matrix(
		90.0f, // FOV 
		4.0f / 3.0f, // Aspect ratio
		0.1f, // Near plane
		10.0f); // Far plane
	BasicVertexShader::modelViewProjectionMatrix = perspectiveMatrix * lookAtMatrix;

	//r.setVertexAttribPointer(0, sizeof(Rasterizer::VertexArrayData), &vdata2[0]);
	//r.drawTriangles(3, idata2);

	std::vector<Rasterizer::VertexArrayData> vdata3;
	std::vector<int> idata3;

	Renderer renderer = Renderer();
	renderer.LoadModel("data/monkey.obj", "data/box.png", vdata3, idata3);

	r.setVertexAttribPointer(0, sizeof(Rasterizer::VertexArrayData), &vdata3[0]);
	
	r.drawTriangles((int) idata3.size(), &idata3[0]);

	SDL_UpdateWindowSurface(window);

	SDL_Event e;
	while (SDL_WaitEvent(&e) && e.type != SDL_QUIT);

	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}