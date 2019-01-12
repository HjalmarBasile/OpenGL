#include "SceneLight.h"

#include <GLFW/glfw3.h>

namespace scene {

	SceneLight::SceneLight(Camera* camera, bool* useMainCamera) :
		p_MainCamera(camera), p_UseMainCamera(useMainCamera),
		m_BackgroundColor(glm::vec3(0.1f, 0.2f, 0.2f)),
		m_LightColor(glm::vec3(1.0f, 1.0f, 1.0f))
	{
		*p_UseMainCamera = true;
		p_MainCamera->SetConstrainToGround(false);

		m_LampCube = std::make_unique<LampCube>();
		m_LampCube->SetLightColor(m_LightColor);
		m_LampCube->Unbind();

		m_LightedCube = std::make_unique<LightedCube>();
		m_LightedCube->SetObjectColor(glm::vec3(1.0f, 0.5f, 0.31f));
		m_LightedCube->SetAmbientColor(m_BackgroundColor);
		m_LightedCube->SetLightColor(m_LightColor);
		m_LightedCube->SetViewPosition(p_MainCamera->GetPosition());
		m_LightedCube->Unbind();

		/* Enable blending */
		GLCheckErrorCall(glEnable(GL_BLEND));
		/* Transparency implementation */
		GLCheckErrorCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

		/* Enable depth testing */
		GLCheckErrorCall(glEnable(GL_DEPTH_TEST));
		/* Choose background color */
		GLCheckErrorCall(glClearColor(m_BackgroundColor.r, m_BackgroundColor.g, m_BackgroundColor.b, 1.0f));
	}

	SceneLight::~SceneLight()
	{
		*p_UseMainCamera = false;
		p_MainCamera->ResetToDefaults();

		GLCheckErrorCall(glDisable(GL_DEPTH_TEST));
	}

	std::string SceneLight::GetName() const { return name; }

	void SceneLight::OnUpdate(float deltaTime) {}

	void SceneLight::OnRender()
	{
		GLCheckErrorCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

		m_View = p_MainCamera->GetViewMatrix();
		m_Proj = p_MainCamera->GetPerspectiveProjMatrix();

		{
			/* We are moving the lamp in a circle */
			float radius = 3.0f;
			float currentTime = (float)glfwGetTime();
			m_LightSourcePosition.x = radius * sin(currentTime);
			m_LightSourcePosition.y = 2.0f;
			m_LightSourcePosition.z = radius * cos(currentTime);

			m_Model = glm::translate(glm::mat4(1.0f), m_LightSourcePosition);
			m_Model = glm::scale(m_Model, glm::vec3(0.2f));
			m_MVP = m_Proj * m_View * m_Model;
			m_LampCube->Bind();
			m_LampCube->SetMVP(m_MVP);
			m_LampCube->Draw();
			m_LampCube->Unbind();
		}

		{
			m_Model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
			m_MVP = m_Proj * m_View * m_Model;
			m_LightedCube->Bind();
			m_LightedCube->SetModelMatrix(m_Model);
			m_LightedCube->SetMVP(m_MVP);
			m_LightedCube->SetLightPosition(m_LightSourcePosition);
			m_LightedCube->Draw();
			m_LightedCube->Unbind();
		}
	}

	void SceneLight::OnImGuiRender() {}

}
