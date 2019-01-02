#include "SceneCamera.h"

namespace scene {

	std::unique_ptr<Camera> SceneCamera::s_Camera = std::make_unique<Camera>(glm::vec3(0.0f, 0.0f, 10.0f), 720.0f / 540.0f);

	SceneCamera::SceneCamera(GLFWwindow* window, int windowWidth, int windowHeight) :
		m_CameraSpeed(5.0f), m_Window(window)
	{
		s_Camera->SetAspectRatio((float)windowWidth / (float)windowHeight);
		s_Camera->SetCameraSpeed(m_CameraSpeed);

		m_Cube = std::make_unique<Cube>(CRATE_TEXTURE_PATH);

		/* Take input from mouse */
		glfwSetCursorPosCallback(m_Window, mouseCallback);
		glfwSetScrollCallback(m_Window, scrollCallback);

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
		s_Camera->ResetToDefaults();

		glfwSetCursorPosCallback(m_Window, NULL);
		glfwSetScrollCallback(m_Window, NULL);

		GLCheckErrorCall(glDisable(GL_DEPTH_TEST));
	}

	std::string SceneCamera::GetName() const { return name; }

	void SceneCamera::OnUpdate(float deltaTime)
	{
		s_Camera->SetCameraSpeed(m_CameraSpeed);
		processUserInput(deltaTime);
	}

	void SceneCamera::OnRender()
	{
		GLCheckErrorCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

		m_View = s_Camera->GetViewMatrix();
		m_Proj = s_Camera->GetPerspectiveProjMatrix();

		for (int i = 0; i < TOTAL_CUBES; ++i) {
			m_Model = glm::translate(glm::mat4(1.0f), m_CubesPositions[i]);

			m_MVP = m_Proj * m_View * m_Model;
			m_Cube->SetMVP(m_MVP);
			m_Cube->Draw();
		}
	}

	void SceneCamera::OnImGuiRender()
	{
		ImGui::Begin("Scene Camera");
		ImGui::SliderFloat("Camera Speed", &m_CameraSpeed, 2.5f, 10.0f);
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::End();
	}

	void SceneCamera::processUserInput(float deltaTime)
	{
		if (GLFW_PRESS == glfwGetKey(m_Window, GLFW_KEY_W)) {
			s_Camera->ProcessKeyboard(Camera::MovementDirection::FORWARD, deltaTime);
		}
		if (GLFW_PRESS == glfwGetKey(m_Window, GLFW_KEY_S)) {
			s_Camera->ProcessKeyboard(Camera::MovementDirection::BACKWARD, deltaTime);
		}
		if (GLFW_PRESS == glfwGetKey(m_Window, GLFW_KEY_D)) {
			s_Camera->ProcessKeyboard(Camera::MovementDirection::RIGHT, deltaTime);
		}
		if (GLFW_PRESS == glfwGetKey(m_Window, GLFW_KEY_A)) {
			s_Camera->ProcessKeyboard(Camera::MovementDirection::LEFT, deltaTime);
		}
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

		s_Camera->ProcessMouseMovement(xoffset, yoffset);
	}

	void SceneCamera::scrollCallback(GLFWwindow* window, double xoffset, double yoffset)
	{
		s_Camera->ProcessMouseScroll((float)yoffset);
	}

}
