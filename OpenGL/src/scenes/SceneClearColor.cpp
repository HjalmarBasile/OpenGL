#include "SceneClearColor.h"

namespace scene {

	SceneClearColor::SceneClearColor(float r, float g, float b, float a) : m_ClearColor{r, g, b, a} {}

	void SceneClearColor::OnUpdate(float deltaTime) {}

	void SceneClearColor::OnRender()
	{
		/* Specify clear values for the color buffers */
		GLCheckErrorCall(glClearColor(m_ClearColor[0], m_ClearColor[1], m_ClearColor[2], m_ClearColor[3]));
		GLCheckErrorCall(glClear(GL_COLOR_BUFFER_BIT));
	}

	void SceneClearColor::OnImGuiRender()
	{
		ImGui::ColorEdit4("Clear Color", m_ClearColor);
	}

}
