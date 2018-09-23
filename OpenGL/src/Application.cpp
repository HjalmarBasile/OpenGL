#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Renderer.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Shader.h"

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
	GLCheckErrorCall(std::cout << "OpenGl version: " << glGetString(GL_VERSION) << std::endl);
	GLCheckErrorCall(std::cout << "GLSL version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl);

	{
		const unsigned int POSITIONS_SIZE = 8;
		const GLint VERTEX_SIZE = 2;
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

		/* Generate vertex array object */
		VertexArray va;

		/* Create and bind vertex buffer */
		VertexBuffer vb(positions, POSITIONS_SIZE * sizeof(float));

		/* Define a vertex buffer layout */
		VertexBufferLayout layout;
		layout.Push<float>(VERTEX_SIZE);

		/* va is defined by the pair (vb, layout) */
		va.AddBuffer(vb, layout);

		/* Create and bind index buffer */
		IndexBuffer ib(indices, INDICES_SIZE);

		/* Create shader program */
		Shader shader(VERTEX_BASIC_SHADER_PATH, FRAGMENT_BASIC_SHADER_PATH);
		shader.Use();

		float red = 0.0f;
		float rincr = 0.01f;

		/*
			Unbind all: this will prove we don't need to bind the array buffer
			and specify its attributes anymore: it's all inside the vertex array object!
		*/
		va.Unbind();
		vb.Unbind();
		ib.Unbind();
		shader.Unuse();

		/* Loop until the user closes the window */
		while (!glfwWindowShouldClose(window))
		{
			/* Render here */
			GLCheckErrorCall(glClear(GL_COLOR_BUFFER_BIT));

			if (red > 1.0f) {
				red = 2.0f - red;
				rincr = -0.01f;
			} else if (red < 0.0f) {
				red = -red;
				rincr = 0.01f;
			}

			/* Bring back what we really need */
			va.Bind();
			ib.Bind();
			shader.Use();

			/* Set uniform variable */
			shader.SetUniform4f("u_Color", red, 0.3f, 0.8f, 1.0f);

			/* From the buffers setup above, OpenGL will know what to do */
			GLCheckErrorCall(glDrawElements(GL_TRIANGLES, INDICES_SIZE, GL_UNSIGNED_INT, nullptr));

			red += rincr;

			/* Swap front and back buffers */
			glfwSwapBuffers(window);

			/* Poll for and process events */
			glfwPollEvents();
		}
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
