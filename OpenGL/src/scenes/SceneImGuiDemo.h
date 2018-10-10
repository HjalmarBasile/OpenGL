#pragma once

#include "Scene.h"

namespace scene {

	class SceneImGuiDemo : public AbstractScene
	{
	public:
		static constexpr const char* name = "ImGui Demo";

		SceneImGuiDemo();

		std::string GetName() const override;

		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;

	private:
		bool m_show_demo_window;
		bool m_show_another_window;
	};

}
