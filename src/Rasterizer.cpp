// See header file
#pragma once

#include "Rasterizer.hpp"
#include "FragmentShaderBase.hpp"
#include "VertexShaderBase.hpp"

Rasterizer::Rasterizer(const int windowWidth, const int windowHeight)
{
	this->m_VertexVarCount = 0;
	this->m_AttribCount = 0;
	this->m_Viewport = { 0 };
	this->m_FragmentShaderFunc = 0;
	this->m_VertexShaderFunc = 0;

	// Initialize depth buffer
	this->m_DepthBuffer = new float[windowWidth * windowHeight];
}

void Rasterizer::setScissorRect(int x, int y, int width, int height)
{
	this->m_MinX = x;
	this->m_MinY = y;
	this->m_MaxX = x + width;
	this->m_MaxY = y + height;
}

void Rasterizer::setViewport(int xmin, int ymin, int xmax, int ymax)
{
	this->m_Viewport.x = xmin;
	this->m_Viewport.y = ymin;
	this->m_Viewport.width = xmax;
	this->m_Viewport.height = ymax;

	this->m_Viewport.px = xmax / 2.0f;
	this->m_Viewport.py = ymax / 2.0f;
	this->m_Viewport.ox = (xmin + m_Viewport.px);
	this->m_Viewport.oy = (ymin + m_Viewport.py);
}

void Rasterizer::setDepthRange(float near, float far)
{
	this->m_NearPlane = near;
	this->m_FarPlane = far;
}

bool Rasterizer::loadModelIntoVertexArray(const char* modelPath, char* texturePath, std::vector<VertexArrayData>& vertexArrayDataRef, std::vector<int>& indexDataRef)
{
	std::vector<VertexIndexData> typedef Face;

	// Intermediary storage of data read from file
	std::vector<vec3f> vertices;
	std::vector<vec3f> normals;
	std::vector<vec2f> texcoords;
	std::vector<Face> faces;

	vertices.clear();
	normals.clear();
	texcoords.clear();
	faces.clear();

	vertices.push_back(vec3f(0.0f));
	normals.push_back(vec3f(0.0f));
	texcoords.push_back(vec2f(0.0));

	std::ifstream input(modelPath, std::ios::in);

	// Iterate over file
	while (input)
	{
		// Fetch next line in file
		std::string line;
		std::getline(input, line);

		std::istringstream iss;
		iss.str(line);

		std::string cmd;
		iss >> cmd;

		if (cmd[0] == '#')
		{
			continue;
		}
		else if (cmd == "v") // This line contains a vertex
		{
			vec3f vertex;
			iss >> vertex.x >> vertex.y >> vertex.z;

			vertices.push_back(vertex);
		}
		else if (cmd == "vn") // This line contains a normal
		{
			vec3f normal;
			iss >> normal.x >> normal.y >> normal.z;

			normals.push_back(normal);
		}
		else if (cmd == "vt") // This line contains a texture coordinate
		{
			vec2f texcoord;
			iss >> texcoord.x >> texcoord.y;

			texcoords.push_back(texcoord);
		}
		else if (cmd == "f") // This line contains face information
		{
			faces.push_back(Face());

			// Get reference of last added face
			Face& face = faces.back();

			// Iterate over the face
			while (iss)
			{
				std::string word;
				iss >> word;

				if (word == "")
					continue;

				VertexIndexData vr;

				std::string v;
				std::string t;
				std::string n;

				std::string* tmp[3] = { &v, &t, &n };

				int tidx = 0;
				std::string* target = tmp[0];

				for (size_t i = 0; i < word.size(); ++i) {
					if (word[i] == '/') target = tmp[++tidx];
					else *target += word[i];
				}

				// input string is supposed to be in the form <vertex_index>/<texture_index>/<normal_index>, 
				// for example: f 1/1/3 5/9/3 6/10/3 2/11/3
				std::istringstream iss1(v);
				vr.vertexIndex = 0;
				iss1 >> vr.vertexIndex;

				std::istringstream iss2(t);
				vr.texcoordIndex = 0;
				iss2 >> vr.texcoordIndex;

				std::istringstream iss3(n);
				vr.normalIndex = 0;
				iss3 >> vr.normalIndex;

				face.push_back(vr);
			}

		}
	}

	vertexArrayDataRef.clear();
	indexDataRef.clear();

	// Temporary map of currently seen vertices & indices
	std::map<VertexIndexData, int, VertexIndexData::VertexIndexDataCompare> vertexIndexMap;

	// Lambda function that returns vertex index
	auto addVertex = [&vertexArrayDataRef, &vertexIndexMap, &vertices, &normals, &texcoords](const VertexIndexData& vertexRef)
	{
		// Assume vertex doesn't exist already
		int index = (int)vertexIndexMap.size();

		// check if this vertex already exists
		auto it = vertexIndexMap.find(vertexRef);

		if (it != vertexIndexMap.end())
		{
			// It did, return its index
			index = it->second;
			return index;
		}

		vertexIndexMap.insert(std::make_pair(vertexRef, index));

		// It didn't exist so insert new vertex data
		Rasterizer::VertexArrayData vertexArrayData;
		vertexArrayData.vertex = vertices[vertexRef.vertexIndex];
		vertexArrayData.normal = normals[vertexRef.normalIndex];
		vertexArrayData.texcoord = texcoords[vertexRef.texcoordIndex];

		vertexArrayDataRef.push_back(vertexArrayData);
		return index;
	};

	// Lambda function to make a triangle face (3 vertices)
	auto addFace = [](const int index1, const int index2, const int index3, std::vector<int>& indices)
	{
		indices.push_back(index1);
		indices.push_back(index2);
		indices.push_back(index3);
	};

	for (size_t i = 0; i < faces.size(); ++i) {

		unsigned i1 = addVertex(faces[i][0]);

		// make a triangle fan if there are more than 3 vertices
		for (size_t j = 2; j < faces[i].size(); ++j) {

			unsigned i2 = addVertex(faces[i][j - 1]);
			unsigned i3 = addVertex(faces[i][j]);

			// Add indices
			addFace(i1, i2, i3, indexDataRef);
		}
	}

	return !vertexArrayDataRef.empty();
}


