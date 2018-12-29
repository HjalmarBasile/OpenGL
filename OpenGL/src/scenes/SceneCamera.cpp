#include "SceneCamera.h"

#include <GLFW/glfw3.h>

namespace scene {

	SceneCamera::SceneCamera(int windowWidth, int windowHeight) :
		m_ASPECT_RATIO((float)windowWidth / (float)windowHeight)
	{
		cube = std::make_unique<Cube>(CRATE_TEXTURE_PATH);

		/* Set the camera and target positions */
		m_Eye = glm::vec3(0.0f, 0.0f, 10.0f);
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
	}

	std::string SceneCamera::GetName() const { return name; }

	void SceneCamera::OnUpdate(float deltaTime) {}

	void SceneCamera::OnRender()
	{
		GLCheckErrorCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

		/* We are moving the camera in a circle */
		float radius = 10.0f;
		float currentTime = (float)glfwGetTime();
		m_Eye.x = radius * sin(currentTime);
		m_Eye.z = radius * cos(currentTime);

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
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::End();
	}

}
