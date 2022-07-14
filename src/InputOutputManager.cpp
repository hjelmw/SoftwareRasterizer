
#include "InputOutputManager.hpp"

InputOutputManager::InputOutputManager()
{
	SDL_Init(SDL_INIT_VIDEO);

	// Reset all keyboard input
	m_KeyStatus.esc = m_KeyStatus.forward = m_KeyStatus.backward = m_KeyStatus.left = m_KeyStatus.right = m_KeyStatus.up = m_KeyStatus.down = false;

	// Reset mouse input
	m_MouseStatus.rightButton = false; 
	m_MouseStatus.firstTimePressed = true;
	m_MouseStatus.mouseX = m_MouseStatus.mouseY = 0;

	// Reset camera
	m_CameraTransforms.pitch = 0.0f; 
	m_CameraTransforms.yaw = -90.0f; m_CameraTransforms.cameraPosition = vec3f(0.0f, 0.0f, 0.0f);
}

InputOutputManager::~InputOutputManager()
{
	SDL_DestroyWindow(m_RenderWindow);
	SDL_Quit();
}


void InputOutputManager::createWindow(const int windowWidth, const int windowHeight)
{
	m_RenderWindow = SDL_CreateWindow(
		"Software Rasterizer",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		windowWidth,
		windowHeight,
		0
	);
}

SDL_Surface* InputOutputManager::getRenderSurface()
{
	return SDL_GetWindowSurface(m_RenderWindow);
}

void InputOutputManager::processInput()
{
	SDL_Event event;


	while (SDL_PollEvent(&event))
	{
		// Handle mouse input
		if (event.type == SDL_MOUSEBUTTONDOWN)
		{
			switch (event.button.button)
			{
			case SDL_BUTTON_RIGHT:
				m_MouseStatus.rightButton = true;
				break;
			}
		}
		if (event.type == SDL_MOUSEBUTTONUP)
		{
			switch (event.button.button)
			{
			case SDL_BUTTON_RIGHT:
				m_MouseStatus.rightButton = false;
				m_MouseStatus.firstTimePressed = true;
				break;
			}
		}

		// Handle keyboard input
		if (event.type == SDL_KEYDOWN)
		{
			switch (event.key.keysym.sym)
			{
			case SDLK_ESCAPE:
				m_KeyStatus.esc = true;
				break;
			case SDLK_a:
				m_KeyStatus.left = true;
				break;
			case SDLK_d:
				m_KeyStatus.right = true;
				break;
			case SDLK_w:
				m_KeyStatus.forward = true;
				break;
			case SDLK_s:
				m_KeyStatus.backward = true;
				break;
			case SDLK_r:				
				m_KeyStatus.up = true;
				break;
			case SDLK_f:
				m_KeyStatus.down = true;

				break;
			}
		}

		if (event.type == SDL_KEYUP)
		{
			switch (event.key.keysym.sym)
			{
			case SDLK_ESCAPE:
				m_KeyStatus.esc = false;
				break;
			case SDLK_a:
				m_KeyStatus.left = false;
				break;
			case SDLK_d:
				m_KeyStatus.right = false;
				break;
			case SDLK_w:
				m_KeyStatus.forward = false;
				break;
			case SDLK_s:
				m_KeyStatus.backward = false;
				break;
			case SDLK_r:
				m_KeyStatus.up = false;
				break;
			case SDLK_f:
				m_KeyStatus.down = false;
				break;
			}
		}
	}
}


void InputOutputManager::updateCameraTransforms(mat4f& cameraMatrix, float dt)
{
	float sensitivity = 0.45f;

	if (m_MouseStatus.rightButton)
	{

		if (m_MouseStatus.firstTimePressed)
		{
			SDL_GetMouseState(&m_MouseStatus.mouseX, &m_MouseStatus.mouseY);
			m_MouseStatus.firstTimePressed = false;
		}

		int previousMouseX = m_MouseStatus.mouseX;
		int previousMouseY = m_MouseStatus.mouseY;

		int currentMouseX, currentMouseY;
		SDL_GetMouseState(&currentMouseX, &currentMouseY);

		float offsetX = (float) previousMouseX - currentMouseX;
		float offsetY = (float) currentMouseY - previousMouseY;

		offsetX *= (dt + sensitivity);
		offsetY *= (dt + sensitivity);

		m_CameraTransforms.yaw += offsetX;
		m_CameraTransforms.pitch += offsetY;

		if (m_CameraTransforms.pitch > 89.0f)
			m_CameraTransforms.pitch = 89.0f;
		
		if (m_CameraTransforms.pitch < -89.0f)
			m_CameraTransforms.pitch = -89.0f;

		m_MouseStatus.mouseX = currentMouseX;
		m_MouseStatus.mouseY = currentMouseY;

	}

	float yawRadians = m_CameraTransforms.yaw * (M_PI / 180.0f);
	float pitchRadians = m_CameraTransforms.pitch * (M_PI / 180.0f);

	vec3f lookDirection;
	lookDirection.x = -sin(yawRadians) * cos(pitchRadians);
	lookDirection.y = -sin(pitchRadians);
	lookDirection.z = -cos(yawRadians) * cos(pitchRadians);

	vec3f cameraFront = lookDirection;
	vmath::normalize(cameraFront);

	if (m_KeyStatus.forward)
	{
		m_CameraTransforms.cameraPosition += cameraFront * (dt + sensitivity);
	}
	if (m_KeyStatus.backward)
	{
		m_CameraTransforms.cameraPosition -= cameraFront * (dt + sensitivity);
	}
	if (m_KeyStatus.up)
	{
		m_CameraTransforms.cameraPosition.y += 1.0f * (dt + sensitivity);
	}
	if (m_KeyStatus.down)
	{
		m_CameraTransforms.cameraPosition.y -= 1.0f * (dt + sensitivity);
	}
	if (m_KeyStatus.left)
	{
		m_CameraTransforms.cameraPosition -= vmath::normalize(vmath::cross(cameraFront, vec3f(0.0f, 1.0f, 0.0f))) * (dt + sensitivity);
	}
	if (m_KeyStatus.right)
	{
		m_CameraTransforms.cameraPosition += vmath::normalize(vmath::cross(cameraFront, vec3f(0.0f, 1.0f, 0.0f))) * (dt + sensitivity);
	}



	mat4f lookAtMatrix = vmath::lookat_matrix(
		m_CameraTransforms.cameraPosition, // Camera position XYZ
		m_CameraTransforms.cameraPosition + cameraFront, // Look at origin
		vec3f(0.0f, 1.0f, 0.0f)); // Y-axis up

	cameraMatrix = lookAtMatrix;

	// Set translation

}