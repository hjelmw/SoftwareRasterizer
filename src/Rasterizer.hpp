#pragma once

#include "SDL.h"

#include <algorithm>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>

#include <iostream>
#include <map>



#include "Vertex.hpp"

#include "TriangleEquations.hpp"
#include "FragmentData.hpp"

#include "FragmentShaderBase.hpp"
#include "VertexShaderBase.hpp"

#include "ViewFrustumClipper.hpp"



//
//					This is the Rasterizer main class.
//
//		The rasterization starting point to look in is DrawTriangles()
//		It works by following the Graphics Pipeline conceptual model
//		
//		It additionally supports arbitrary (and interpolated) per vertex variables 
//		as well as programmable vertex & fragment shaders!
//

typedef vmath::vec3<float> vec3f;
typedef vmath::vec2<float> vec2f;
typedef vmath::vec4<float> vec4f;
typedef vmath::mat3<float> mat3f;
typedef vmath::mat4<float> mat4f;

class Rasterizer
{

	/* Structs and member variables */
private:
	int m_MinX;
	int m_MaxX;
	int m_MinY;
	int m_MaxY;

	// Vector and matrix helpers
	typedef vmath::vec3<float> vec3f;
	typedef vmath::vec2<float> vec2f;
	typedef vmath::vec4<float> vec4f;
	typedef vmath::mat4<float> mat4f;

	// Fragment and vertex shaders should be defined and set by user.
	// C++ magic but basically we store these as function pointers so compiler can inline them
	void (Rasterizer::* m_FragmentShaderFunc)(const Vertex& v0, const Vertex& v1, const Vertex& v2) const;
	void (*m_VertexShaderFunc)(void* in[], Vertex* out);

	int m_AttribCount;
	int m_VertexVarCount;

	struct VertexIndexData {
		unsigned vertexIndex;
		unsigned normalIndex;
		unsigned texcoordIndex;

		struct VertexIndexDataCompare {
			bool operator () (const VertexIndexData& v1, const VertexIndexData& v2) const
			{
				if (v1.vertexIndex < v2.vertexIndex) return true;
				else if (v1.vertexIndex > v2.vertexIndex) return false;
				else {
					if (v1.normalIndex < v2.normalIndex) return true;
					else if (v1.normalIndex > v2.normalIndex) return false;
					else {
						if (v1.texcoordIndex < v2.texcoordIndex) return true;
						else return false;
					}
				}
			}
		};
	};

	// Used in transformVertices to perform a viewport transform
	struct Viewport {
		int x, y, width, height;
		float px, py, ox, oy;
	} m_Viewport;

	float m_NearPlane = 0;
	float m_FarPlane = 0;

	// Used to keep track of per vertex attributes
	struct Attribute {
		const void* buffer;
		int stride;
	} m_Attributes[8] = { 0 };

	struct ClipMask {
		enum Enum {
			PosX = 0x01,
			NegX = 0x02,
			PosY = 0x04,
			NegY = 0x08,
			PosZ = 0x10,
			NegZ = 0x20
		};
	};

	// Used in processVertices to keep track of currently seen vertices
	struct VertexCache
	{
		static const int VertexCacheSize = 16;

		int inputIndex[VertexCacheSize];
		int outputIndex[VertexCacheSize];

		void clear()
		{
			for (size_t i = 0; i < VertexCacheSize; i++)
				inputIndex[i] = -1;
		}

		void set(int inIndex, int outIndex)
		{
			int cacheIndex = inIndex % VertexCacheSize;
			inputIndex[cacheIndex] = inIndex;
			outputIndex[cacheIndex] = outIndex;
		}

		int lookup(int inIndex) const
		{
			int cacheIndex = inIndex % VertexCacheSize;
			if (inputIndex[cacheIndex] == inIndex)
				return outputIndex[cacheIndex];
			else
				return -1;
		}

	};

	// Outputs from vertex shader
	mutable std::vector<Vertex> m_VerticesOut;
	mutable std::vector<int> m_VertexIndicesOut;

	float* m_DepthBuffer;

	/* Structs and public functions */
public:

	/*
	*  Helper structs for passing in vertex data for render
	*/
	struct VertexData {
		float x, y, z, w;
		float r, g, b;
	};

	struct VertexArrayData {
		vmath::vec3<float> vertex;
		vmath::vec3<float> normal;
		vmath::vec2<float> texcoord;
	};

