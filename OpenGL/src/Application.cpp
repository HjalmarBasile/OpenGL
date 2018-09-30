#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Renderer.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "Texture.h"

#include "glm/gtc/matrix_transform.hpp"

#define WINDOW_WIDTH 720
#define WINDOW_HEIGHT 540

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
	window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Welcome to OpenGL!", NULL, NULL);
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
		const unsigned int POSITIONS_SIZE = 4 * 4;
		const GLint VERTEX_SIZE = 2;
		const GLint UV_SIZE = 2;
		/* Square vertices */
		float positions[POSITIONS_SIZE] = {
			/* vertices */	/* UV coordinates */
			180.0f, 135.0f,	0.0f, 0.0f, // 0
			540.0f, 135.0f,	1.0f, 0.0f, // 1
			540.0f, 405.0f,	1.0f, 1.0f, // 2
			180.0f, 405.0f,	0.0f, 1.0f  // 3
		};

		const unsigned int INDICES_SIZE = 6;
		/* Index buffer */
		unsigned int indices[INDICES_SIZE] = {
			0, 1, 2,
			2, 3, 0
		};

		/* Enable blending */
		GLCheckErrorCall(glEnable(GL_BLEND));
		/* Transparency implementation */
		GLCheckErrorCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

		/* Generate vertex array object */
		VertexArray va;

		/* Create and bind vertex buffer */
		VertexBuffer vb(positions, POSITIONS_SIZE * sizeof(float));

		/* Define a vertex buffer layout */
		VertexBufferLayout layout;
		layout.Push<float>(VERTEX_SIZE);
		layout.Push<float>(UV_SIZE);

		/* va is defined by the pair (vb, layout) */
		va.AddBuffer(vb, layout);

		/* Create and bind index buffer */
		IndexBuffer ib(indices, INDICES_SIZE);

		/* Create shader program */
		Shader shader(VERTEX_TEXTURE_2D_SHADER_PATH, FRAGMENT_TEXTURE_2D_SHADER_PATH);
		shader.Use();

		/* Load texture to memory */
		Texture texture(DICE_TEXTURE_PATH);
		const unsigned int slot = 0;
		texture.Bind(slot);
		shader.SetUniform1i("u_Texture", slot);

		/* Let's translate the model */
		float modelTranslateX = 150.0f;
		float modelTranslateY = 75.0f;
		glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(modelTranslateX, modelTranslateY, 0.0f));

		/* Moving the camera to the right is equivalent to move the model to the left */
		float cameraTranslateX = 100.0f;
		glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(-cameraTranslateX, 0.0f, 0.0f));

		/* Orthographic projection matrix (window aspect ratio) */
		/* See math folder in the solution dir for explanation */
		glm::mat4 proj = glm::ortho<float>(0.0f, WINDOW_WIDTH, 0.0f, WINDOW_HEIGHT);

		/* Finally we build our MVP matrix */
		glm::mat4 MVP = proj * view * model;
		shader.SetUniformMatrix4fv("u_MVP", MVP);

		/*
			Unbind all: this will prove we don't need to bind the array buffer
			and specify its attributes anymore: it's all inside the vertex array object!
		*/
		va.Unbind();
		vb.Unbind();
		ib.Unbind();
		shader.Unuse();

		Renderer renderer;

		/* Loop until the user closes the window */
		while (!glfwWindowShouldClose(window))
		{
			/* Render here */
			renderer.Clear();

			shader.Use();

			renderer.Draw(va, ib, shader);

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
