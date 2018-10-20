#pragma once

#include "Scene.h"

#include <math.h>
#include <GLFW/glfw3.h>

static constexpr int TRIANGLE_VERTICES = 3;

namespace scene {

	class SceneHelloTriangle : public AbstractScene
	{
	public:
		static constexpr const char* name = "Hello Triangle";

		SceneHelloTriangle();
		~SceneHelloTriangle();

		std::string GetName() const override;

		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;

	private:
		bool m_fixed_color, m_show_wireframe;
		std::unique_ptr<VertexArray> m_VAO;
		std::unique_ptr<VertexBuffer> m_VertexBuffer;
		std::unique_ptr<Shader> m_Shader;
	};

}
