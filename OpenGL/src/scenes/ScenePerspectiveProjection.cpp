#include "ScenePerspectiveProjection.h"

#include <random>
#include <GLFW/glfw3.h>

namespace scene {

	ScenePerspectiveProjection::ScenePerspectiveProjection(int windowWidth, int windowHeight) :
		m_ASPECT_RATIO((float)windowWidth / (float)windowHeight),
		m_ModelScale(1.0f), m_CameraTranslateZ(10.0f), m_FOV(45.0f), m_ZBufferClearValue(1.0f)
	{
		cube = std::make_unique<Cube>(Cube::ShadingType::TEXTURE, CRATE_TEXTURE_PATH);

		std::random_device rd;
		std::mt19937 rng(rd());
		std::uniform_real_distribution<float> randTranslation(-15.0f, 15.0f);
		std::uniform_real_distribution<float> randRotation(-1.0f, 1.0f);

		for (int i = 0; i < TOTAL_CUBES; ++i) {
			m_CubesPositions[i] = glm::vec3(randTranslation(rng), randTranslation(rng), -abs(randTranslation(rng)));
			m_CubesRotations[i] = glm::vec3(randRotation(rng), randRotation(rng), randRotation(rng));
		}

		/* Enable blending */
		GLCheckErrorCall(glEnable(GL_BLEND));
		/* Transparency implementation */
		GLCheckErrorCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

		/* Enable depth testing */
		GLCheckErrorCall(glEnable(GL_DEPTH_TEST));

		GLCheckErrorCall(glClearColor(0.20f, 0.35f, 0.50f, 1.00f));
	}

	ScenePerspectiveProjection::~ScenePerspectiveProjection()
	{
		GLCheckErrorCall(glDisable(GL_DEPTH_TEST));
	}

	std::string ScenePerspectiveProjection::GetName() const { return name; }

	void ScenePerspectiveProjection::OnUpdate(float deltaTime) {}

	void ScenePerspectiveProjection::OnRender()
	{
		GLCheckErrorCall(glClearDepth(m_ZBufferClearValue));
		GLCheckErrorCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

		/* Moving the camera backwards is equivalent to moving the model forward */
		m_View = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -m_CameraTranslateZ));

		/* N.B. Depth testing does not work if zNear is set to 0.0f ! */
		m_Proj = glm::perspective<float>(glm::radians(m_FOV), m_ASPECT_RATIO, 0.1f, 100.0f);

		for (int i = 0; i < TOTAL_CUBES; ++i) {
			m_Model = glm::translate(glm::mat4(1.0f), m_CubesPositions[i]);
			m_Model = glm::rotate(m_Model, (float)glfwGetTime() * glm::radians((i + 1) * 17.0f), m_CubesRotations[i]);
			m_Model = glm::scale(m_Model, glm::vec3(m_ModelScale, m_ModelScale, m_ModelScale));

			m_MVP = m_Proj * m_View * m_Model;
			cube->SetMVP(m_MVP);
			cube->Draw();
		}
	}

	void ScenePerspectiveProjection::OnImGuiRender()
	{
		ImGui::Begin("Scene Perspective Projection");
		ImGui::SliderFloat("Model Scale", &m_ModelScale, 1.0f, 10.0f);
		ImGui::SliderFloat("Camera Translate Z", &m_CameraTranslateZ, 10.0f, 100.0f);
		ImGui::SliderFloat("Camera FOV", &m_FOV, 45.0f, 145.0f);
		ImGui::SliderFloat("Z-buffer clear value", &m_ZBufferClearValue, 0.0f, 1.0f);
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::End();
	}

}
