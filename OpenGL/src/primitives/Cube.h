#pragma once

#include <memory>
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "Shader.h"
#include "Texture.h"

class Cube
{
public:
	enum ShadingType {
		TEXTURE,
		LIGHT
	};

public:
	Cube(ShadingType shadingType, const char* texturePath);
	~Cube();

	void Bind();
	void Unbind();
	void Draw();

	void SetMVP(glm::mat4 MVP);
	// TODO create specific subclass
	void SetObjectColor(glm::vec3 objectColor);
	void SetLightColor(glm::vec3 lightColor);

private:
	static constexpr int CUBE_VERTICES = 36;
	static constexpr int VERTEX_SIZE = 3;
	static constexpr int UV_SIZE = 2;
	static constexpr unsigned int POSITIONS_SIZE = CUBE_VERTICES * (VERTEX_SIZE + UV_SIZE);

	static const float s_Positions[POSITIONS_SIZE];

	std::unique_ptr<VertexArray> m_VAO;
	std::unique_ptr<VertexBuffer> m_VertexBuffer;
	std::unique_ptr<Shader> m_Shader;
	std::unique_ptr<Texture> m_Texture2D;
};
