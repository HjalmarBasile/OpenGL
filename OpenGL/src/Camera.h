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
	Camera(glm::vec3 position, float aspectRatio, bool constrainToGround = CONSTRAIN_TO_GROUND_DEFAULT);

	glm::vec3 GetPosition();
	glm::mat4 GetViewMatrix();
	glm::mat4 GetPerspectiveProjMatrix();

	void SetAspectRatio(float aspectRatio);
	void SetConstrainToGround(bool constrainToGround);
	void SetFOV(float fov);
	void SetCameraSpeed(float speed);
	void ResetToDefaults();

	void ProcessKeyboard(MovementDirection direction, float deltaTime);
	void ProcessMouseMovement(float xoffset, float yoffset);
	void ProcessMouseScroll(float yoffset);

private:
	static constexpr float MOUSE_HORIZONTAL_SENSITIVITY = 0.20f;
	static constexpr float MOUSE_VERTICAL_SENSITIVITY = 0.15f;
	static constexpr float MOUSE_SCROLL_SENSITIVITY = 1.5f;

	static constexpr float CONSTRAIN_TO_GROUND_DEFAULT = true;
	static constexpr float FOV_DEFAULT = 45.0f;
	static constexpr float CAMERA_SPEED_DEFAULT = 5.0f;
	static constexpr float PITCH_DEFAULT = 0.0f;
	static constexpr float YAW_DEFAULT = 0.0f;

	const glm::vec3 EYE_DEFAULT = glm::vec3(0.0f, 0.0f, 10.0f);
	const glm::vec3 WORLD_UP_DEFAULT = glm::vec3(0.0f, 1.0f, 0.0f);

	float m_AspectRatio;
	bool m_ConstrainToGround;
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
