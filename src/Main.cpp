//#define TINYOBJLOADER_IMPLEMENTATION // define this in only *one* .cc
//#include "tiny_obj_loader.hpp"

#include "SDL.h"
#include "Rasterizer.hpp"
#include "VectorMath.hpp"

#include<vector>

typedef vmath::vec3<float> vec3f;
typedef vmath::vec4<float> vec4f;
typedef vmath::mat4<float> mat4f;

static SDL_Surface* m_surface;

class BasicVertexShader : public VertexShaderBase<BasicVertexShader> {
public:
	static const int attribCount = 1;
	static const int varCount = 3;

	static mat4f modelViewProjectionMatrix;

	static void processVertex(Vertex* in[], Vertex* out)
	{
		const Vertex* vertexData = in[0];

		vec4f position = modelViewProjectionMatrix * vec4f(vertexData->x, vertexData->y, vertexData->z, vertexData->w);

		out->x = position.x;
		out->y = position.y;
		out->z = position.z;
		out->w = position.w;
		out->var[0] = vertexData->r;
		out->var[1] = vertexData->g;
		out->var[2] = vertexData->b;
	}
};

class BasicFragmentShader : public FragmentShaderBase<BasicFragmentShader> {
public:
	static const bool interpolateZ = false;
	static const bool interpolateW = false;
	static const int varCount = 3;

	static SDL_Surface* surface;

	static void drawPixel(const FragmentData& p)
	{
		int rint = (int)(p.var[0] * 255);
		int gint = (int)(p.var[1] * 255);
		int bint = (int)(p.var[2] * 255);

		Uint32 color = rint << 16 | gint << 8 | bint;

		Uint32* buffer = (Uint32*)((Uint8*)surface->pixels + (int)p.y * surface->pitch + (int)p.x * 4);
		*buffer = color;
	}
};

SDL_Surface* BasicFragmentShader::surface;

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
	BasicFragmentShader::surface = m_surface;

	Rasterizer::VertexData vdata[3];

	vdata[0].x = 100.0f;
	vdata[0].y = 400.0f;
	vdata[0].z = 100.0f;
	vdata[0].w = 1.0f;
	vdata[0].r = 1.0f;
	vdata[0].g = 0.0f;
	vdata[0].b = 0.0f;


	vdata[1].x = 400.0f;
	vdata[1].y = 100.0f;
	vdata[1].z = 0.0f;
	vdata[1].w = 1.0f;
	vdata[1].r = 0.0f;
	vdata[1].g = 1.0f;
	vdata[1].b = 0.0f;

	vdata[2].x = 600.0f;
	vdata[2].y = 400.0f;
	vdata[2].z = 0.0f;
	vdata[2].w = 1.0f;
	vdata[2].r = 0.0f;
	vdata[2].g = 0.0f;
	vdata[2].b = 1.0f;

	int idata[3];
	idata[0] = 0;
	idata[1] = 1;
	idata[2] = 2;

	Rasterizer r;

	r.setScissorRect(0, 0, 768, 640);
	r.setFragmentShader<BasicFragmentShader>();
	r.setVertexShader<BasicVertexShader>();
	r.setVertexAttribPointer(0, sizeof(Rasterizer::VertexData), vdata);


	mat4f lookAtMatrix = vmath::lookat_matrix(vec3f(3.0f, 2.0f, 5.0f), vec3f(0.0f), vec3f(0.0f, 1.0f, 0.0f));
	mat4f perspectiveMatrix = vmath::perspective_matrix(60.0f, 4.0f / 3.0f, 0.1f, 10.0f);
	BasicVertexShader::modelViewProjectionMatrix = lookAtMatrix * perspectiveMatrix;

	r.drawElements(3, idata);
	//r.drawTriangle(v0, v1, v2);

	SDL_UpdateWindowSurface(window);

	//std::string inputfile = "../data/box.obj";
	//tinyobj::attrib_t attrib;
	//std::vector<tinyobj::shape_t> shapes;
	//std::vector<tinyobj::material_t> materials;

	//std::string warn;
	//std::string err;

	//bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, inputfile.c_str());


	SDL_Event e;
	while (SDL_WaitEvent(&e) && e.type != SDL_QUIT);

	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}