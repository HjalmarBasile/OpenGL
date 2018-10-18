#include "SceneTwoTriangles.h"

namespace scene {

	SceneTwoTriangles::SceneTwoTriangles() : m_show_wireframe(false)
	{
		const GLint VERTEX_SIZE = 3;
		const unsigned int POSITIONS_SIZE = TRIANGLE_VERTICES * VERTEX_SIZE;
		float triangle1[POSITIONS_SIZE] = {
			-0.50f, -0.50f, 0.00f,
			 0.50f, -0.50f, 0.00f,
			 0.00f,  0.50f, 0.00f
		};
		float triangle2[POSITIONS_SIZE] = {
			0.25f, 0.75f, 0.00f,
			0.75f, 0.75f, 0.00f,
			0.50f, 0.25f, 0.00f
		};

		VertexBufferLayout layout;
		layout.Push<float>(VERTEX_SIZE);

		m_VAO1 = std::make_unique<VertexArray>();
		m_VertexBuffer1 = std::make_unique<VertexBuffer>(triangle1, POSITIONS_SIZE * sizeof(float));
		m_VAO1->AddBuffer(*m_VertexBuffer1, layout);

		m_VAO2 = std::make_unique<VertexArray>();
		m_VertexBuffer2 = std::make_unique<VertexBuffer>(triangle2, POSITIONS_SIZE * sizeof(float));
		m_VAO2->AddBuffer(*m_VertexBuffer2, layout);

		m_Shader = std::make_unique<Shader>(VERTEX_TRIANGLE_SHADER_PATH, FRAGMENT_TRIANGLE_SHADER_PATH);
		m_Shader->Use();

		VertexArray::Unbind();
		VertexBuffer::Unbind();

		GLCheckErrorCall(glClearColor(0.2f, 0.3f, 0.3f, 1.0f));
	}

	SceneTwoTriangles::~SceneTwoTriangles()
	{
		GLCheckErrorCall(glPolygonMode(GL_FRONT_AND_BACK, GL_FILL));
	}

	std::string SceneTwoTriangles::GetName() const { return name; }

	void SceneTwoTriangles::OnUpdate(float deltaTime) {}

	void SceneTwoTriangles::OnRender()
	{
		Renderer::Clear();

		if (m_show_wireframe) {
			GLCheckErrorCall(glPolygonMode(GL_FRONT_AND_BACK, GL_LINE));
		} else {
			GLCheckErrorCall(glPolygonMode(GL_FRONT_AND_BACK, GL_FILL));
		}

		m_VAO1->Bind();
		GLCheckErrorCall(glDrawArrays(GL_TRIANGLES, 0, TRIANGLE_VERTICES));

		m_VAO2->Bind();
		GLCheckErrorCall(glDrawArrays(GL_TRIANGLES, 0, TRIANGLE_VERTICES));
	}

	void SceneTwoTriangles::OnImGuiRender()
	{
		ImGui::Begin("Scene Two Triangles (Exercise)");
		ImGui::Checkbox("Show Wireframe", &m_show_wireframe);
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::End();
	}

}
