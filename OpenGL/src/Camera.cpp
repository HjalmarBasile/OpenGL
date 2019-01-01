#include "Camera.h"

#include <algorithm>

Camera::Camera(glm::vec3 position, float aspectRatio) :
	m_AspectRatio(aspectRatio), m_FOV(45.0f), m_CameraSpeed(5.0f),
	m_Pitch(0.0f), m_Yaw(0.0f)
{
	/* Set the camera position */
	m_Eye = position;
	/* Set the up world vector */
	m_WorldUp = glm::vec3(0.0f, 1.0f, 0.0f);

	/* Update front and right versors */
	UpdateCameraVectors();
}

glm::mat4 Camera::GetViewMatrix()
{
	/* See math folder for explanation */
	return glm::lookAt(m_Eye, m_Eye + m_CameraFront, m_WorldUp);
}

glm::mat4 Camera::GetPerspectiveProjMatrix()
{
	/* N.B. Depth testing does not work if zNear is set to 0.0f ! */
	return glm::perspective<float>(glm::radians(m_FOV), m_AspectRatio, 0.1f, 100.0f);
}

void Camera::SetAspectRatio(float aspectRatio)
{
	this->m_AspectRatio = aspectRatio;
}

void Camera::SetFOV(float fov)
{
	this->m_FOV = fov;
}

void Camera::SetCameraSpeed(float speed)
{
	this->m_CameraSpeed = speed;
}

void Camera::ProcessKeyboard(MovementDirection direction, float deltaTime)
{
	float deltaSpace = m_CameraSpeed * deltaTime;
	if (FORWARD == direction) {
		m_Eye += deltaSpace * m_CameraFront;
	}
	if (BACKWARD == direction) {
		m_Eye -= deltaSpace * m_CameraFront;
	}
	if (RIGHT == direction) {
		m_Eye += deltaSpace * m_CameraRight;
	}
	if (LEFT == direction) {
		m_Eye -= deltaSpace * m_CameraRight;
	}
}

void Camera::ProcessMouseMovement(float xoffset, float yoffset)
{
	xoffset *= m_MouseHorizontalSensitivity;
	yoffset *= m_MouseVerticalSensitivity;

	float pitchOffset = yoffset;
	float yawOffset = -xoffset;

	m_Pitch = std::clamp(m_Pitch + pitchOffset, -80.0f, 80.0f);
	m_Yaw += yawOffset;

	UpdateCameraVectors();
}

void Camera::ProcessMouseScroll(float yoffset)
{
	m_FOV = std::clamp(m_FOV - (float)yoffset * m_MouseScrollSensitivity, 5.0f, 55.0f);
}

void Camera::UpdateCameraVectors()
{
	m_CameraFront.x = -cos(glm::radians(m_Pitch)) * sin(glm::radians(m_Yaw));
	m_CameraFront.y = sin(glm::radians(m_Pitch));
	m_CameraFront.z = -cos(glm::radians(m_Pitch)) * cos(glm::radians(m_Yaw));
	/* No need to normalize, since the magnitude is already 1 by construction */

	m_CameraRight = glm::normalize(glm::cross(m_CameraFront, m_WorldUp));
}
