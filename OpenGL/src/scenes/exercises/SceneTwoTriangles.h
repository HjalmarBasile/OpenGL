#pragma once

#include "Scene.h"

#define TRIANGLE_VERTICES 3

namespace scene {

	class SceneTwoTriangles : public AbstractScene
	{
	public:
		static constexpr const char* name = "Two Triangles";

		SceneTwoTriangles();
		~SceneTwoTriangles();

		std::string GetName() const override;

		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;

	private:
		bool m_show_wireframe;
		std::unique_ptr<VertexArray> m_VAO1, m_VAO2;
		std::unique_ptr<VertexBuffer> m_VertexBuffer1, m_VertexBuffer2;
		std::unique_ptr<Shader> m_Shader1, m_Shader2;
	};

}
