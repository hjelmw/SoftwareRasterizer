#pragma once

#include "SDL.h"

#include <algorithm>
#include <vector>

#include "Vertex.hpp"

#include "EdgeEquation.hpp"
#include "ParameterEquation.hpp"
#include "TriangleEquations.hpp"
#include "FragmentData.hpp"
#include "EdgeData.hpp"

#include "FragmentShaderBase.hpp"
#include "VertexShaderBase.hpp"

// Rasterizer main class.
class Rasterizer
{
private:
	SDL_Window* m_window;

	int m_minX;
	int m_maxX;
	int m_minY;
	int m_maxY;

	// Fragment and vertex shaders should be defined and set by user.
	void (Rasterizer::*m_fragmentFunc)(const Vertex& v0, const Vertex& v1, const Vertex& v2) const;
	void (*m_vertexFunc)(Vertex* in[], Vertex *out);

	int m_attribCount;
	int m_vertexVarCount;

	struct Attribute {
		const void* buffer;
		int stride;
	} m_attributes[8];

	mutable std::vector<Vertex> m_verticesOut; // Output from vertex shader
	mutable std::vector<int> m_indicesOut; // 
public:
	struct VertexData {
		float x, y, z, w;
		float r, g, b;
	};


	Rasterizer() {};

	/// Constructor.
	Rasterizer(int width, int height) 
	{
		this->m_minX = 0;
		this->m_minY = 0;
		this->m_maxX = 0;
		this->m_maxY = 0;

		this->m_vertexVarCount = 0;
		this->m_attribCount = 0;

		SDL_Init(SDL_INIT_VIDEO);
		createWindow(width, height, "Software Renderer");
		setScissorRect(0, 0, width, height);
	};

	void createWindow(int width, int height, char* name)
	{
		SDL_Window* window = SDL_CreateWindow(
			name,
			SDL_WINDOWPOS_UNDEFINED,
			SDL_WINDOWPOS_UNDEFINED,
			width,
			height,
			0
		);
	}

	SDL_Surface* getWindowSurface(SDL_Window* window) const
	{
		return SDL_GetWindowSurface(window);
	}

	// Set the scissor rectangle.
	void setScissorRect(int x, int y, int width, int height)
	{
		this->m_minX = x;
		this->m_minY = y;
		this->m_maxX = x + width;
		this->m_maxY = y + height;
	}

	// Set the pixel shader.
	template <class FragmentShader>
	void setFragmentShader()	
	{
		this->m_fragmentFunc = &Rasterizer::drawTriangleBlockTemplate<FragmentShader>;
	}

	// Set the vertex shader
	template <class VertexShader>
	void setVertexShader()
	{
		this->m_vertexVarCount = VertexShader::varCount;
		this->m_attribCount = VertexShader::attribCount;
		this->m_vertexFunc = &VertexShader::processVertex;
	}

	// Specify structure of vertex array
	void setVertexAttribPointer(int index, int stride, const void* buffer)
	{
		m_attributes[index].buffer = buffer;
		m_attributes[index].stride = stride;
	}

	void processVertices(int count, int* indices, std::vector<Vertex> verticesIn, std::vector<int> indicesIn)
	{
		for (int i = 0; i < count; i++)
		{
			int index = indices[i];
			int outputIndex = (int)m_verticesOut.size();

			// max 8 per vertex attributes
			Vertex* vIn[8];

			for (int j = 0; j < m_attribCount; ++j)
			{
				int offset = m_attributes[j].stride * index;
				vIn[j] = (Vertex*)((char*)m_attributes[j].buffer + (int)offset);
			}

			this->m_indicesOut.push_back(outputIndex);
			this->m_verticesOut.resize(m_verticesOut.size() + 1);

			Vertex& vOut = m_verticesOut.back();
			processVertex(vIn, &vOut);
		}
	}

	void processVertex(Vertex* in[], Vertex *out)
	{
		(*m_vertexFunc)(in, out);
	}
	
	// Compute fragments (in this case pixels) and run a fragment shader
	void processFragments(const Vertex* vertices, int* indices, int indexCount) const
	{
		for (size_t i = 0; i + 3 <= indexCount; i += 3) {
			if (indices[i] == -1)
				continue;
			drawTriangle(vertices[indices[i]], vertices[indices[i + 1]], vertices[indices[i + 2]]);
		}
	}

	// Draw a single triangle.
	void drawTriangle(const Vertex& v0, const Vertex& v1, const Vertex& v2) const
	{
		(this->*m_fragmentFunc)(v0, v1, v2);
	}

	void displayFragments() const
	{
		SDL_UpdateWindowSurface(this->m_window);
	}

	// Draw a list of primitives (triangles)
	void drawElements(int count, int *indices)
	{
		m_verticesOut.clear();
		m_indicesOut.clear();
		
		/* 

		// Process the input indices and run a vertex shader on each vertex.
		// Process the vertices to figure out which triangles cover fragments,
		// then run a fragment shader for each. 
		// Finally the manipulated surface is displayed on screen

		*/

		processVertices(count, indices, m_verticesOut, m_indicesOut);
		processFragments(&m_verticesOut[0], &m_indicesOut[0], (int) m_indicesOut.size());
		//displayFragments();
	}

private:
	template <class FragmentShader>
	void drawTriangleBlockTemplate(const Vertex& v0, const Vertex& v1, const Vertex& v2) const
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
			EdgeData e00(teqn, xf, yf);
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
					FragmentShader::template drawBlock<true>(teqn, x, y);
			}
			else if (result == 4)
			{
				// Fully Covered.
				FragmentShader::template drawBlock<false>(teqn, x, y);
			}
			else
			{
				// Partially Covered.
				FragmentShader::template drawBlock<true>(teqn, x, y);
			}
		}
	}
};
