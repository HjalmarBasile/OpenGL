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

		std::unique_ptr<Cube> m_LightedCube;

		glm::mat4 m_Model;
		glm::mat4 m_View;
		glm::mat4 m_Proj;
		glm::mat4 m_MVP;
	};

}
