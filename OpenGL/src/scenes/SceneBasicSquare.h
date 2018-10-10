#pragma once

#include "Scene.h"

namespace scene {

	class SceneBasicSquare : public AbstractScene
	{
	public:
		static constexpr const char* name = "Basic Square";

		SceneBasicSquare();

		std::string GetName() const override;

		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;

	private:
		float m_Red;
		float m_RedIncrease;

		std::unique_ptr<VertexArray> m_VAO;
		std::unique_ptr<VertexBuffer> m_VertexBuffer;
		std::unique_ptr<IndexBuffer> m_IndexBuffer;
		std::unique_ptr<Shader> m_Shader;
	};

}
