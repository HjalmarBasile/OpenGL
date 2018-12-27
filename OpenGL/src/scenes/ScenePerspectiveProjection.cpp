#include "ScenePerspectiveProjection.h"

#include <GLFW/glfw3.h>

namespace scene {

	ScenePerspectiveProjection::ScenePerspectiveProjection(int windowWidth, int windowHeight) :
		m_ASPECT_RATIO((float)windowWidth / (float)windowHeight),
		m_ModelScale(1.0f), m_CameraTranslateZ(10.0f), m_FOV(45.0f), m_ZBufferClearValue(1.0f)
	{
		const GLint VERTEX_SIZE = 3;
		const GLint UV_SIZE = 2;
		const unsigned int POSITIONS_SIZE = CUBE_VERTICES * (VERTEX_SIZE + UV_SIZE);

		/* Cube */
		float positions[POSITIONS_SIZE] = {
			/* vertices */		  /* UV coordinates */
			-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
			-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
			 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
			 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
			 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
			-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

			-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
			 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
			 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
			 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
			-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
			-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

			-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
			-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
			-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
			-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
			-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
			-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

			 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
			 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
			 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
			 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
			 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
			 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

			-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
			 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
			 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
			 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
			-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
			-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

			-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
			-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
			 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
			 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
			 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
			-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
		};

		/* Enable blending */
		GLCheckErrorCall(glEnable(GL_BLEND));
		/* Transparency implementation */
		GLCheckErrorCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

		/* Enable depth testing */
		GLCheckErrorCall(glEnable(GL_DEPTH_TEST));

		GLCheckErrorCall(glClearColor(0.20f, 0.35f, 0.50f, 1.00f));

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

		/* Create shader program */
		m_Shader = std::make_unique<Shader>(VERTEX_TEXTURE_2D_POS_3D_SHADER_PATH, FRAGMENT_TEXTURE_2D_SHADER_PATH);
		m_Shader->Use();

		/* Load texture to memory */
		m_Texture2D = std::make_unique<Texture>(CRATE_TEXTURE_PATH);
		const unsigned int slot = 0;
		m_Texture2D->Bind(slot);
		m_Shader->SetUniform1i("u_Texture", slot);
	}

	ScenePerspectiveProjection::~ScenePerspectiveProjection()
	{
		GLCheckErrorCall(glDisable(GL_DEPTH_TEST));
	}

	std::string ScenePerspectiveProjection::GetName() const { return name; }

	void ScenePerspectiveProjection::OnUpdate(float deltaTime) {}

	void ScenePerspectiveProjection::OnRender()
	{
		GLCheckErrorCall(glClearDepth(m_ZBufferClearValue));
		GLCheckErrorCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

		m_Model = glm::rotate(glm::mat4(1.0f), (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.3f));
		m_Model = glm::scale(m_Model, glm::vec3(m_ModelScale, m_ModelScale, m_ModelScale));

		/* Moving the camera backwards is equivalent to moving the model forward */
		m_View = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -m_CameraTranslateZ));

		/* N.B. Depth testing does not work if zNear is set to 0.0f ! */
		m_Proj = glm::perspective<float>(glm::radians(m_FOV), m_ASPECT_RATIO, 0.1f, 100.0f);
		m_MVP = m_Proj * m_View * m_Model;
		m_Shader->SetUniformMatrix4fv("u_MVP", m_MVP);
		GLCheckErrorCall(glDrawArrays(GL_TRIANGLES, 0, CUBE_VERTICES));
	}

	void ScenePerspectiveProjection::OnImGuiRender()
	{
		ImGui::Begin("Scene Perspective Projection");
		ImGui::SliderFloat("Model Scale", &m_ModelScale, 1.0f, 10.0f);
		ImGui::SliderFloat("Camera Translate Z", &m_CameraTranslateZ, 10.0f, 100.0f);
		ImGui::SliderFloat("Camera FOV", &m_FOV, 45.0f, 145.0f);
		ImGui::SliderFloat("Z-buffer clear value", &m_ZBufferClearValue, 0.0f, 1.0f);
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::End();
	}

}
