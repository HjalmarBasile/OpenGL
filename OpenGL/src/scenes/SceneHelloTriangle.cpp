#include "SceneHelloTriangle.h"

namespace scene {

	SceneHelloTriangle::SceneHelloTriangle() :
		m_fixed_color(true), m_show_wireframe(false)
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
		m_Shader = std::make_unique<Shader>(VERTEX_BASIC_SHADER_PATH, FRAGMENT_BASIC_SHADER_PATH);
		m_Shader->Use();

		/*
			Unbind all: this will prove we don't need to bind the array buffer
			and specify its attributes anymore: it's all inside the vertex array object!
		*/
		m_VAO->Unbind();
		m_VertexBuffer->Unbind();
		m_Shader->Unuse();
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

		if (m_fixed_color) {
			m_Shader->SetUniform4f(UNIFORM_COLOR, 1.0f, 0.5f, 0.2f, 1.0f);
		} else {
			float currentTime = (float)glfwGetTime();
			float green = 0.5f * sin(currentTime) + 0.5f;
			m_Shader->SetUniform4f(UNIFORM_COLOR, 1.0f, green, 0.2f, 1.0f);
		}

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
		ImGui::Checkbox("Fixed color", &m_fixed_color);
		ImGui::Checkbox("Show Wireframe", &m_show_wireframe);
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::End();
	}

}
