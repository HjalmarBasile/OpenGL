#pragma once

#include <memory>
#include "Scene.h"
#include "primitives/Cube.h"

namespace scene {

	class ScenePerspectiveProjection : public AbstractScene
	{
	public:
		static constexpr const char* name = "Perspective Projection";

		ScenePerspectiveProjection(int windowWidth, int windowHeight);
		~ScenePerspectiveProjection();

		std::string GetName() const override;

		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;

	private:
		static constexpr int TOTAL_CUBES = 10;
		const float m_ASPECT_RATIO;

		std::unique_ptr<Cube> cube;

		glm::vec3 m_CubesPositions[TOTAL_CUBES];
		glm::vec3 m_CubesRotations[TOTAL_CUBES];
		glm::mat4 m_Model;
		glm::mat4 m_View;
		glm::mat4 m_Proj;
		glm::mat4 m_MVP;

		float m_ModelScale;
		float m_CameraTranslateZ;
		float m_FOV;
		float m_ZBufferClearValue;
	};

}
