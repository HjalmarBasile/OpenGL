#pragma once

#include <string>
#include <unordered_map>
#include <GL/glew.h>

#define VERTEX_BASIC_SHADER_PATH "res/shaders/basic.vert"
#define FRAGMENT_BASIC_SHADER_PATH "res/shaders/basic.frag"

class Shader
{
private:
	unsigned int m_RendererID;
	std::unordered_map<std::string, GLint> m_UniformLocationCache;
public:
	Shader(const std::string& vertfilepath, const std::string& fragfilepath);
	~Shader();

	void Use() const;
	void Unuse() const;

	void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
private:
	GLint GetUniformLocation(const std::string& name);

	static std::string ParseShader(const std::string& filepath);
	static const char* GetShaderName(GLenum shaderType);
	static GLuint CompileShader(GLenum shaderType, const std::string& source);
	static GLuint CreateShader(const std::string& vertexShader, const std::string& fragmentShader);
};
