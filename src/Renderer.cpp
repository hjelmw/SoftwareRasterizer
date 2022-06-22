#include "Renderer.hpp"

#include <fstream>
#include <string>
#include <sstream>

#include <iostream>
#include <map>

Renderer::Renderer() 
{
	m_Rasterizer = &Rasterizer();
	SDL_Init(SDL_INIT_VIDEO);
}

Renderer::~Renderer()
{
	delete m_Rasterizer;

	
}


void Renderer::Init(const int screenWidth, const int screenHeight, void* vertexShader, const void* fragmentShader)
{
	SDL_Window* window = SDL_CreateWindow(
		"Software Rasterizer",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		800,
		600,
		0
	);

	m_RenderSurface = SDL_GetWindowSurface(window);
	
}


void Renderer::UpdateTransform(mat4f mvpMatrix)
{

}

void Renderer::LoadModel(const char* modelPath, char* texturePath, std::vector<Rasterizer::VertexArrayData>& vertexArrayDataRef, std::vector<int>& indexDataRef)
{
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
		int index = (int) vertexIndexMap.size();

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
}