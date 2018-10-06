#include "SceneImGuiDemo.h"

namespace scene {

	SceneImGuiDemo::SceneImGuiDemo()
	{
		/* Do not show ImGui demo windows at startup */
		m_show_demo_window = false;
		m_show_another_window = false;
	}

	void SceneImGuiDemo::OnUpdate(float deltaTime) {}

	void SceneImGuiDemo::OnRender() {
		Renderer::Clear();
	}

	void SceneImGuiDemo::OnImGuiRender()
	{
		/* 1. Show the ImGui big demo window */
		if (m_show_demo_window) {
			ImGui::ShowDemoWindow(&m_show_demo_window);
		}

		/* 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window. */
		{
			static float f = 0.0f;
			static int counter = 0;

			/* Create a window called "Hello, ImGui!" and append into it. */
			ImGui::Begin("Hello, ImGui!");

			ImGui::Text("This is some useful text.");

			/* Edit bools storing our window open/close state */
			ImGui::Checkbox("Demo Window", &m_show_demo_window);
			ImGui::Checkbox("Another Window", &m_show_another_window);

			ImGui::SliderFloat("float", &f, 0.0f, 1.0f);

			/* Buttons return true when clicked (most widgets return true when edited/activated) */
			if (ImGui::Button("Button")) {
				counter++;
			}
			ImGui::SameLine();
			ImGui::Text("counter = %d", counter);

			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
			ImGui::End();
		}

		/* 3. Show another simple window. */
		if (m_show_another_window)
		{
			/*
				Pass a pointer to our bool variable (the window will have a
				closing button that will clear the bool when clicked)
			*/
			ImGui::Begin("Another Window", &m_show_another_window);
			ImGui::Text("Hello from another window!");
			if (ImGui::Button("Close Me")) {
				m_show_another_window = false;
			}
			ImGui::End();
		}
	}

}
