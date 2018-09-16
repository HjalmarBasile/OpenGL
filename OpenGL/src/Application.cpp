#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

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

	const unsigned int POSITIONS_SIZE = 6;
	const unsigned int VERTEX_SIZE = 2;
	/* Triangle vertices */
	float positions[POSITIONS_SIZE] = {
		-0.5f, -0.5f,
		 0.0f,  0.5f,
		 0.5f, -0.5f
	};

	/* Generate Vertex Buffer with modern OpenGL */
	GLuint buffer;

	/* 1. Retrieve an Id for the buffer */
	glGenBuffers(1, &buffer);

	/* 2. Binds GL_ARRAY_BUFFER to buffer */
	glBindBuffer(GL_ARRAY_BUFFER, buffer);

	/* 3. Specify that the actual data is contained in positions array and that it must be drawn*/
	glBufferData(GL_ARRAY_BUFFER, POSITIONS_SIZE * sizeof(float), positions, GL_STATIC_DRAW);

	/* 4. Set the index of the coordinates attribute in the positions array */
	GLuint coord_attrib_index = 0;

	/* 5. Define an attribute for the position array, in our case the points coordinates */
	glVertexAttribPointer(coord_attrib_index, VERTEX_SIZE, GL_FLOAT, GL_FALSE, VERTEX_SIZE * sizeof(float), 0);

	/* 6. Enable the coordinates attribute */
	glEnableVertexAttribArray(coord_attrib_index);

	/* Write vertex shader source code */
	std::string vertexShader =
		"#version 330 core\n"
		"\n"
		"layout(location = 0) in vec4 position;"
		"\n"
		"void main()\n"
		"{\n"
		"	gl_Position = position;\n"
		"}\n";

	/* Write fragment shader source code */
	std::string fragmentShader =
		"#version 330 core\n"
		"\n"
		"out vec4 color;"
		"\n"
		"void main()\n"
		"{\n"
		"	color = vec4(1.0, 0.0, 0.0, 1.0);\n"
		"}\n";

	/* Create and compile the shader */
	GLuint shader = CreateShader(vertexShader, fragmentShader);

	/* 7. Install the shader as part of the current rendering state */
	glUseProgram(shader);

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT);

		/* From the vertex buffer setup above, OpenGL will know what to do */
		glDrawArrays(GL_TRIANGLES, 0, POSITIONS_SIZE / 2);

		// Not for now: in case we are using index buffers
		// glDrawElements(GL_TRIANGLES, POSITION_SIZE / 2, GL_UNSIGNED_BYTE, /* TODO */);

		/* Legacy OpenGL
		// Draw a triangle
		glBegin(GL_TRIANGLES);
		// Coordinates range from -1 to 1
		glVertex2f(-0.5f, -0.5f);
		glVertex2f( 0.0f,  0.5f);
		glVertex2f( 0.5f, -0.5f);
		glEnd();
		*/

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	/* Delete the shader program */
	glDeleteProgram(shader);

	glfwTerminate();

	return 0;
}
