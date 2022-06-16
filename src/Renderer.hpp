#pragma once

#include "VertexShaderBase.hpp"
#include "FragmentShaderBase.hpp"
#include "VectorMath.hpp"
#include "Rasterizer.hpp"

typedef vmath::vec3<float> vec3f;
typedef vmath::vec2<float> vec2f;

typedef vmath::vec4<float> vec4f;
typedef vmath::mat4<float> mat4f;

class Renderer
{
private:
	//Rasterizer* m_Rasterizer;

	//std::vector<int> m_Indices;
	//std::vector<Vertex> m_Vertices;

public:

	Renderer();
	~Renderer();

	// Initialize renderer
	void Init(const int screenWidth, const int screenHeight, const void* vertexShader, const void* fragmentShader);
	
	// Load a .obj, .png file into rasterizer as vertex arrays
	void LoadModel(const char* modelPath, char* texturePath, std::vector<Rasterizer::VertexArrayData>& vertexDataRef, std::vector<int>& indexDataRef);

	// Update Model-View Projection Matrix
	void UpdateTransform(vmath::mat4<float> mvpMatrix);

	// Render vertices on screen
	void Render();
}; 