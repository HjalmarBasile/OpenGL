#include "ScenePerspectiveProjection.h"

namespace scene {

	ScenePerspectiveProjection::ScenePerspectiveProjection(int windowWidth, int windowHeight) :
		m_WINDOW_WIDTH(windowWidth), m_WINDOW_HEIGHT(windowHeight),
		m_CameraTranslateZ(10.0f), m_FOV(45.0f)
	{
		const unsigned int POSITIONS_SIZE = 4 * 4;
		const GLint VERTEX_SIZE = 2;
		const GLint UV_SIZE = 2;
		/* Texture vertices */
		float positions[POSITIONS_SIZE] = {
			/* vertices */		/* UV coordinates */
			-10.0f, -10.0f,	0.0f, 0.0f, // 0
			 10.0f, -10.0f,	1.0f, 0.0f, // 1
			 10.0f,  10.0f,	1.0f, 1.0f, // 2
			-10.0f,  10.0f,	0.0f, 1.0f  // 3
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
		m_Texture2D = std::make_unique<Texture>(CRATE_TEXTURE_PATH);
		const unsigned int slot = 0;
		m_Texture2D->Bind(slot);
		m_Shader->SetUniform1i("u_Texture", slot);

		/* Let's define the model matrix */
		m_Model = glm::rotate(glm::mat4(1.0f), glm::radians(-65.0f), glm::vec3(1.0f, 0.0f, 0.0f));

		/* Moving the camera backwards is equivalent to moving the model forward */
		m_View = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -m_CameraTranslateZ));

		/* Perspective projection matrix */
		m_Proj = glm::perspective<float>(glm::radians(m_FOV), (float)m_WINDOW_WIDTH / (float)m_WINDOW_HEIGHT, 0.1f, 1000.0f);

		/* Finally we build our MVP matrix */
		m_MVP = m_Proj * m_View * m_Model;

		m_VAO->Unbind();
		m_VertexBuffer->Unbind();
		m_IndexBuffer->Unbind();
		m_Shader->Unuse();
	}

	std::string ScenePerspectiveProjection::GetName() const { return name; }

	void ScenePerspectiveProjection::OnUpdate(float deltaTime) {}

	void ScenePerspectiveProjection::OnRender()
	{
		Renderer::Clear();
		m_Shader->Use();

		m_View = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -m_CameraTranslateZ));
		m_Proj = glm::perspective<float>(glm::radians(m_FOV), (float)m_WINDOW_WIDTH / (float)m_WINDOW_HEIGHT, 0.1f, 1000.0f);
		m_MVP = m_Proj * m_View * m_Model;
		m_Shader->SetUniformMatrix4fv("u_MVP", m_MVP);
		Renderer::Draw(*m_VAO, *m_IndexBuffer, *m_Shader);
	}

	void ScenePerspectiveProjection::OnImGuiRender()
	{
		ImGui::Begin("Scene Perspective Projection");
		ImGui::SliderFloat("Camera Translate Z", &m_CameraTranslateZ, 10.0f, 100.0f);
		ImGui::SliderFloat("Camera FOV", &m_FOV, 45.0f, 145.0f);
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::End();
	}

}
