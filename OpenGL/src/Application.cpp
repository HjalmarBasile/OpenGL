#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "scenes/SceneHelloImGui.h"
#include "scenes/SceneClearColor.h"
#include "scenes/SceneBasicSquare.h"
#include "scenes/SceneTexture2D.h"

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

	/* Initialize GLAD library */
	if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
		std::cout << "Failed to initialize OpenGL context" << std::endl;
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
		scene::AbstractScene* currentScene = nullptr;
		scene::SceneMenu* menu = new scene::SceneMenu(currentScene);
		ImVec2 menuPosition(0.0f, 0.0f);
		ImVec2 menuSize(120.0f, 150.0f);
		currentScene = menu;

		menu->RegisterScene<scene::SceneHelloImGui>(scene::SceneHelloImGui::name);
		menu->RegisterScene<scene::SceneClearColor>(scene::SceneClearColor::name, 0.2f, 0.3f, 0.8f, 1.0f);
		menu->RegisterScene<scene::SceneBasicSquare>(scene::SceneBasicSquare::name);
		menu->RegisterScene<scene::SceneTexture2D>(scene::SceneTexture2D::name, WINDOW_WIDTH, WINDOW_HEIGHT);

		/* Loop until the user closes the window */
		while (!glfwWindowShouldClose(window))
		{
			/* Start the Dear ImGui frame */
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();

			if (currentScene) {
				currentScene->OnUpdate();
				currentScene->OnRender();

				ImGui::SetNextWindowPos(menuPosition);
				ImGui::SetNextWindowSize(menuSize);

				ImGui::Begin(currentScene->GetName().c_str());
				if (currentScene != menu && ImGui::Button("Return to Menu")) {
					delete currentScene;
					currentScene = menu;
				} else {
					currentScene->OnImGuiRender();
				}
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

		if (menu != currentScene) {
			delete menu;
		}
		delete currentScene;
	}

	/* Cleanup */
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	/*
		glfwTerminate destroys the OpenGL context, so after that any call
		to glGetError would return an error. That is why we need to close
		the scope and trigger the index and vertex buffer destructors now,
		in case they were created in the scene.
		Otherwise we would get an infinite loop.
	*/
	glfwTerminate();

	return 0;
}
