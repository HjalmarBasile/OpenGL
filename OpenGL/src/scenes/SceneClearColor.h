#pragma once

#include "Scene.h"

namespace scene {

	class SceneClearColor : public AbstractScene
	{
	public:
		static constexpr const char* name = "Clear Color";

		SceneClearColor(float r, float g, float b, float a);

		std::string GetName() const override;

		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;

	private:
		float m_ClearColor[4];
		ImVec2 m_Size;
	};

}
