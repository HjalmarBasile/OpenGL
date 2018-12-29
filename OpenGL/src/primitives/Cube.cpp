#include "Cube.h"

Cube::Cube(const char* texturePath)
{
	/* Generate vertex array object */
	m_VAO = std::make_unique<VertexArray>();

	/* Create and bind vertex buffer */
	m_VertexBuffer = std::make_unique<VertexBuffer>(m_Positions, POSITIONS_SIZE * sizeof(float));

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
	m_Texture2D = std::make_unique<Texture>(texturePath);
	const unsigned int slot = 0;
	m_Texture2D->Bind(slot);
	m_Shader->SetUniform1i("u_Texture", slot);
}

Cube::~Cube()
{
	this->Unbind();
}

void Cube::Bind()
{
	m_VAO->Bind();
	m_Shader->Use();
	m_Texture2D->Bind(0);
}

void Cube::Unbind()
{
	m_VAO->Unbind();
	m_VertexBuffer->Unbind();
	m_Shader->Unuse();
	m_Texture2D->Unbind();
}

void Cube::SetMVP(glm::mat4 MVP)
{
	m_Shader->SetUniformMatrix4fv("u_MVP", MVP);
}