void Rasterizer::setVertexAttribPointer(int index, int stride, const void* buffer)
{
	m_Attributes[index].buffer = buffer;
	m_Attributes[index].stride = stride;
}

void Rasterizer::sortTriangles() const
{
	size_t num_vertices = m_VerticesOut.size();

	// Sort vertices on
	for (int i = 0; i < num_vertices; i++)
	{
		int current_index = m_VertexIndicesOut[i];
		for (int j = 0; j < num_vertices - i - 1; j++)
		{
			if (m_VerticesOut[j].z > m_VerticesOut[j + 1].z)
			{
				//std::swap(m_VerticesOut[j], m_VerticesOut[j + 1]);
			}
		}
	}

}

void Rasterizer::cullBackfacingTriangles() const
{
	for (size_t i = 0; i + 3 <= m_VertexIndicesOut.size(); i += 3)
	{
		if (m_VertexIndicesOut[i] == -1)
			continue;

		// Load all three triangle vertices
		Vertex& v0 = m_VerticesOut[m_VertexIndicesOut[i]];
		Vertex& v1 = m_VerticesOut[m_VertexIndicesOut[i + 1]];
		Vertex& v2 = m_VerticesOut[m_VertexIndicesOut[i + 2]];

		bool backfacing = ((v0.x - v1.x) * (v2.y - v1.y) - (v2.x - v1.x) * (v0.y - v1.y)) < 0;

		// Cull
		if (backfacing)
		{
			m_VertexIndicesOut[i] = m_VertexIndicesOut[i + 1] = m_VertexIndicesOut[i + 2] = -1;
		}
		else
		{
			std::swap(m_VertexIndicesOut[i], m_VertexIndicesOut[i + 2]);
		}
	}
}

void Rasterizer::clipVertices()
{
	std::vector<int> clipMasks;
	clipMasks.resize(m_VerticesOut.size());

	// Create triangle clip masks for all vertices
	for (size_t i = 0; i < m_VerticesOut.size(); i++)
	{
		int mask = 0;
		Vertex vertex = m_VerticesOut[i];

		if (vertex.w - vertex.x < 0) mask |= ClipMask::PosX;
		if (vertex.x + vertex.w < 0) mask |= ClipMask::NegX;
		if (vertex.w - vertex.y < 0) mask |= ClipMask::PosY;
		if (vertex.y + vertex.w < 0) mask |= ClipMask::NegY;
		if (vertex.w - vertex.z < 0) mask |= ClipMask::PosZ;
		if (vertex.z + vertex.w < 0) mask |= ClipMask::NegZ;

		clipMasks[i] = mask;
	}

	// Clipping can modify index buffers so we extract current value first
	size_t n = m_VertexIndicesOut.size();

	for (size_t i = 0; i < n; i += 3)
	{
		int i0 = m_VertexIndicesOut[i];
		int i1 = m_VertexIndicesOut[i + 1];
		int i2 = m_VertexIndicesOut[i + 2];

		int clipMask = clipMasks[i0] | clipMasks[i1] | clipMasks[i2];

		ViewFrustumClipper polygonClipper = ViewFrustumClipper(&m_VerticesOut, i0, i1, i2, m_VertexVarCount);

		// Clip against planes using the mask
		if (clipMask & ClipMask::PosX) polygonClipper.clipToPlane(-1, 0, 0, 1);
		if (clipMask & ClipMask::NegX) polygonClipper.clipToPlane(1, 0, 0, 1);
		if (clipMask & ClipMask::PosY) polygonClipper.clipToPlane(0, -1, 0, 1);
		if (clipMask & ClipMask::NegY) polygonClipper.clipToPlane(0, 1, 0, 1);
		if (clipMask & ClipMask::PosZ) polygonClipper.clipToPlane(0, 0, -1, 1);
		if (clipMask & ClipMask::NegZ) polygonClipper.clipToPlane(0, 0, 1, 1);

		// Triangle fully outside clipping plane, discard
		if (polygonClipper.fullyClipped())
		{
			m_VertexIndicesOut[i] = -1;
			m_VertexIndicesOut[i + 1] = -1;
			m_VertexIndicesOut[i + 2] = -1;
			continue;
		}

		// Get list of clipped indices
		std::vector<int>& indices = polygonClipper.getIndices();

		m_VertexIndicesOut[i] = indices[0];
		m_VertexIndicesOut[i + 1] = indices[1];
		m_VertexIndicesOut[i + 2] = indices[2];

		for (size_t index = 3; index < indices.size(); ++index) {
			m_VertexIndicesOut.push_back(indices[0]);
			m_VertexIndicesOut.push_back(indices[index - 1]);
			m_VertexIndicesOut.push_back(indices[index]);
		}
	}
}

