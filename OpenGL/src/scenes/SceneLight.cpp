#include "SceneLight.h"

#include <GLFW/glfw3.h>

namespace scene {

	SceneLight::SceneLight(Camera* camera, bool* useMainCamera) :
		p_MainCamera(camera), p_UseMainCamera(useMainCamera),
		m_BackgroundColor(glm::vec3(0.1f, 0.2f, 0.2f)),
		m_LightColor(glm::vec3(1.0f, 1.0f, 1.0f)),
		m_AmbientStrenght(0.8f), m_DiffuseStrenght(1.0f), m_SpecularStrenght(0.5f), m_SpecularShininess(32.0f),
		m_UseGouraudShading(false)
	{
		*p_UseMainCamera = true;
		p_MainCamera->SetConstrainToGround(false);

		m_LampCube = std::make_unique<LampCube>();
		m_LampCube->SetLightColor(m_LightColor);
		m_LampCube->Unbind();

		glm::vec3 objectColor(1.0f, 0.5f, 0.31f);
		m_LightedCube = std::make_shared<LightedCube>(m_BackgroundColor, objectColor, m_LightColor);
		m_LightedCube->Unbind();

		m_GouraudLightedCube = std::make_shared<LightedCube>(m_BackgroundColor, objectColor, m_LightColor,
			VERTEX_GOURAUD_SHADER_PATH, FRAGMENT_GOURAUD_SHADER_PATH);
		m_GouraudLightedCube->Unbind();

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

		p_CurrentLightedCube = nullptr;

		GLCheckErrorCall(glDisable(GL_DEPTH_TEST));
	}

	std::string SceneLight::GetName() const { return name; }

	void SceneLight::OnUpdate(float deltaTime) {}

	void SceneLight::OnRender()
	{
		GLCheckErrorCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

		m_View = p_MainCamera->GetViewMatrix();
		m_Proj = p_MainCamera->GetPerspectiveProjMatrix();

		float currentTime = (float)glfwGetTime();
		float red = 0.5f * sin(currentTime) + 0.5f;
		m_LightColor = glm::vec3(red, 1.0f, 0.4f);

		{
			/* We are moving the lamp in a circle */
			float radius = 3.0f;
			m_LightSourcePosition.x = radius * sin(currentTime);
			m_LightSourcePosition.y = 1.0f;
			m_LightSourcePosition.z = radius * cos(currentTime);

			m_Model = glm::translate(glm::mat4(1.0f), m_LightSourcePosition);
			m_Model = glm::scale(m_Model, glm::vec3(0.2f));
			m_MVP = m_Proj * m_View * m_Model;
			m_LampCube->Bind();
			m_LampCube->SetMVP(m_MVP);
			m_LampCube->SetLightColor(m_LightColor);
			m_LampCube->Draw();
			m_LampCube->Unbind();
		}

		{
			m_Model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
			m_Model = glm::rotate(m_Model, (float)glfwGetTime() * glm::radians(30.0f), glm::vec3(30.0f, -45.0f, 80.0f));
			m_Model = glm::scale(m_Model, glm::vec3(3.0f));
			m_ModelView = m_View * m_Model;
			m_MVP = m_Proj * m_ModelView;

			p_CurrentLightedCube = m_UseGouraudShading ? m_GouraudLightedCube.get() : m_LightedCube.get();
			p_CurrentLightedCube->Bind();
			p_CurrentLightedCube->SetViewMatrix(m_View);
			p_CurrentLightedCube->SetModelViewMatrix(m_ModelView);
			p_CurrentLightedCube->SetMVP(m_MVP);
			p_CurrentLightedCube->SetLightColor(m_LightColor);
			p_CurrentLightedCube->SetLightPosition(m_LightSourcePosition);
			p_CurrentLightedCube->SetAmbientStrenght(m_AmbientStrenght);
			p_CurrentLightedCube->SetDiffuseStrenght(m_DiffuseStrenght);
			p_CurrentLightedCube->SetSpecularStrenght(m_SpecularStrenght);
			p_CurrentLightedCube->SetSpecularShininess(m_SpecularShininess);

			p_CurrentLightedCube->Draw();
			p_CurrentLightedCube->Unbind();
		}
	}

	void SceneLight::OnImGuiRender()
	{
		ImGui::Begin("Scene Light");
		ImGui::SliderFloat("Ambient Strenght", &m_AmbientStrenght, 0.0f, 1.0f);
		ImGui::SliderFloat("Diffuse Strenght", &m_DiffuseStrenght, 0.0f, 1.0f);
		ImGui::SliderFloat("Specular Strenght", &m_SpecularStrenght, 0.0f, 1.0f);
		ImGui::SliderFloat("Specular Shininess", &m_SpecularShininess, 1.0f, 256.0f);
		ImGui::Checkbox("Use Gouraud shading", &m_UseGouraudShading);
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::End();
	}

}
