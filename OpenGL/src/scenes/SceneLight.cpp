#include "SceneLight.h"

namespace scene {

	SceneLight::SceneLight(Camera* camera, bool* useMainCamera) :
		p_MainCamera(camera), p_UseMainCamera(useMainCamera)
	{
		*p_UseMainCamera = true;

		m_LightedCube = std::make_unique<Cube>(Cube::ShadingType::LIGHT, nullptr);
		m_LightedCube->SetObjectColor(glm::vec3(1.0f, 0.5f, 0.31f));
		m_LightedCube->SetLightColor(glm::vec3(1.0f, 1.0f, 1.0f));

		/* Enable blending */
		GLCheckErrorCall(glEnable(GL_BLEND));
		/* Transparency implementation */
		GLCheckErrorCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

		/* Enable depth testing */
		GLCheckErrorCall(glEnable(GL_DEPTH_TEST));
		/* Choose background color */
		GLCheckErrorCall(glClearColor(0.6f, 1.0f, 1.0f, 1.0f));
	}

	SceneLight::~SceneLight()
	{
		*p_UseMainCamera = false;

		GLCheckErrorCall(glDisable(GL_DEPTH_TEST));
	}

	std::string SceneLight::GetName() const { return name; }

	void SceneLight::OnUpdate(float deltaTime) {}

	void SceneLight::OnRender()
	{
		GLCheckErrorCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

		m_View = p_MainCamera->GetViewMatrix();
		m_Proj = p_MainCamera->GetPerspectiveProjMatrix();
		m_MVP = m_Proj * m_View;
		m_LightedCube->SetMVP(m_MVP);

		m_LightedCube->Draw();
	}

	void SceneLight::OnImGuiRender() {}

}
