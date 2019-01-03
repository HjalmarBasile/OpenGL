#include "Cube.h"

Cube::Cube(ShadingType shadingType, const char* texturePath)
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
	switch (shadingType)
	{
	case ShadingType::TEXTURE:
		m_Shader = std::make_unique<Shader>(VERTEX_TEXTURE_2D_POS_3D_SHADER_PATH, FRAGMENT_TEXTURE_2D_SHADER_PATH);
		break;
	case ShadingType::LIGHT:
		m_Shader = std::make_unique<Shader>(VERTEX_BASIC_MVP_SHADER_PATH, FRAGMENT_BASIC_LIGHTED_SHADER_PATH);
		break;
	default:
		throw "Unknown ShadingType enum value";
	}
	m_Shader->Use();

	if (nullptr != texturePath) {
		/* Load texture to memory */
		m_Texture2D = std::make_unique<Texture>(texturePath);
		const unsigned int slot = 0;
		m_Texture2D->Bind(slot);
		m_Shader->SetUniform1i("u_Texture", slot);
	} else {
		m_Texture2D = nullptr;
	}
}

Cube::~Cube()
{
	this->Unbind();
}

void Cube::Bind()
{
	m_VAO->Bind();
	m_Shader->Use();
	if (nullptr != m_Texture2D) {
		m_Texture2D->Bind(0);
	}
}

void Cube::Unbind()
{
	m_VAO->Unbind();
	m_VertexBuffer->Unbind();
	m_Shader->Unuse();
	if (nullptr != m_Texture2D) {
		m_Texture2D->Unbind();
	}
}

void Cube::Draw()
{
	GLCheckErrorCall(glDrawArrays(GL_TRIANGLES, 0, CUBE_VERTICES));
}

void Cube::SetMVP(glm::mat4 MVP)
{
	m_Shader->SetUniformMatrix4fv("u_MVP", MVP);
}

void Cube::SetObjectColor(glm::vec3 objectColor)
{
	m_Shader->SetUniform3f("u_ObjectColor", objectColor.r, objectColor.g, objectColor.b);
}

void Cube::SetLightColor(glm::vec3 lightColor)
{
	m_Shader->SetUniform3f("u_LightColor", lightColor.r, lightColor.g, lightColor.b);
}
