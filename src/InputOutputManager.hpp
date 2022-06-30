#pragma once

#include "SDL.h"
#include "VectorMath.hpp"
#include <vector>


//						Input Output Manager
//
//		This class is separate from the Rasterizer and handles 
//			* Window creation
//			* Input output
//			* As well as updating the main render surface
//   
//		It uses mostly SDL so you need to set that up to get everything up and running
// 		SDL is the window manager but it is not used to render anything. 
//		Instead we simply use it to get a handle to an SDL_Surface and rasterize directly into it in software
// 



class Rasterizer;

class InputOutputManager
{
	// Vector and matrix helpers
	typedef vmath::vec3<float> vec3f;
	typedef vmath::vec2<float> vec2f;
	typedef vmath::vec4<float> vec4f;
	typedef vmath::mat3<float> mat3f;
	typedef vmath::mat4<float> mat4f;


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



private:

	SDL_Window* m_RenderWindow;
	SDL_Surface* m_RenderSurface;

	struct KeyStatus
	{
		bool left;	// left arrow
		bool right; // right arrow
		bool forward;
		bool backward;
		bool up;	// up arrow
		bool down;	// down arrow


		bool esc;	// escape button
	} m_KeyStatus;

	struct MouseStatus
	{
		bool rightButton;
		bool firstTimePressed;
		int mouseX;
		int mouseY;
	} m_MouseStatus;

	struct CameraTransforms
	{
		float pitch;
		float yaw;
		vec3f cameraPosition;
	} m_CameraTransforms;

public:
	InputOutputManager();
	~InputOutputManager();

	// Initialize 
	void createWindow(int windowWidth, int windowHeight);
	void processInput();

	SDL_Surface* getRenderSurface();

	void updateCameraTransforms(mat4f &outputTransform, float dt);
};