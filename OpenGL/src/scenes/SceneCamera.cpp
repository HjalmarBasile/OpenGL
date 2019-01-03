#include "SceneCamera.h"

namespace scene {

	SceneCamera::SceneCamera(Camera* camera, bool* useMainCamera) :
		p_MainCamera(camera), p_UseMainCamera(useMainCamera),
		m_CameraSpeed(5.0f)
	{
		*p_UseMainCamera = true;
		p_MainCamera->SetCameraSpeed(m_CameraSpeed);

		m_Cube = std::make_unique<TexturedCube>(CRATE_TEXTURE_PATH);

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
		*p_UseMainCamera = false;
		p_MainCamera->ResetToDefaults();

		GLCheckErrorCall(glDisable(GL_DEPTH_TEST));
	}

	std::string SceneCamera::GetName() const { return name; }

	void SceneCamera::OnUpdate(float deltaTime)
	{
		p_MainCamera->SetCameraSpeed(m_CameraSpeed);
	}

	void SceneCamera::OnRender()
	{
		GLCheckErrorCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

		m_View = p_MainCamera->GetViewMatrix();
		m_Proj = p_MainCamera->GetPerspectiveProjMatrix();

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
		ImGui::Text("Use WASD and mouse to move and look around,\nUse scroll wheel to zoom");
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::End();
	}

}
