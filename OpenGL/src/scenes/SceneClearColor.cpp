#include "SceneClearColor.h"

namespace scene {

	SceneClearColor::SceneClearColor(float r, float g, float b, float a) :
		m_ClearColor{ r, g, b, a }, m_Size{ 350.0f, 80.0f } {}

	std::string SceneClearColor::GetName() const { return name; }

	void SceneClearColor::OnUpdate(float deltaTime) {}

	void SceneClearColor::OnRender()
	{
		/* Specify clear values for the color buffers */
		GLCheckErrorCall(glClearColor(m_ClearColor[0], m_ClearColor[1], m_ClearColor[2], m_ClearColor[3]));
		Renderer::Clear();
	}

	void SceneClearColor::OnImGuiRender()
	{
		ImGui::SetNextWindowSize(m_Size);

		ImGui::Begin("Color Picker");
		ImGui::ColorEdit4("Clear Color", m_ClearColor);

		if (!ImGui::IsWindowCollapsed()) {
			m_Size = ImGui::GetWindowSize();
		}
		ImGui::End();
	}

}