	/* Rasterizer functions */

	Rasterizer(const int windowHeight, const int windowWidth);

	// Set the scissor rectangle.
	void setScissorRect(int x, int y, int width, int height);

	// Set viewport
	void setViewport(int xmin, int ymin, int xmax, int ymax);

	// Define near and far plane
	void setDepthRange(float near, float far);

	// Loads a .obj into struct VertexArrayData
	bool loadModelIntoVertexArray(const char* modelPath, char* texturePath, mat4f modelTransforms, std::vector<Rasterizer::VertexArrayData>& vertexDataRef, std::vector<int>& indexDataRef);

	/* Note: Template functions need to be defined in the header file */

	// Set the pixel shader using c++ template magic.
	template <class FragmentShader>
	void setFragmentShader()
	{
		//this->m_FragmentShaderFunc = &Rasterizer::drawTriangleBlockTemplate<FragmentShader>;
		this->m_FragmentShaderFunc = &Rasterizer::drawSingleTriangle2<FragmentShader>;
	};

	// Set the vertex shader using c++ template magic
	template <class VertexShader>
	void setVertexShader()
	{
		this->m_VertexVarCount = VertexShader::varCount;
		this->m_AttribCount = VertexShader::attribCount;
		this->m_VertexShaderFunc = &VertexShader::processVertex;
	}

	// Specify structure of vertex array
	void setVertexAttribPointer(int index, int stride, const void* buffer);

	// Draw a list of triangles
	void drawTriangles(int count, int* indices);

	/* Main rasterization functions. */
private:

	///////////////////////////////////////////////////////////////////

	// Process list of vertices and transform to view space
	void processVertices(int count, int* indices);

	// Run vertex shader on a single vertex
	void processSingleVertex(void* in[], Vertex* out);

	// clip vertices outside view frustum
	void clipVertices();

	// Perspective divide and transform to screen space
	void transformVertices() const;

	// Cull back facing triangles
	void cullBackfacingTriangles() const;

	void sortTriangles() const;

	// Compute fragments (in this case pixels) and run a fragment shader
	void processFragments() const;

	// Draws a triangle on screen by running fragment shader.
	void drawSingleTriangle(const Vertex& v0, const Vertex& v1, const Vertex& v2) const;

	template <class FragmentShader>
	void drawSingleTriangle2(const Vertex& v0, const Vertex& v1, const Vertex& v2) const
	{
		// Compute triangle equations.
		TriangleEquations teqn(v0, v1, v2, FragmentShader::varCount);

		// Check if triangle is backfacing.
		if (teqn.area <= 0)
			return;

		// Compute triangle bounding box.
		int minX = (int)std::min(std::min(v0.x, v1.x), v2.x);
		int maxX = (int)std::max(std::max(v0.x, v1.x), v2.x);
		int minY = (int)std::min(std::min(v0.y, v1.y), v2.y);
		int maxY = (int)std::max(std::max(v0.y, v1.y), v2.y);

		// Clip to window dimensions
		minX = std::max(minX, m_MinX);
		maxX = std::min(maxX, m_MaxX);
		minY = std::max(minY, m_MinY);
		maxY = std::min(maxY, m_MaxY);

		// Add 0.5 to sample at pixel centers.
		for (float y = minY + 0.5f; y <= maxY + 0.5f; y += 1.0f)
		for (float x = minX + 0.5f; x <= maxX + 0.5f; x += 1.0f)
		{
			if (teqn.e0.test(x, y) && teqn.e1.test(x, y) && teqn.e2.test(x, y))
			{
				float currentDepthAtPixel = m_DepthBuffer[(int) ((y-0.5f) * m_MaxX + (x-0.5f))]; // row * rows + col
				FragmentData fragmentData = FragmentData(teqn, x, y, FragmentShader::varCount, FragmentShader::interpolateZ, FragmentShader::interpolateW);

				// Only draw fragment if its depth is above (in front) what is currently stored.
				if (fragmentData.z < currentDepthAtPixel)
				{
					FragmentShader::processFragment(fragmentData);

					// Update current stored depth at this pixel
					this->m_DepthBuffer[(int) ((y - 0.5f) * m_MaxX + (x - 0.5f))] = fragmentData.z;
				}
			}
		}
	}
};
///////////////////////////////////////////////////////////////////