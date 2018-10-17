#include "SceneHelloTriangle.h"

namespace scene {

	SceneHelloTriangle::SceneHelloTriangle() : m_show_wireframe(false)
	{
		const GLint VERTEX_SIZE = 3;
		const unsigned int POSITIONS_SIZE = TRIANGLE_VERTICES * VERTEX_SIZE;
		/* Triangle vertices */
		float positions[POSITIONS_SIZE] = {
			-0.5f, -0.5f, 0.0f,
			 0.5f, -0.5f, 0.0f,
			 0.0f,  0.5f, 0.0f
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

		/* Create shader program */
		m_Shader = std::make_unique<Shader>(VERTEX_TRIANGLE_SHADER_PATH, FRAGMENT_TRIANGLE_SHADER_PATH);
		m_Shader->Use();

		/*
			Unbind all: this will prove we don't need to bind the array buffer
			and specify its attributes anymore: it's all inside the vertex array object!
		*/
		m_VAO->Unbind();
		m_VertexBuffer->Unbind();
		m_Shader->Unuse();

		/* Choose a background color */
		GLCheckErrorCall(glClearColor(0.2f, 0.3f, 0.3f, 1.0f));
	}

	SceneHelloTriangle::~SceneHelloTriangle()
	{
		GLCheckErrorCall(glPolygonMode(GL_FRONT_AND_BACK, GL_FILL));
	}

	std::string SceneHelloTriangle::GetName() const { return name; }

	void SceneHelloTriangle::OnUpdate(float deltaTime) {}

	void SceneHelloTriangle::OnRender()
	{
		Renderer::Clear();

		m_VAO->Bind();
		m_Shader->Use();

		if (m_show_wireframe) {
			GLCheckErrorCall(glPolygonMode(GL_FRONT_AND_BACK, GL_LINE));
		} else {
			GLCheckErrorCall(glPolygonMode(GL_FRONT_AND_BACK, GL_FILL));
		}

		GLCheckErrorCall(glDrawArrays(GL_TRIANGLES, 0, TRIANGLE_VERTICES));
	}

	void SceneHelloTriangle::OnImGuiRender()
	{
		ImGui::Begin("Scene Hello Triangle");
		ImGui::Checkbox("Show Wireframe", &m_show_wireframe);
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::End();
	}

}
