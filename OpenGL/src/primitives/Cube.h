#pragma once

#include <memory>
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "Shader.h"
#include "Texture.h"

class Cube
{
public:
	Cube();
	virtual ~Cube();

	void Draw();
	virtual void Bind() = 0;
	virtual void Unbind() = 0;

	void SetMVP(const glm::mat4& MVP);

protected:
	static constexpr int CUBE_VERTICES = 36;
	static constexpr int VERTEX_SIZE = 3;
	static constexpr int NORMAL_SIZE = 3;
	static constexpr int UV_SIZE = 2;
	static constexpr unsigned int POSITIONS_SIZE = CUBE_VERTICES * (VERTEX_SIZE + NORMAL_SIZE + UV_SIZE);

	static const float s_Positions[POSITIONS_SIZE];

	std::unique_ptr<VertexArray> m_VAO;
	std::unique_ptr<VertexBuffer> m_VertexBuffer;
	std::unique_ptr<Shader> m_Shader;
};

class TexturedCube : public Cube
{
public:
	TexturedCube(const char* texturePath);
	~TexturedCube();

	void Bind() override;
	void Unbind() override;

private:
	std::unique_ptr<Texture> m_Texture2D;
};

class LampCube : public Cube
{
public:
	LampCube();
	~LampCube();

	void Bind() override;
	void Unbind() override;

	void SetLightColor(const glm::vec3& lightColor);
};

class LightedCube : public Cube
{
public:
	LightedCube(glm::vec3 ambientColor, glm::vec3 objectColor, glm::vec3 lightColor,
		const char* vertShader = VERTEX_POS_NORM_UMVP_SHADER_PATH, const char* fragShader = FRAGMENT_BASIC_LIGHTED_SHADER_PATH);
	~LightedCube();

	void Bind() override;
	void Unbind() override;

	void SetViewMatrix(const glm::mat4& view);
	void SetModelViewMatrix(const glm::mat4& modelView);
	void SetObjectColor(const glm::vec3& objectColor);
	void SetAmbientColor(const glm::vec3& ambientColor);
	void SetLightColor(const glm::vec3& lightColor);
	void SetLightPosition(const glm::vec3& lightPosition);
	void SetAmbientStrenght(float ambientStrenght);
	void SetDiffuseStrenght(float diffuseStrenght);
	void SetSpecularStrenght(float specularStrenght);
	void SetSpecularShininess(float specularShininess);

private:
	static constexpr float AMBIENT_STRENGHT_DEFAULT = 0.8f;
	static constexpr float DIFFUSE_STRENGHT_DEFAULT = 1.0f;
	static constexpr float SPECULAR_STRENGHT_DEFAULT = 0.5f;
	static constexpr float SPECULAR_SHININESS_DEFAULT = 32.0f;
};
