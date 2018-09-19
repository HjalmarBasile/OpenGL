#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"

#define VERTEX_BASIC_SHADER_PATH "res/shaders/VertexBasic.shader"
#define FRAGMENT_BASIC_SHADER_PATH "res/shaders/FragmentBasic.shader"

static std::string ParseShader(const std::string& filepath) {
	std::ifstream fstreamin(filepath);

	std::stringstream ssout;
	if(fstreamin.is_open()) {
		std::string line;
		while (getline(fstreamin, line)) {
			ssout << line << '\n';
		}
		fstreamin.close();
	}

	return ssout.str();
}

/* Custom utility function */
static const char* getShaderName(GLenum shaderType) {

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
static GLuint CompileShader(GLenum shaderType, const std::string& source) {

	/* Create an empty shader object and return its id */
	GLuint id = glCreateShader(shaderType);

	/* OpenGL needs the source code as a c-string */
	const char* src = source.c_str();

	/* Copies the source code into the shader object */
	glShaderSource(id, 1, &src, NULL);

	/* Compile the shader */
	glCompileShader(id);

	/* Retrieve the compilation result */
	GLint compilationResult;
	glGetShaderiv(id, GL_COMPILE_STATUS, &compilationResult);

	/*
	 * Log the error if the compilation was not successful,
	 * also delete the shader
	 */
	if (GL_TRUE != compilationResult) {
		GLint logLength;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &logLength);

		/*
		 * Allocate the message on the stack, free will happen automatically
		 * when the control returns to the caller.
		 *
		 * Note: there is no full consensus on the usage of this function:
		 * it will create undefined behaviour in case of stack overflow!
		 */
		char* logMessage = (char*)alloca(logLength * sizeof(char));

		/* Get the information log for the shader object */
		glGetShaderInfoLog(id, logLength, &logLength, logMessage);

		/* Log the error message */
		std::cout << "Failed to compile " << getShaderName(shaderType) << " shader!\n";
		std::cout << logMessage << std::endl;

		/* Delete the shader from memory and invalidate its id */
		glDeleteShader(id);

		/* Return an invalid id */
		return 0;
	}

	return id;
}

/* The inputs are the source code, we are gonna compile them here */
static GLuint CreateShader(const std::string& vertexShader, const std::string& fragmentShader) {

	/* Create an empty program object and return its id */
	GLuint program = glCreateProgram();

	/* Compile the vertex shader and return its id */
	GLuint vs = CompileShader(GL_VERTEX_SHADER, vertexShader);

	/* Compile the fragment shader and return its id */
	GLuint fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

	// TODO: check if the returned id are valid

	/* Attach both shaders to a single program */
	glAttachShader(program, vs);
	glAttachShader(program, fs);

	/* It's time to link! */
	glLinkProgram(program);

	// TODO: detach shader?

	/* Validate the program to check if it can be executed */
	glValidateProgram(program);

	/* We do not need intermediates binaries anymore */
	glDeleteProgram(vs);
	glDeleteProgram(fs);

	/* Return shader id */
	return program;
}

