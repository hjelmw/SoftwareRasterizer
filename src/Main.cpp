#include "SDL.h"

#include "Rasterizer.hpp"
#include "InputOutputManager.hpp"

#include<vector>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

struct VertexArrayData {
	vmath::vec3<float> vertex;
	vmath::vec3<float> normal;
	vmath::vec2<float> texcoord;
};

class BasicVertexShader : public VertexShaderBase<BasicVertexShader> {
public:
	static const int attribCount = 1;
	static const int varCount = 5;

	static mat4f modelViewProjectionMatrix;
	static mat4f transformMatrix;

	static void processVertex(void* in[], Vertex* out)
	{
		const Rasterizer::VertexArrayData* vertexData = static_cast<const Rasterizer::VertexArrayData*>(in[0]);

		vec3f translation = vec3f(transformMatrix.elem[3][0], transformMatrix.elem[3][1], transformMatrix.elem[3][2]);


		float elapsedTime = SDL_GetTicks() / 1000.0f;

		mat3f rotationY = mat3f(
			vec3f(cos(elapsedTime), 0, -sin(elapsedTime)), // col 0
			vec3f(0, 1, 0), // col 1
			vec3f(sin(elapsedTime), 0, cos(elapsedTime)) // col 2
		);

		vec3f transformed_vertex = vertexData->vertex;//* rotationY;


		// Bring vertex into clip space
		vec4f position = modelViewProjectionMatrix * vec4f(transformed_vertex, 1.0f);

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
mat4f BasicVertexShader::transformMatrix;

int main(int argc, char* argv[])
{

	// Setup window manager
	SDL_Init(SDL_INIT_VIDEO);
	SDL_Window* window = SDL_CreateWindow(
		"Software Rasterizer",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		WINDOW_WIDTH,
		WINDOW_HEIGHT,
		0
	);

	// This is the main buffer we will draw into
	SDL_Surface* renderSurface = SDL_GetWindowSurface(window);

	Rasterizer softwareRasterizer(WINDOW_WIDTH, WINDOW_HEIGHT);

	// Setup matrices
	mat4f lookAtMatrix2 = vmath::lookat_matrix(
		vec3f(2.0f, 1.0f, 1.5f), // Camera position XYZ
		vec3f(0.0f, 0.0f, 0.0f), // Look at origin
		vec3f(0.0f, 1.0f, 0.0f)); // Y-axis up
	mat4f perspectiveMatrix = vmath::perspective_matrix(
		90.0f, // FOV 
		4.0f / 3.0f, // Aspect ratio
		0.1f, // Near plane
		10.0f); // Far plane
	
	// Setup shader constants
	//BasicVertexShader::modelViewProjectionMatrix = perspectiveMatrix * lookAtMatrix;
	//BasicFragmentShader::surface = renderSurface;

	// Configure rasterizer
	softwareRasterizer.setScissorRect(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
	softwareRasterizer.setViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
	softwareRasterizer.setDepthRange(0.0f, 1.0f);
	softwareRasterizer.setVertexShader<BasicVertexShader>();
	softwareRasterizer.setFragmentShader<BasicFragmentShader>();

	// Load vertices into rasterizer
	std::vector<Rasterizer::VertexArrayData> vertexData;
	std::vector<int> indexData;

	mat4f objectWorld1 = mat4f(
		vec4f(0.70f, 0.0f, 0.70f, 0.0f), // Rotation
		vec4f(0.0f, 1.0f, 0.0f, 0.0f),
		vec4f(-0.70f, 0.0f, 0.70f, 0.0f),
		vec4f(0.0f, 0.0f, 3.0f, 1.0f) // Translation
	);

	softwareRasterizer.loadModelIntoVertexArray("data/monkey.obj", "data/box.png", objectWorld1, vertexData, indexData);
	softwareRasterizer.setVertexAttribPointer(0, sizeof(Rasterizer::VertexArrayData), &vertexData[0]);


	InputOutputManager inputOutputManager = InputOutputManager();


	mat4f inputMatrix = vmath::identity4<float>();

	mat4f lookAtMatrix;

	while (true)
	{
		SDL_FillRect(renderSurface, NULL, 0x00000000);

		float elapsedTime = ((float)SDL_GetTicks()) / 1000.0f;

		inputOutputManager.processInput();
		inputOutputManager.updateCameraTransforms(lookAtMatrix, 0.1f);


		BasicVertexShader::modelViewProjectionMatrix = perspectiveMatrix * lookAtMatrix;
		BasicVertexShader::transformMatrix = inputMatrix;
		BasicFragmentShader::surface = renderSurface;



		// Draw into buffer
		softwareRasterizer.drawTriangles((int)indexData.size(), &indexData[0]);

		SDL_UpdateWindowSurface(window);


	}

	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}