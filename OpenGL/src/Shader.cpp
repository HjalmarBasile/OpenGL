#include "Shader.h"

#include <iostream>
#include <sstream>
#include <fstream>

#include "Renderer.h"

Shader::Shader(const std::string& vertfilepath, const std::string& fragfilepath)
{
	/* Parse vertex shader source code */
	std::string vertexShader = Shader::ParseShader(vertfilepath);

	/* Parse fragment shader source code */
	std::string fragmentShader = Shader::ParseShader(fragfilepath);

	/* Create and compile the shader */
	m_RendererID = Shader::CreateShader(vertexShader, fragmentShader);
}

Shader::~Shader()
{
	GLCheckErrorCall(glDeleteProgram(m_RendererID));
}

void Shader::Use() const
{
	/* Install the shader as part of the current rendering state */
	GLCheckErrorCall(glUseProgram(m_RendererID));
}

void Shader::Unuse() const
{
	GLCheckErrorCall(glUseProgram(0));
}

void Shader::SetUniform1i(const std::string& name, int value)
{
	GLCheckErrorCall(glUniform1i(GetUniformLocation(name), value));
}

void Shader::SetUniform1f(const std::string& name, float value)
{
	GLCheckErrorCall(glUniform1f(GetUniformLocation(name), value));
}

void Shader::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3)
{
	/* Set uniform variable */
	GLCheckErrorCall(glUniform4f(GetUniformLocation(name), v0, v1, v2, v3));
}

void Shader::SetUniformMatrix4fv(const std::string& name, const glm::mat4& matrix)
{
	/* No need to transpose: both OpenGL and glm use column-major order for matrices */
	GLCheckErrorCall(glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &matrix[0][0]));
}

GLint Shader::GetUniformLocation(const std::string& name)
{
	/* Search in the cache first */
	if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end()) {
		return m_UniformLocationCache[name];
	}

	/* Get the id of the uniform variable (can be done only after linking) */
	GLCheckErrorCall(GLint uniformLocation = glGetUniformLocation(m_RendererID, name.c_str()));
	if (-1 == uniformLocation) {
		std::cout << "Error while getting " << name << " uniform location" << std::endl;
	}
	m_UniformLocationCache[name] = uniformLocation;

	return uniformLocation;
}

std::string Shader::ParseShader(const std::string& filepath) {
	std::ifstream fstreamin(filepath);

	std::stringstream ssout;
	if (fstreamin.is_open()) {
		std::string line;
		while (getline(fstreamin, line)) {
			ssout << line << '\n';
		}
		fstreamin.close();
	}

	return ssout.str();
}

/* Custom utility function */
const char* Shader::GetShaderName(GLenum shaderType) {

	switch (shaderType) {
	case GL_VERTEX_SHADER:
		return "vertex";
	case GL_GEOMETRY_SHADER:
		return "geometry";
	case GL_FRAGMENT_SHADER:
		return "fragment";
	default:
		return "unknown";
	}
}

/* Take the source code of the shader and compile it */
GLuint Shader::CompileShader(GLenum shaderType, const std::string& source) {

	/* Create an empty shader object and return its id */
	GLCheckErrorCall(GLuint id = glCreateShader(shaderType));

	/* OpenGL needs the source code as a c-string */
	const char* src = source.c_str();

	/* Copies the source code into the shader object */
	GLCheckErrorCall(glShaderSource(id, 1, &src, NULL));

	/* Compile the shader */
	GLCheckErrorCall(glCompileShader(id));

	/* Retrieve the compilation result */
	GLint compilationResult;
	GLCheckErrorCall(glGetShaderiv(id, GL_COMPILE_STATUS, &compilationResult));

	/*
	 * Log the error if the compilation was not successful,
	 * also delete the shader
	 */
	if (GL_TRUE != compilationResult) {
		GLint logLength;
		GLCheckErrorCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &logLength));

		/*
		 * Allocate the message on the stack, free will happen automatically
		 * when the control returns to the caller.
		 *
		 * Note: there is no full consensus on the usage of this function:
		 * it will create undefined behaviour in case of stack overflow!
		 */
		char* logMessage = (char*)alloca(logLength * sizeof(char));

		/* Get the information log for the shader object */
		GLCheckErrorCall(glGetShaderInfoLog(id, logLength, &logLength, logMessage));

		/* Log the error message */
		std::cout << "Failed to compile " << Shader::GetShaderName(shaderType) << " shader!\n";
		std::cout << logMessage << std::endl;

		/* Delete the shader from memory and invalidate its id */
		GLCheckErrorCall(glDeleteShader(id));

		/* Return an invalid id */
		return 0;
	}

	return id;
}

/* The inputs are the source code, we are gonna compile them here */
GLuint Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader) {

	/* Create an empty program object and return its id */
	GLCheckErrorCall(GLuint program = glCreateProgram());

	/* Compile the vertex shader and return its id */
	GLuint vs = CompileShader(GL_VERTEX_SHADER, vertexShader);

	/* Compile the fragment shader and return its id */
	GLuint fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

	/* Check if the returned ids are valid */
#ifdef _PR_DEBUG
	ASSERT_AND_BREAK(0 != vs)
	ASSERT_AND_BREAK(0 != fs)
#endif

	/* Attach both shaders to a single program */
	GLCheckErrorCall(glAttachShader(program, vs));
	GLCheckErrorCall(glAttachShader(program, fs));

	/* It's time to link! */
	GLCheckErrorCall(glLinkProgram(program));

	/* Validate the program to check if it can be executed */
	GLCheckErrorCall(glValidateProgram(program));

	/* Detach before delete */
	GLCheckErrorCall(glDetachShader(program, vs));
	GLCheckErrorCall(glDetachShader(program, fs));

	/* We do not need intermediates binaries anymore */
	GLCheckErrorCall(glDeleteShader(vs));
	GLCheckErrorCall(glDeleteShader(fs));

	/* Return shader id */
	return program;
}