int main() {
	GLFWwindow* window;

	/* Initialize glfw library */
	if (!glfwInit()) {
		return -1;
	}

	/* Use OpenGL core profile */
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(640, 480, "Welcome to OpenGL!", NULL, NULL);
	if (!window) {
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	/* Enable v-sync */
	glfwSwapInterval(1);

	/* Initialize glew library */
	GLenum err = glewInit();
	if (GLEW_OK != err) {
		/* Problem: glewInit failed, something is seriously wrong */
		std::cout << "Error: " << glewGetErrorString(err) << std::endl;
		return -1;
	}

	/* Just to know... */
	std::cout << "OpenGl version: " << glGetString(GL_VERSION) << std::endl;
	std::cout << "GLSL version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;

	{
		const unsigned int POSITIONS_SIZE = 8;
		const unsigned int VERTEX_SIZE = 2;
		/* Square vertices */
		float positions[POSITIONS_SIZE] = {
			-0.5f, -0.5f,
			 0.5f, -0.5f,
			 0.5f,  0.5f,
			-0.5f,  0.5f
		};

		const unsigned int INDICES_SIZE = 6;
		/* Index buffer */
		unsigned int indices[INDICES_SIZE] = {
			0, 1, 2,
			2, 3, 0
		};

		/* 0. Generate Vertex Array Object */
		GLuint vao;

		/* 0.1 Retrieve an Id for the vao */
		GLCheckErrorCall(glGenVertexArrays(1, &vao));

		/* 0.2 Bind the vertex array vao */
		GLCheckErrorCall(glBindVertexArray(vao));

		/* 1. Create and bind vertex buffer */
		VertexBuffer vb(positions, POSITIONS_SIZE * sizeof(float));

		/* 1.1 Set the index of the coordinates attribute in the positions array */
		GLuint coord_attrib_index = 0;

		/* 1.2 Enable the new coordinates attribute */
		GLCheckErrorCall(glEnableVertexAttribArray(coord_attrib_index));

		/* 1.3 Define the attribute for the position array, in our case the points coordinates */
		GLCheckErrorCall(glVertexAttribPointer(coord_attrib_index, VERTEX_SIZE, GL_FLOAT, GL_FALSE, VERTEX_SIZE * sizeof(float), 0));

		/* 2. Create and bind index buffer */
		IndexBuffer ib(indices, INDICES_SIZE);

		/* Parse vertex shader source code */
		std::string vertexShader = ParseShader(VERTEX_BASIC_SHADER_PATH);

		/* Parse fragment shader source code */
		std::string fragmentShader = ParseShader(FRAGMENT_BASIC_SHADER_PATH);

		/* Create and compile the shader */
		GLuint shader = CreateShader(vertexShader, fragmentShader);

		/* 3. Install the shader as part of the current rendering state */
		GLCheckErrorCall(glUseProgram(shader));

		/* 4. Inject values into shader program */
		/* 4.1 Get the id of a uniform variable (can be done only after linking) */
		GLint uniformColorLocation = glGetUniformLocation(shader, "u_Color");
		if (-1 == uniformColorLocation) {
			std::cout << "Error while getting u_Color location" << std::endl;
			return -1;
		}

		float red = 0.0f;
		float rincr = 0.01f;

		/*
			Unbind all: this will prove we don't need to bind the array buffer
			and specify its attributes anymore: it's all inside the vertex array object!
		*/
		GLCheckErrorCall(glBindVertexArray(0));
		GLCheckErrorCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
		GLCheckErrorCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
		GLCheckErrorCall(glUseProgram(0));

		/* Loop until the user closes the window */
		while (!glfwWindowShouldClose(window))
		{
			/* Render here */
			glClear(GL_COLOR_BUFFER_BIT);

			if (red > 1.0f) {
				red = 2.0f - red;
				rincr = -0.01f;
			} else if (red < 0.0f) {
				red = -red;
				rincr = 0.01f;
			}

			/* Bring back what we really need */
			GLCheckErrorCall(glBindVertexArray(vao));
			ib.Bind();
			GLCheckErrorCall(glUseProgram(shader));

			/* 4.2 Set uniform variable */
			GLCheckErrorCall(glUniform4f(uniformColorLocation, red, 0.3f, 0.8f, 1.0f));

			/* From the buffers setup above, OpenGL will know what to do */
			GLCheckErrorCall(glDrawElements(GL_TRIANGLES, INDICES_SIZE, GL_UNSIGNED_INT, nullptr));

			red += rincr;

			/* Swap front and back buffers */
			glfwSwapBuffers(window);

			/* Poll for and process events */
			glfwPollEvents();
		}

		/* Delete the shader program */
		GLCheckErrorCall(glDeleteProgram(shader));
	}
	/*
		glfwTerminate destroys the OpenGL context, so after that any call
		to glGetError would return an error. That is why we need to close
		the scope and trigger the index and vertex buffer destructors now.
		Otherwise we would get an infinite loop.
	*/
	glfwTerminate();

	return 0;
}
