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

	void SetMVP(glm::mat4 MVP);

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

	void SetLightColor(glm::vec3 lightColor);
};

class LightedCube : public Cube
{
public:
	LightedCube();
	~LightedCube();

	void Bind() override;
	void Unbind() override;

	void SetModelMatrix(glm::mat4 Model);
	void SetObjectColor(glm::vec3 objectColor);
	void SetAmbientColor(glm::vec3 ambientColor);
	void SetLightColor(glm::vec3 lightColor);
	void SetLightPosition(glm::vec3 lightPosition);
	void SetViewPosition(glm::vec3 viewPosition);
};
