#include "Scene.h"

namespace scene {

	SceneMenu::SceneMenu(AbstractScene*& currentScenePointer) : m_CurrentScene(currentScenePointer) {}

	std::string SceneMenu::GetName() const { return name; }

	void SceneMenu::OnUpdate(float deltaTime) {}

	void SceneMenu::OnRender() {
		Renderer::ClearColorSetBlack();
		Renderer::Clear();
	}

	void SceneMenu::OnImGuiRender() {
		for (auto& scene : m_Scenes) {
			if (ImGui::Button(scene.first.c_str())) {
				m_CurrentScene = scene.second();
			}
		}
	}

}
