#pragma once

#include <string>
#include <unordered_map>
#include <glad/glad.h>

#include "glm/gtc/matrix_transform.hpp"

static constexpr const char* VERTEX_BASIC_SHADER_PATH = "res/shaders/basic.vert";
static constexpr const char* FRAGMENT_BASIC_SHADER_PATH = "res/shaders/basic.frag";

static constexpr const char* VERTEX_POS_COL_SHADER_PATH = "res/shaders/pos_col.vert";
static constexpr const char* FRAGMENT_COL_IN_SHADER_PATH = "res/shaders/col_in.frag";

static constexpr const char* VERTEX_TEXTURE_2D_SHADER_PATH = "res/shaders/texture2D.vert";
static constexpr const char* FRAGMENT_TEXTURE_2D_SHADER_PATH = "res/shaders/texture2D.frag";

static constexpr const char* VERTEX_POS_COL_UV_SHADER_PATH = "res/shaders/pos_col_uv.vert";
static constexpr const char* FRAGMENT_POS_COL_UV_SHADER_PATH = "res/shaders/pos_col_uv.frag";

typedef void (APIENTRYP GLGetObjectivHandler)(GLuint object, GLenum pname, GLint* params);
typedef void (APIENTRYP GLGetObjectInfoLogHandler)(GLuint object, GLsizei maxLength, GLsizei* length, GLchar* infoLog);
typedef void (APIENTRYP GLDeleteObjectHandler)(GLuint object);

class Shader
{
private:
	unsigned int m_RendererID;
	std::unordered_map<std::string, GLint> m_UniformLocationCache;
public:
	Shader(const std::string& vertfilepath, const std::string& fragfilepath);
	~Shader();

	void Use() const;
	static void Unuse();

	void SetUniform1i(const std::string& name, int value);
	void SetUniform1f(const std::string& name, float value);
	void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
	void SetUniformMatrix4fv(const std::string& name, const glm::mat4& matrix);
private:
	GLint GetUniformLocation(const std::string& name);

	static const char* GetShaderName(GLenum shaderType);
	static const std::string GetErrorMessage(GLenum GL_STATUS, GLenum shaderType);

	static GLboolean GLValidateObjectStatus(GLuint object, GLenum GL_STATUS, GLenum shaderType,
		GLGetObjectivHandler GLGetObjectiv, GLGetObjectInfoLogHandler GLGetObjectInfoLog, GLDeleteObjectHandler GLDeleteObject);

	static std::string ParseShader(const std::string& filepath);
	static GLuint CompileShader(GLenum shaderType, const std::string& source);
	static GLuint CreateShader(const std::string& vertexShader, const std::string& fragmentShader);
};
