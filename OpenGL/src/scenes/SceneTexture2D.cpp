#include "SceneTexture2D.h"

namespace scene {

	SceneTexture2D::SceneTexture2D(int windowWidth, int windowHeight) :
		m_WINDOW_WIDTH(windowWidth), m_WINDOW_HEIGHT(windowHeight),
		m_ModelTranslationA(glm::vec3(0.0f, 0.0f, 0.0f)), m_ModelTranslationB(glm::vec3(0.0f, 0.0f, 0.0f))
	{
		const unsigned int POSITIONS_SIZE = 4 * 4;
		const GLint VERTEX_SIZE = 2;
		const GLint UV_SIZE = 2;
		/* Texture vertices */
		float positions[POSITIONS_SIZE] = {
			/* vertices */		/* UV coordinates */
			-180.0f, -135.0f,	0.0f, 0.0f, // 0
			 180.0f, -135.0f,	1.0f, 0.0f, // 1
			 180.0f,  135.0f,	1.0f, 1.0f, // 2
			-180.0f,  135.0f,	0.0f, 1.0f  // 3
		};

		const unsigned int INDICES_SIZE = 6;
		/* Index buffer */
		unsigned int indices[INDICES_SIZE] = {
			0, 1, 2,
			2, 3, 0
		};

		/* Enable blending */
		GLCheckErrorCall(glEnable(GL_BLEND));
		/* Transparency implementation */
		GLCheckErrorCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

		/* Generate vertex array object */
		m_VAO = std::make_unique<VertexArray>();

		/* Create and bind vertex buffer */
		m_VertexBuffer = std::make_unique<VertexBuffer>(positions, POSITIONS_SIZE * sizeof(float));

		/* Define a vertex buffer layout */
		VertexBufferLayout layout;
		layout.Push<float>(VERTEX_SIZE);
		layout.Push<float>(UV_SIZE);

		/* m_VAO is defined by the pair (m_VertexBuffer, layout) */
		m_VAO->AddBuffer(*m_VertexBuffer, layout);

		/* Create and bind index buffer */
		m_IndexBuffer = std::make_unique<IndexBuffer>(indices, INDICES_SIZE);

		/* Create shader program */
		m_Shader = std::make_unique<Shader>(VERTEX_TEXTURE_2D_SHADER_PATH, FRAGMENT_TEXTURE_2D_SHADER_PATH);
		m_Shader->Use();

		/* Load texture to memory */
		m_Texture2D = std::make_unique<Texture>(DICE_TEXTURE_PATH);
		const unsigned int slot = 0;
		m_Texture2D->Bind(slot);
		m_Shader->SetUniform1i("u_Texture", slot);

		/* Let's define the model matrix */
		m_Model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));

		/* Moving the camera to the right is equivalent to move the model to the left */
		float cameraTranslateX = 0.0f; /* For now let's keep it where it is anyway */
		m_View = glm::translate(glm::mat4(1.0f), glm::vec3(-cameraTranslateX, 0.0f, 0.0f));

		/* Orthographic projection matrix (window aspect ratio) */
		/* See math folder in the solution dir for explanation */
		m_Proj = glm::ortho<float>(0.0f, (float)m_WINDOW_WIDTH, 0.0f, (float)m_WINDOW_HEIGHT);

		/* Finally we build our MVP matrix */
		m_MVP = m_Proj * m_View * m_Model;

		/*
			Unbind all: this will prove we don't need to bind the array buffer
			and specify its attributes anymore: it's all inside the vertex array object!
		*/
		m_VAO->Unbind();
		m_VertexBuffer->Unbind();
		m_IndexBuffer->Unbind();
		m_Shader->Unuse();
	}

	void SceneTexture2D::OnUpdate(float deltaTime) {}

	void SceneTexture2D::OnRender()
	{
		Renderer::Clear();
		m_Shader->Use();

		/* Update MVP every frame */
		{
			m_Model = glm::translate(glm::mat4(1.0f), m_ModelTranslationA);
			m_MVP = m_Proj * m_View * m_Model;
			m_Shader->SetUniformMatrix4fv("u_MVP", m_MVP);
			Renderer::Draw(*m_VAO, *m_IndexBuffer, *m_Shader);
		}

		{
			m_Model = glm::translate(glm::mat4(1.0f), m_ModelTranslationB);
			m_MVP = m_Proj * m_View * m_Model;
			m_Shader->SetUniformMatrix4fv("u_MVP", m_MVP);
			Renderer::Draw(*m_VAO, *m_IndexBuffer, *m_Shader);
		}
	}

	void SceneTexture2D::OnImGuiRender()
	{
		/* Show a simple ImGui window. We use a Begin/End pair to create a named window. */
		ImGui::Begin("Scene Texture2D");
		ImGui::Text("Use the slider to move the model around.");
		ImGui::SliderFloat3("Model A Translation", reinterpret_cast<float*>(&m_ModelTranslationA.x), 0.0f, (float)m_WINDOW_WIDTH);
		ImGui::SliderFloat3("Model B Translation", reinterpret_cast<float*>(&m_ModelTranslationB.x), 0.0f, (float)m_WINDOW_WIDTH);
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::End();
	}

}
