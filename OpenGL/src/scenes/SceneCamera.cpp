#include "SceneCamera.h"

#include <algorithm>

namespace scene {

	SceneCamera::SceneCamera(GLFWwindow* window, int windowWidth, int windowHeight) :
		m_Window(window), m_ASPECT_RATIO((float)windowWidth / (float)windowHeight),
		m_CameraSpeed(5.0f), m_Pitch(0.0f), m_Yaw(0.0f)
	{
		/* Take input from mouse */
		glfwSetCursorPosCallback(m_Window, mouseCallback);

		cube = std::make_unique<Cube>(CRATE_TEXTURE_PATH);

		/* Set the camera and target positions */
		m_Eye = glm::vec3(0.0f, 0.0f, 10.0f);
		m_CameraFront = glm::vec3(0.0f, 0.0f, -1.f);
		m_Center = glm::vec3(0.0f, 0.0f, 0.0f);
		/* Set the up world vector */
		m_WorldUp = glm::vec3(0.0f, 1.0f, 0.0f);

		/* Enable blending */
		GLCheckErrorCall(glEnable(GL_BLEND));
		/* Transparency implementation */
		GLCheckErrorCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

		/* Enable depth testing */
		GLCheckErrorCall(glEnable(GL_DEPTH_TEST));
		/* Choose background color */
		GLCheckErrorCall(glClearColor(0.25f, 0.35f, 0.30f, 1.00f));
	}

	SceneCamera::~SceneCamera()
	{
		GLCheckErrorCall(glDisable(GL_DEPTH_TEST));
		glfwSetCursorPosCallback(m_Window, NULL);
	}

	std::string SceneCamera::GetName() const { return name; }

	void SceneCamera::OnUpdate(float deltaTime)
	{
		this->processUserInput(deltaTime);
		m_Center = m_Eye + m_CameraFront;

		EulerAngles* offset = (EulerAngles*)glfwGetWindowUserPointer(m_Window);
		if (offset) {
			m_Pitch = std::clamp(m_Pitch + offset->pitch, -80.0f, 80.0f);
			m_Yaw += offset->yaw;

			m_CameraFront.x = -cos(glm::radians(m_Pitch)) * sin(glm::radians(m_Yaw));
			m_CameraFront.y =  sin(glm::radians(m_Pitch));
			m_CameraFront.z = -cos(glm::radians(m_Pitch)) * cos(glm::radians(m_Yaw));
			/* No need to normalize, since the magnitude is already 1 by construction */

			/* Reset offset to zero */
			offset->pitch = 0.0f;
			offset->yaw = 0.0f;
			offset->roll = 0.0f;
		}
	}

	void SceneCamera::OnRender()
	{
		GLCheckErrorCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

		/* See math folder for explanation */
		m_View = glm::lookAt(m_Eye, m_Center, m_WorldUp);

		/* N.B. Depth testing does not work if zNear is set to 0.0f ! */
		m_Proj = glm::perspective<float>(glm::radians(60.0f), m_ASPECT_RATIO, 0.1f, 100.0f);

		for (int i = 0; i < TOTAL_CUBES; ++i) {
			m_Model = glm::translate(glm::mat4(1.0f), m_CubesPositions[i]);

			m_MVP = m_Proj * m_View * m_Model;
			cube->SetMVP(m_MVP);
			cube->Draw();
		}
	}

	void SceneCamera::OnImGuiRender()
	{
		ImGui::Begin("Scene Camera");
		ImGui::SliderFloat("Camera Speed", &m_CameraSpeed, 2.5f, 10.0f);
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::End();
	}

	void SceneCamera::mouseCallback(GLFWwindow* window, double xpos, double ypos)
	{
		float fxpos = (float)xpos;
		float fypos = (float)ypos;
		static float lastMouseX = fxpos;
		static float lastMouseY = fypos;

		float xoffset =  fxpos - lastMouseX;
		float yoffset = -fypos + lastMouseY; /* Y screen coordinates returned by glfw are reversed */
		lastMouseX = fxpos;
		lastMouseY = fypos;

		static const float hSensitivity = 0.20f;
		static const float vSensitivity = 0.15f;
		xoffset *= hSensitivity;
		yoffset *= vSensitivity;

		static EulerAngles offset;
		offset.pitch = yoffset;
		offset.yaw = -xoffset;
		offset.roll = 0.0f;
		glfwSetWindowUserPointer(window, &offset);
	}

	void SceneCamera::processUserInput(float deltaTime)
	{
		float deltaSpace = m_CameraSpeed * deltaTime;
		if (GLFW_PRESS == glfwGetKey(m_Window, GLFW_KEY_W)) {
			m_Eye += deltaSpace * m_CameraFront;
		}
		if (GLFW_PRESS == glfwGetKey(m_Window, GLFW_KEY_S)) {
			m_Eye -= deltaSpace * m_CameraFront;
		}
		if (GLFW_PRESS == glfwGetKey(m_Window, GLFW_KEY_D)) {
			m_Eye += deltaSpace * glm::normalize(glm::cross(m_CameraFront, m_WorldUp));
		}
		if (GLFW_PRESS == glfwGetKey(m_Window, GLFW_KEY_A)) {
			m_Eye -= deltaSpace * glm::normalize(glm::cross(m_CameraFront, m_WorldUp));
		}
	}

}
