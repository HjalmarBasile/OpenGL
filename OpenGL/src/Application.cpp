#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define VERTEX_BASIC_SHADER_PATH "res/shaders/VertexBasic.shader"
#define FRAGMENT_BASIC_SHADER_PATH "res/shaders/FragmentBasic.shader"

#ifdef _PR_DEBUG
	#define ASSERT_AND_BREAK(x) if(!(x)) __debugbreak();

	#define GLCheckErrorCall(glFun) GLClearError();\
		glFun;\
		ASSERT_AND_BREAK(GLLogCall(#glFun, __FILE__, __LINE__))

	static void GLClearError()
	{
		while (glGetError() != GL_NO_ERROR) {
			/* Just clearing the flags */
		}
	}

	static bool GLLogCall(const char* function, const char* file, int line)
	{
		GLenum error;
		while ((error = glGetError()) != GL_NO_ERROR) {
			std::cout << "[OpenGL Error] (" << error << "):" << function << " in " << file << ":" << line << std::endl;
			return false;
		}
		return true;
	}

#else
	#define GLCheckErrorCall(glFun) glFun
#endif

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

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(640, 480, "Welcome to OpenGL!", NULL, NULL);
	if (!window) {
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	/* Initialize glew library */
	GLenum err = glewInit();
	if (GLEW_OK != err) {
		/* Problem: glewInit failed, something is seriously wrong */
		std::cout << "Error: " << glewGetErrorString(err) << std::endl;
		return -1;
	}

	/* Just to know... */
	std::cout << glGetString(GL_VERSION) << std::endl;

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

	/* 1. Generate Vertex Buffer with modern OpenGL */
	GLuint buffer;

	/* 1.1 Retrieve an Id for the buffer */
	GLCheckErrorCall(glGenBuffers(1, &buffer));

	/* 1.2 Binds GL_ARRAY_BUFFER to buffer */
	GLCheckErrorCall(glBindBuffer(GL_ARRAY_BUFFER, buffer));

	/* 1.3 Specify that the actual data is contained in positions array and that it must be drawn */
	GLCheckErrorCall(glBufferData(GL_ARRAY_BUFFER, POSITIONS_SIZE * sizeof(float), positions, GL_STATIC_DRAW));

	/* 1.4 Set the index of the coordinates attribute in the positions array */
	GLuint coord_attrib_index = 0;

	/* 1.5 Define an attribute for the position array, in our case the points coordinates */
	GLCheckErrorCall(glVertexAttribPointer(coord_attrib_index, VERTEX_SIZE, GL_FLOAT, GL_FALSE, VERTEX_SIZE * sizeof(float), 0));

	/* 1.6 Enable the coordinates attribute */
	GLCheckErrorCall(glEnableVertexAttribArray(coord_attrib_index));

	/* 2. Generate Index Buffer object (similar to above step) */
	GLuint index_buffer;
	GLCheckErrorCall(glGenBuffers(1, &index_buffer));
	GLCheckErrorCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer));
	GLCheckErrorCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, INDICES_SIZE * sizeof(unsigned int), indices, GL_STATIC_DRAW));

	/* Parse vertex shader source code */
	std::string vertexShader = ParseShader(VERTEX_BASIC_SHADER_PATH);

	/* Parse fragment shader source code */
	std::string fragmentShader = ParseShader(FRAGMENT_BASIC_SHADER_PATH);

	/* Create and compile the shader */
	GLuint shader = CreateShader(vertexShader, fragmentShader);

	/* 3. Install the shader as part of the current rendering state */
	GLCheckErrorCall(glUseProgram(shader));

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT);

		/* From the buffers setup above, OpenGL will know what to do */
		GLCheckErrorCall(glDrawElements(GL_TRIANGLES, INDICES_SIZE, GL_UNSIGNED_INT, nullptr));

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	/* Delete the shader program */
	GLCheckErrorCall(glDeleteProgram(shader));

	glfwTerminate();

	return 0;
}
