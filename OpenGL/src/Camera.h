#pragma once

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <GLFW/glfw3.h>

class Camera
{
public:
	enum MovementDirection {
		FORWARD,
		BACKWARD,
		LEFT,
		RIGHT
	};

public:
	Camera(glm::vec3 position, float aspectRatio);

	glm::mat4 GetViewMatrix();
	glm::mat4 GetPerspectiveProjMatrix();

	void SetAspectRatio(float aspectRatio);
	void SetFOV(float fov);
	void SetCameraSpeed(float speed);

	void ProcessKeyboard(MovementDirection direction, float deltaTime);
	void ProcessMouseMovement(float xoffset, float yoffset);
	void ProcessMouseScroll(float yoffset);

private:
	static constexpr float m_MouseHorizontalSensitivity = 0.20f;
	static constexpr float m_MouseVerticalSensitivity = 0.15f;
	static constexpr float m_MouseScrollSensitivity = 1.5f;

	float m_AspectRatio;
	float m_FOV;
	float m_CameraSpeed;

	void UpdateCameraVectors();

	glm::vec3 m_Eye;
	glm::vec3 m_CameraFront;
	glm::vec3 m_CameraRight;
	glm::vec3 m_WorldUp;

	float m_Pitch;
	float m_Yaw;
};
