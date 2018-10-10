#include "SceneBasicSquare.h"

namespace scene {

	SceneBasicSquare::SceneBasicSquare() :
		m_Red(0.0f), m_RedIncrease(0.01f)
	{
		const unsigned int POSITIONS_SIZE = 8;
		const GLint VERTEX_SIZE = 2;
		/* Square vertices */
		float positions[POSITIONS_SIZE] = {
			-0.5f, -0.5f,
			 0.5f, -0.5f,
			 0.5f,  0.5f,
			-0.5f,  0.5f
		};
		/* It won't appear as a square since we are not dealing with the projection matrix */

		const unsigned int INDICES_SIZE = 6;
		/* Index buffer */
		unsigned int indices[INDICES_SIZE] = {
			0, 1, 2,
			2, 3, 0
		};

		/* Generate vertex array object */
		m_VAO = std::make_unique<VertexArray>();

		/* Create and bind vertex buffer */
		m_VertexBuffer = std::make_unique<VertexBuffer>(positions, POSITIONS_SIZE * sizeof(float));

		/* Define a vertex buffer layout */
		VertexBufferLayout layout;
		layout.Push<float>(VERTEX_SIZE);

		/* m_VAO is defined by the pair (m_VertexBuffer, layout) */
		m_VAO->AddBuffer(*m_VertexBuffer, layout);

		/* Create and bind index buffer */
		m_IndexBuffer = std::make_unique<IndexBuffer>(indices, INDICES_SIZE);

		/* Create shader program */
		m_Shader = std::make_unique<Shader>(VERTEX_BASIC_SHADER_PATH, FRAGMENT_BASIC_SHADER_PATH);
		m_Shader->Use();

		/*
			Unbind all: this will prove we don't need to bind the array buffer
			and specify its attributes anymore: it's all inside the vertex array object!
		*/
		m_VAO->Unbind();
		m_VertexBuffer->Unbind();
		m_IndexBuffer->Unbind();
		m_Shader->Unuse();
	}

	void SceneBasicSquare::OnUpdate(float deltaTime) {
		if (m_Red > 1.0f) {
			m_Red = 2.0f - m_Red;
			m_RedIncrease = -0.01f;
		} else if (m_Red < 0.0f) {
			m_Red = -m_Red;
			m_RedIncrease = 0.01f;
		}
	}

	void SceneBasicSquare::OnRender()
	{
		Renderer::Clear();

		m_Shader->Use();
		/* Set uniform variable */
		m_Shader->SetUniform4f("u_Color", m_Red, 0.3f, 0.8f, 1.0f);

		Renderer::Draw(*m_VAO, *m_IndexBuffer, *m_Shader);

		m_Red += m_RedIncrease;
	}

	void SceneBasicSquare::OnImGuiRender() {}

}
