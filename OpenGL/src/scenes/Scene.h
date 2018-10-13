#pragma once

#include <vector>
#include <utility>
#include <string>
#include <functional>
#include <type_traits>

#include "Renderer.h"
#include "imgui/imgui.h"

namespace scene {

	class AbstractScene
	{
	public:
		AbstractScene() {}
		virtual ~AbstractScene() {}

		virtual std::string GetName() const = 0;

		void OnUpdate() { OnUpdate(0.0f); }

		virtual void OnUpdate(float deltaTime) = 0;
		virtual void OnRender() = 0;
		virtual void OnImGuiRender() = 0;
	};

	class SceneMenu : public AbstractScene {
	public:
		static constexpr const char* name = "Scene Menu";

		SceneMenu(AbstractScene*& currentTestPointer);

		std::string GetName() const override;

		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;

		/* Similar pattern used by std::make_shared to pass varargs to constructor */
		template <typename T, typename... Args>
		void RegisterScene(const std::string& name, Args&&... args) {
#ifdef _PR_DEBUG
			/* We want this method to be called only for classes derived from AbstractScene */
			ASSERT_AND_BREAK((std::is_base_of<AbstractScene, T>::value))
#endif
			std::cout << "Registering Scene --> " << name << std::endl;

			/* This should work, since the created lambdas live in the same scope of the menu object */
			m_Scenes.push_back(std::make_pair(name, [&args...]() { return new T(std::forward<Args>(args)...); }));
		}

	private:
		AbstractScene*& m_CurrentScene;
		std::vector<std::pair<std::string, std::function<AbstractScene*()>>> m_Scenes;
	};

}
