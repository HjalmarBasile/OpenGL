#pragma once

#include "Scene.h"
#include "Camera.h"
#include "primitives/Cube.h"

namespace scene {

	class SceneLight : public AbstractScene
	{
	public:
		static constexpr const char* name = "Light";
		SceneLight(Camera* camera, bool* useMainCamera);
		~SceneLight();

		std::string GetName() const override;

		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;

	private:
		Camera* p_MainCamera;
		bool* p_UseMainCamera;

		glm::vec3 m_BackgroundColor;
		glm::vec3 m_LightColor;
		glm::vec3 m_LightSourcePosition;

		std::unique_ptr<LampCube> m_LampCube;
		std::unique_ptr<LightedCube> m_LightedCube;

		float m_AmbientStrenght;
		float m_DiffuseStrenght;
		float m_SpecularStrenght;
		float m_SpecularShininess;

		glm::mat4 m_Model;
		glm::mat4 m_View;
		glm::mat4 m_Proj;
		glm::mat4 m_ModelView;
		glm::mat4 m_MVP;
	};

}
