#include "SceneMixedTexture.h"

namespace scene {

	SceneMixedTexture::SceneMixedTexture() : m_MixLambda(0.5f)
	{
		const GLint POSITION_SIZE = 3;
		const GLint COLOR_SIZE = 3;
		const GLint UV_SIZE = 2;
		const unsigned int VERTICES_SIZE = 4 * (POSITION_SIZE + COLOR_SIZE + UV_SIZE);
		/* Texture vertices */
		float vertices[VERTICES_SIZE] = {
			/* positions */		/* colors */		/* UVs */
			 0.5f,  0.5f, 0.0f,	1.0f, 0.0f, 0.0f,	1.0f, 1.0f, // 1
			 0.5f, -0.5f, 0.0f,	0.0f, 1.0f, 0.0f,	1.0f, 0.0f, // 2
			-0.5f, -0.5f, 0.0f,	0.0f, 0.0f, 1.0f,	0.0f, 0.0f, // 3
			-0.5f,  0.5f, 0.0f,	1.0f, 0.0f, 1.0f,	0.0f, 1.0f  // 4
		};

		const unsigned int INDICES_SIZE = 6;
		unsigned int indices[INDICES_SIZE] = {
			0, 1, 2,
			2, 3, 0
		};

		/* Enable blending */
		GLCheckErrorCall(glEnable(GL_BLEND));
		GLCheckErrorCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

		m_VAO = std::make_unique<VertexArray>();
		m_VertexBuffer = std::make_unique<VertexBuffer>(vertices, VERTICES_SIZE * sizeof(float));

		VertexBufferLayout layout;
		layout.Push<float>(POSITION_SIZE);
		layout.Push<float>(COLOR_SIZE);
		layout.Push<float>(UV_SIZE);
		m_VAO->AddBuffer(*m_VertexBuffer, layout);

		m_IndexBuffer = std::make_unique<IndexBuffer>(indices, INDICES_SIZE);

		m_Shader = std::make_unique<Shader>(VERTEX_POS_COL_UV_SHADER_PATH, FRAGMENT_POS_COL_UV_SHADER_PATH);
		m_Shader->Use();

		/* Load textures to memory */
		const unsigned int slot0 = 0;
		const unsigned int slot1 = 1;

		m_Texture2D_1 = std::make_unique<Texture>(AWESOME_FACE_TEXTURE_PATH);
		m_Texture2D_2 = std::make_unique<Texture>(DICE_TEXTURE_PATH);

		m_Shader->SetUniform1i(UNIFORM_TEXTURE1, slot0);
		m_Shader->SetUniform1i(UNIFORM_TEXTURE2, slot1);

		m_Texture2D_1->Bind(slot0);
		m_Texture2D_2->Bind(slot1);

		m_VertexBuffer->Unbind();

		GLCheckErrorCall(glClearColor(0.2f, 0.3f, 0.5f, 1.0f));
	}

	std::string SceneMixedTexture::GetName() const { return std::string(); }

	void SceneMixedTexture::OnUpdate(float deltaTime) {}

	void SceneMixedTexture::OnRender()
	{
		Renderer::Clear();
		m_Shader->SetUniform1f(UNIFORM_MIX_LAMBDA, m_MixLambda);
		Renderer::Draw(*m_VAO, *m_IndexBuffer, *m_Shader);
	}

	void SceneMixedTexture::OnImGuiRender()
	{
		ImGui::Begin("Mixed Texture");
		ImGui::SliderFloat("Mixture Lambda", &m_MixLambda, 0.0f, 1.0f);
		ImGui::Text("Notice the transparency difference: this is because\n"
					"the face texture has white background with alpha equal to 0, while\n"
					"the dice texture has white background with alpha equal to 1.");
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::End();
	}

}
