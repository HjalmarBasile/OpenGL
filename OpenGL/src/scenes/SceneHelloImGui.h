#pragma once

#include "Scene.h"

namespace scene {

	class SceneHelloImGui : public AbstractScene
	{
	public:
		static constexpr const char* name = "Hello ImGui";

		SceneHelloImGui();

		std::string GetName() const override;

		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;

	private:
		bool m_show_demo_window;
		bool m_show_another_window;
	};

}
