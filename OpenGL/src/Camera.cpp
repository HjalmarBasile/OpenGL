#include "Camera.h"

#include <algorithm>

Camera::Camera(glm::vec3 position, float aspectRatio, bool constrainToGround) :
	m_AspectRatio(aspectRatio), m_ConstrainToGround(constrainToGround),
	m_FOV(FOV_DEFAULT), m_CameraSpeed(CAMERA_SPEED_DEFAULT),
	m_Pitch(PITCH_DEFAULT), m_Yaw(YAW_DEFAULT)
{
	/* Set the camera position */
	m_Eye = position;
	if (m_ConstrainToGround) {
		m_Eye.y = 0.0f;
	}

	/* Set the up world vector */
	m_WorldUp = WORLD_UP_DEFAULT;

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

void Camera::SetConstrainToGround(bool constrainToGround)
{
	this->m_ConstrainToGround = constrainToGround;
}

void Camera::SetFOV(float fov)
{
	this->m_FOV = fov;
}

void Camera::SetCameraSpeed(float speed)
{
	this->m_CameraSpeed = speed;
}

void Camera::ResetToDefaults()
{
	this->m_ConstrainToGround = CONSTRAIN_TO_GROUND_DEFAULT;
	this->m_FOV = FOV_DEFAULT;
	this->m_CameraSpeed = CAMERA_SPEED_DEFAULT;
	this->m_Pitch = PITCH_DEFAULT;
	this->m_Yaw = YAW_DEFAULT;

	this->m_Eye = EYE_DEFAULT;
	this->m_WorldUp = WORLD_UP_DEFAULT;
	UpdateCameraVectors();
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

	if (m_ConstrainToGround) {
		m_Eye.y = 0.0f;
	}
}

void Camera::ProcessMouseMovement(float xoffset, float yoffset)
{
	xoffset *= MOUSE_HORIZONTAL_SENSITIVITY;
	yoffset *= MOUSE_VERTICAL_SENSITIVITY;

	/* See math notes about Euler angles for explanation */
	float pitchOffset = yoffset;
	float yawOffset = -xoffset;

	m_Pitch = std::clamp(m_Pitch + pitchOffset, -80.0f, 80.0f);
	m_Yaw += yawOffset;

	UpdateCameraVectors();
}

void Camera::ProcessMouseScroll(float yoffset)
{
	m_FOV = std::clamp(m_FOV - (float)yoffset * MOUSE_SCROLL_SENSITIVITY, 5.0f, 55.0f);
}

void Camera::UpdateCameraVectors()
{
	/* See math notes about Euler angles for explanation */
	m_CameraFront.x = -cos(glm::radians(m_Pitch)) * sin(glm::radians(m_Yaw));
	m_CameraFront.y = sin(glm::radians(m_Pitch));
	m_CameraFront.z = -cos(glm::radians(m_Pitch)) * cos(glm::radians(m_Yaw));
	/* No need to normalize, since the magnitude is already 1 by construction */

	m_CameraRight = glm::normalize(glm::cross(m_CameraFront, m_WorldUp));
}
