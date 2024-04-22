#ifndef GLMIncluded
#define GLMIncluded

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE

#define GLM_ENABLE_EXPERIMENTAL
#pragma warning(push)
#pragma warning(disable : 4201)
#include <glm/gtx/hash.hpp>
#include <glm/gtc/matrix_transform.hpp>

#pragma warning(pop)

#endif 
#include "Vertex.h"
#include "GLFW/glfw3.h"
#include <iostream>

constexpr auto PI = 3.14159265358979323846f;
constexpr auto TO_RADIANS(PI / 180.0f);
struct Camera
{
	Camera() = default;

	Camera(const glm::vec3& _origin, float _fovAngle) :
		m_Origin{ _origin },
		m_FovAngle{ _fovAngle }
	{
	}


	glm::vec3 m_Origin{};
	float m_FovAngle{ 90.f };
	float m_Fov{ tanf((m_FovAngle * TO_RADIANS) / 2.f) };

	glm::vec3 m_Forward{ glm::vec3{0,0,1} };
	glm::vec3 m_Up{ glm::vec3{0,1,0} };
	glm::vec3 m_Right{ glm::vec3{1,0,0} };

	float m_TotalPitch{};
	float m_TotalYaw{};
	float m_SpeedCamera{ 0.0005f };
	float m_SpeedMovement{ 0.01f };

	float m_Near{ 0.1f };
	float m_Far{ 1000.0f };
	
	glm::mat4 m_ViewMatrix{};
	glm::mat4 m_ProjectionMatrix{};

	float m_AspectRatio{};

	void Initialize(float _fovAngle = 90.f, glm::vec3 _origin = { 0.f,0.f,0.f }, float aspectRatio = 1.f)
	{
		m_FovAngle = _fovAngle;
		m_Fov = tanf((m_FovAngle * TO_RADIANS) / 2.f);

		m_Origin = _origin;

		m_AspectRatio = aspectRatio;
		CalculateViewMatrix();
		CalculateProjectionMatrix();
	}

	void CalculateViewMatrix()
	{
		m_ViewMatrix =
		{
			glm::vec4{m_Right,0},
			glm::vec4(m_Up, 0.0f),
			glm::vec4(-m_Forward, 0.0f),
			glm::vec4(m_Origin, 1.0f)
		};

		//m_ViewMatrix = glm::lookAt(m_Origin, m_Origin + m_Forward, m_Up);
	}

	void CalculateProjectionMatrix()
	{
		//TODO W3
		m_ProjectionMatrix = {
		{1 / (m_AspectRatio * m_Fov), 0, 0, 0},
		{0, 1 / m_Fov, 0, 0},
		{0, 0, (m_Far / (m_Far - m_Near)), 1},
		{0, 0, -((m_Far * m_Near) / (m_Far - m_Near)), 0}
		};
		//m_ProjectionMatrix = glm::perspective(glm::radians(m_Fov), m_AspectRatio, m_Near, m_Far);
	}

	ViewProjection GetProjectionViewMatrix() const
	{
		ViewProjection vp;
		vp.proj = m_ProjectionMatrix;
		vp.view = m_ViewMatrix;

		return vp;
	}

	glm::vec3 GetOrigin() const
	{
		return m_Origin;
	}

	void KeyPress(int key, int scancode, int action, int mods, float deltaTime)
	{
		if (key == GLFW_KEY_W && (action == GLFW_REPEAT || action == GLFW_PRESS))
		{
			std::cout << "W";
			m_Origin -= m_Forward * m_SpeedMovement * deltaTime;
		}
		if (key == GLFW_KEY_S && (action == GLFW_REPEAT || action == GLFW_PRESS))
		{
			m_Origin += m_Forward * m_SpeedMovement * deltaTime;
		}
		if (key == GLFW_KEY_A && (action == GLFW_REPEAT || action == GLFW_PRESS))
		{
			std::cout << "A";
			m_Origin += m_Right * m_SpeedMovement * deltaTime;
		}
		if (key == GLFW_KEY_D && (action == GLFW_REPEAT || action == GLFW_PRESS))
		{
			m_Origin -= m_Right * m_SpeedMovement * deltaTime;
		}

		std::cout << m_Origin.z << '\n';

		UpdateCamera();

		//Update Matrices
		CalculateProjectionMatrix();
	}

	void mouseMove(float xpos, float ypos, glm::vec3 dragStart, float deltaTime)
	{
		static float lastMouseX = xpos;
		static float lastMouseY = ypos;

		float deltaX = xpos - lastMouseX;
		float deltaY = ypos - lastMouseY;

		lastMouseX = xpos;
		lastMouseY = ypos;

		// Calculate the change in mouse position
		float dx = deltaX; //- dragStart.x;
		float dy = deltaY; //- dragStart.y;

		// Set sensitivity for mouse movement
		// Update total pitch and yaw based on mouse movement
		m_TotalPitch -= dy * m_SpeedCamera * deltaTime;
		m_TotalYaw -= dx * m_SpeedCamera * deltaTime;

		// Clamp pitch to avoid flipping
		//m_TotalPitch = glm::clamp(m_TotalPitch, -89.0f, 89.0f);

		UpdateCamera();

		//Update Matrices
		CalculateProjectionMatrix();
	}

	void UpdateCamera()
	{
		glm::mat4 yawRotation = glm::rotate(glm::mat4(1.0f), m_TotalYaw, glm::vec3(0, 1, 0));
		glm::mat4 pitchRotation = glm::rotate(glm::mat4(1.0f), m_TotalPitch, glm::vec3(1, 0, 0));

		// Combine the rotation matrices to get the final rotation matrix
		glm::mat4 finalRotation = yawRotation * pitchRotation;

		// Transform the forward vector using the final rotation matrix
		glm::vec4 transformedForward = finalRotation * glm::vec4(0, 0, 1, 0);
		m_Forward = glm::normalize(glm::vec3(transformedForward));

		m_Right = glm::normalize(glm::cross(glm::vec3{0,1,0}, m_Forward));
		m_Up = glm::normalize(glm::cross(m_Forward, m_Right));

		CalculateViewMatrix();
	}
};