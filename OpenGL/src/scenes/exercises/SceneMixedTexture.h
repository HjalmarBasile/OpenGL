#pragma once

#include <memory>
#include "Scene.h"
#include "Texture.h"

namespace scene {

	class SceneMixedTexture : public AbstractScene
	{
	public:
		static constexpr const char* name = "Mixed Texture";

		SceneMixedTexture();

		std::string GetName() const override;

		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;

	private:
		float m_MixLambda;
		std::unique_ptr<VertexArray> m_VAO;
		std::unique_ptr<VertexBuffer> m_VertexBuffer;
		std::unique_ptr<IndexBuffer> m_IndexBuffer;
		std::unique_ptr<Shader> m_Shader;
		std::unique_ptr<Texture> m_Texture2D_1, m_Texture2D_2;
	};

}
