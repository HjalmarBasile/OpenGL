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
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

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

	/* Setup Dear ImGui */
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.IniFilename = NULL;
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 130");
	ImGui::StyleColorsDark();

	/* Just to know... */
	GLCheckErrorCall(std::cout << "OpenGl version: " << glGetString(GL_VERSION) << std::endl);
	GLCheckErrorCall(std::cout << "GLSL version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl);

	{
		const unsigned int POSITIONS_SIZE = 4 * 4;
		const GLint VERTEX_SIZE = 2;
		const GLint UV_SIZE = 2;
		/* Square vertices */
		float positions[POSITIONS_SIZE] = {
			/* vertices */		/* UV coordinates */
			-180.0f, -135.0f,	0.0f, 0.0f, // 0
			 180.0f, -135.0f,	1.0f, 0.0f, // 1
			 180.0f,  135.0f,	1.0f, 1.0f, // 2
			-180.0f,  135.0f,	0.0f, 1.0f  // 3
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

		/* Let's define the model matrix */
		glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));

		/* Moving the camera to the right is equivalent to move the model to the left */
		float cameraTranslateX = 0.0f; /* For now let's keep it where it is anyway */
		glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(-cameraTranslateX, 0.0f, 0.0f));

		/* Orthographic projection matrix (window aspect ratio) */
		/* See math folder in the solution dir for explanation */
		glm::mat4 proj = glm::ortho<float>(0.0f, WINDOW_WIDTH, 0.0f, WINDOW_HEIGHT);

		/* Finally we build our MVP matrix */
		glm::mat4 MVP = proj * view * model;

		/*
			Unbind all: this will prove we don't need to bind the array buffer
			and specify its attributes anymore: it's all inside the vertex array object!
		*/
		va.Unbind();
		vb.Unbind();
		ib.Unbind();
		shader.Unuse();

		/* We will update these vectors at runtime via ImGui */
		glm::vec3 modelTranslationA = glm::vec3(0.0f, 0.0f, 0.0f);
		glm::vec3 modelTranslationB = glm::vec3(0.0f, 0.0f, 0.0f);

		Renderer renderer;

		/* Do not show ImGui demo window at startup */
		bool show_demo_window = false;

		/* Loop until the user closes the window */
		while (!glfwWindowShouldClose(window))
		{
			/* Render here */
			renderer.Clear();

			/* Start the Dear ImGui frame */
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();

			shader.Use();

			/* Update MVP every frame */
			{
				model = glm::translate(glm::mat4(1.0f), modelTranslationA);
				MVP = proj * view * model;
				shader.SetUniformMatrix4fv("u_MVP", MVP);
				renderer.Draw(va, ib, shader);
			}

			{
				model = glm::translate(glm::mat4(1.0f), modelTranslationB);
				MVP = proj * view * model;
				shader.SetUniformMatrix4fv("u_MVP", MVP);
				renderer.Draw(va, ib, shader);
			}

			/* Show the ImGui big demo window */
			if (show_demo_window) {
				ImGui::ShowDemoWindow(&show_demo_window);
			}

			/* Show a simple ImGui window. We use a Begin/End pair to create a named window. */
			{
				ImGui::Begin("Hello, ImGui!");
				ImGui::Checkbox("Demo Window", &show_demo_window);

				ImGui::Text("Use the slider to move the model around.");
				ImGui::SliderFloat3("Model A Translation", reinterpret_cast<float*>(&modelTranslationA.x), 0.0f, WINDOW_WIDTH);
				ImGui::SliderFloat3("Model B Translation", reinterpret_cast<float*>(&modelTranslationB.x), 0.0f, WINDOW_WIDTH);
				ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
				ImGui::End();
			}

			/* ImGui Rendering */
			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

			/* Swap front and back buffers */
			glfwSwapBuffers(window);

			/* Poll for and process events */
			glfwPollEvents();
		}
	}

	/* Cleanup */
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	/*
		glfwTerminate destroys the OpenGL context, so after that any call
		to glGetError would return an error. That is why we need to close
		the scope and trigger the index and vertex buffer destructors now.
		Otherwise we would get an infinite loop.
	*/
	glfwTerminate();

	return 0;
}
