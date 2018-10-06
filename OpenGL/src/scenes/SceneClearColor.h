#pragma once

#include "AbstractScene.h"

namespace scene {

	class SceneClearColor : public AbstractScene
	{
	public:
		SceneClearColor(float r, float g, float b, float a);

		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;

	private:
		float m_ClearColor[4];
		ImVec2 m_Size;
	};

}
