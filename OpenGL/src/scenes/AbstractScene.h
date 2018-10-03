#pragma once

#include "Renderer.h"
#include "imgui/imgui.h"

namespace scene {

	class AbstractScene
	{
	public:
		AbstractScene() {}
		virtual ~AbstractScene() {}

		virtual void OnUpdate(float deltaTime) = 0;
		virtual void OnRender() = 0;
		virtual void OnImGuiRender() = 0;
	};

}
