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

	/* Check if the returned id is valid */
#ifdef _PR_DEBUG
	ASSERT_AND_BREAK(0 != m_RendererID)
#endif
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

void Shader::Unuse()
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

void Shader::SetUniform3f(const std::string& name, float v0, float v1, float v2)
{
	GLCheckErrorCall(glUniform3f(GetUniformLocation(name), v0, v1, v2));
}

void Shader::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3)
{
	/* Set uniform variable */
	GLCheckErrorCall(glUniform4f(GetUniformLocation(name), v0, v1, v2, v3));
}

void Shader::SetUniformMatrix4fv(const std::string& name, const glm::mat4& matrix)
{
	/* No need to transpose: both OpenGL and glm use column-major order for matrices */
	GLCheckErrorCall(glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, glm::value_ptr(matrix)));
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

/* Custom utility function */
const char* Shader::GetShaderName(GLenum shaderType)
{
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

const std::string Shader::GetErrorMessage(GLenum GL_STATUS, GLenum shaderType)
{
	switch (GL_STATUS) {
		case GL_LINK_STATUS:
			return "Failed to link shader program!";
		case GL_VALIDATE_STATUS:
			return "Program validation failed!";
		case GL_COMPILE_STATUS: {
			std::stringstream sserr;
			sserr << "Failed to compile " << GetShaderName(shaderType) << " shader!";
			return sserr.str();
		}
		default:
			return "Unknown error occurred!";
	}
}

GLboolean Shader::GLValidateObjectStatus(GLuint object, GLenum GL_STATUS, GLenum shaderType,
	GLGetObjectivHandler GLGetObjectiv, GLGetObjectInfoLogHandler GLGetObjectInfoLog, GLDeleteObjectHandler GLDeleteObject)
{
	/* Retrieve the object status */
	GLint objectStatus;
	GLCheckErrorCall(GLGetObjectiv(object, GL_STATUS, &objectStatus));

	/*
	 * Log the error if the status is not success,
	 * also delete the object
	 */
	if (GL_TRUE != objectStatus) {
		const GLsizei LOG_MAX_LENGTH = 512;
		char logMessage[LOG_MAX_LENGTH];

		/* Get the information log for the object */
		GLCheckErrorCall(GLGetObjectInfoLog(object, LOG_MAX_LENGTH, NULL, logMessage));

		/* Log the error message */
		std::cout << GetErrorMessage(GL_STATUS, shaderType) << "\n";
		std::cout << logMessage << std::endl;

		/* Delete the object from memory and invalidate its id */
		GLCheckErrorCall(GLDeleteObject(object));
	}

	return objectStatus;
}

std::string Shader::ParseShader(const std::string& filepath) {
	std::ifstream fstreamin;
	fstreamin.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	std::stringstream ssout;
	try {
		fstreamin.open(filepath);
		if (fstreamin.is_open()) {
			ssout << fstreamin.rdbuf();
			fstreamin.close();
		}
	} catch (const std::ifstream::failure& e) {
		std::cout << "Error while opening/reading/closing shader file " << filepath << " --> ";
		std::cout << e.what() << std::endl;
	}

	return ssout.str();
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
	GLboolean compilationResult = GLValidateObjectStatus(id, GL_COMPILE_STATUS, shaderType,
		glGetShaderiv, glGetShaderInfoLog, glDeleteShader);

	if (GL_TRUE != compilationResult) {
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
	ASSERT_AND_BREAK(0 != program)
	ASSERT_AND_BREAK(0 != vs)
	ASSERT_AND_BREAK(0 != fs)
#endif

	/* Attach both shaders to a single program */
	GLCheckErrorCall(glAttachShader(program, vs));
	GLCheckErrorCall(glAttachShader(program, fs));

	/* It's time to link! */
	GLCheckErrorCall(glLinkProgram(program));

	/* Detach before delete */
	GLCheckErrorCall(glDetachShader(program, vs));
	GLCheckErrorCall(glDetachShader(program, fs));

	/* We do not need intermediates binaries anymore */
	GLCheckErrorCall(glDeleteShader(vs));
	GLCheckErrorCall(glDeleteShader(fs));

	/* Retrieve the linking result */
	GLboolean linkingResult = GLValidateObjectStatus(program, GL_LINK_STATUS, 0,
		glGetProgramiv, glGetProgramInfoLog, glDeleteProgram);

	if (GL_TRUE != linkingResult) {
		/* Return an invalid id */
		return 0;
	}

	/* Validate the program to check if it can be executed */
	GLCheckErrorCall(glValidateProgram(program));

	/* Retrieve the validation result */
	GLboolean validationResult = GLValidateObjectStatus(program, GL_VALIDATE_STATUS, 0,
		glGetProgramiv, glGetProgramInfoLog, glDeleteProgram);

	if (GL_TRUE != validationResult) {
		/* Return an invalid id */
		return 0;
	}

	/* Return shader id */
	return program;
}
