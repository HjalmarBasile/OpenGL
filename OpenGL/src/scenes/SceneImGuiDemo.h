#pragma once

#include "Scene.h"

namespace scene {

	class SceneImGuiDemo : public AbstractScene
	{
	public:
		SceneImGuiDemo();

		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;

	private:
		bool m_show_demo_window;
		bool m_show_another_window;
	};

}
