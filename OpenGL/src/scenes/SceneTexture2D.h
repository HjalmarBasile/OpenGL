#pragma once

#include <memory>
#include "Scene.h"
#include "Texture.h"

namespace scene {

	class SceneTexture2D : public AbstractScene
	{
	public:
		static constexpr const char* name = "Texture2D";

		SceneTexture2D(int windowWidth, int windowHeight);

		std::string GetName() const override;

		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;

	private:
		const int m_WINDOW_WIDTH;
		const int m_WINDOW_HEIGHT;

		std::unique_ptr<VertexArray> m_VAO;
		std::unique_ptr<VertexBuffer> m_VertexBuffer;
		std::unique_ptr<IndexBuffer> m_IndexBuffer;
		std::unique_ptr<Shader> m_Shader;
		std::unique_ptr<Texture> m_Texture2D;

		glm::mat4 m_Model;
		glm::mat4 m_View;
		glm::mat4 m_Proj;
		glm::mat4 m_MVP;

		glm::vec3 m_ModelTranslationA;
		glm::vec3 m_ModelTranslationB;
	};

}