void Rasterizer::processVertices(int count, int* indices)
{
	VertexCache vertexCache;
	vertexCache.clear();

	for (int i = 0; i < count; i++)
	{
		int index = indices[i];
		int outputIndex = vertexCache.lookup(index);

		if (outputIndex != -1)
		{
			m_VertexIndicesOut.push_back(outputIndex);
		}
		else
		{
			// max 16 per vertex attributes
			void* vIn[16];

			for (int j = 0; j < m_AttribCount; ++j)
			{
				int offset = m_Attributes[j].stride * index;
				vIn[j] = (void*)((char*)m_Attributes[j].buffer + (int)offset);
			}
			outputIndex = m_VerticesOut.size();
			this->m_VertexIndicesOut.push_back(outputIndex);
			this->m_VerticesOut.resize(m_VerticesOut.size() + 1);

			Vertex& vOut = m_VerticesOut.back();

			// Run vertex shader
			processSingleVertex(vIn, &vOut);

			vertexCache.set(index, outputIndex);
		}
	}
}

void Rasterizer::processSingleVertex(void* in[], Vertex* out)
{
	(*m_VertexShaderFunc)(in, out);
}

void Rasterizer::processFragments() const
{
	for (size_t i = 0; i + 3 <= m_VertexIndicesOut.size(); i += 3) {
		if (m_VertexIndicesOut[i] == -1)
			continue;

		// Fragment shader
		drawSingleTriangle(m_VerticesOut[m_VertexIndicesOut[i]], m_VerticesOut[m_VertexIndicesOut[i + 1]], m_VerticesOut[m_VertexIndicesOut[i + 2]]);
	}
}

void Rasterizer::drawSingleTriangle(const Vertex& v0, const Vertex& v1, const Vertex& v2) const
{
	(this->*m_FragmentShaderFunc)(v0, v1, v2);
}

void Rasterizer::transformVertices() const
{
	std::vector<bool> alreadyProcessed;
	alreadyProcessed.resize(m_VerticesOut.size());

	for (size_t i = 0; i < m_VertexIndicesOut.size(); i++)
	{
		int index = m_VertexIndicesOut[i];

		if (index == -1)
			continue;

		// Index buffer can contain the same vertex index multiple times but we should only do a viewport transform for it once
		if (alreadyProcessed[index])
			continue;


		Vertex& vOut = m_VerticesOut[index];

		//// Perspective divide
		float invW = 1.0f / vOut.w;
		vOut.x *= invW;
		vOut.y *= invW;
		vOut.z *= invW;

		// Viewport transform
		vOut.x = (m_Viewport.px * vOut.x + m_Viewport.ox);
		vOut.y = (m_Viewport.py * -vOut.y + m_Viewport.oy);
		vOut.z = 0.5f * (m_FarPlane - m_NearPlane) * vOut.z + 0.5f * (m_NearPlane + m_FarPlane);

		alreadyProcessed[index] = true;
	}
}

void Rasterizer::drawTriangles(int count, int* indices)
{
	m_VerticesOut.clear();
	m_VertexIndicesOut.clear();

	for (int i = 0; i < m_MaxX * m_MaxY; i++)
		this->m_DepthBuffer[i] = INT_MAX;

	// Process each vertex in a vertex shader e.g to transform 
	processVertices(count, indices);

	// Cull/clip vertices outside of view frustum and build new index list
	clipVertices();

	// Transform vertices into screen space
	transformVertices();

	// Cull backfacing triangles
	cullBackfacingTriangles();

	// Find which pixels the triangles cover and invoke fragment shaders
	processFragments();
}