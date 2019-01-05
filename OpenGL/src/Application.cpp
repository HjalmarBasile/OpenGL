/*
	MIT License

	Copyright (c) 2018-2019 Hjalmar Basile

	Permission is hereby granted, free of charge, to any person obtaining a copy
	of this software and associated documentation files (the "Software"), to deal
	in the Software without restriction, including without limitation the rights
	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
	copies of the Software, and to permit persons to whom the Software is
	furnished to do so, subject to the following conditions:

	The above copyright notice and this permission notice shall be included in all
	copies or substantial portions of the Software.

	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
	SOFTWARE.
*/
#include <iostream>
#include <mutex>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Camera.h"
#include "SceneHelloImGui.h"
#include "SceneClearColor.h"
#include "SceneHelloTriangle.h"
#include "SceneBasicSquare.h"
#include "SceneTexture2D.h"
#include "ScenePerspectiveProjection.h"
#include "SceneCamera.h"
#include "SceneLight.h"
#include "exercises/SceneTwoTriangles.h"
#include "exercises/SceneMixedTexture.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

void processUserInput(GLFWwindow* window, float deltaTime);
void FramebufferSizeCallback(GLFWwindow* window, int width, int height);
void CursorPosCallback(GLFWwindow* window, double xpos, double ypos);
void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset);

static constexpr int WINDOW_WIDTH = 720;
static constexpr int WINDOW_HEIGHT = 540;

Camera MainCamera(glm::vec3(0.0f, 0.0f, 10.0f), (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT);
std::mutex cameraMutex;
bool useMainCamera = false;

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
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	/* Set the callback to be invoked when the framebuffer of the specified window is resized */
	glfwSetFramebufferSizeCallback(window, FramebufferSizeCallback);

	/* Enable v-sync */
	glfwSwapInterval(1);

	/* Initialize GLAD library */
	if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
		std::cout << "Failed to initialize OpenGL context" << std::endl;
		glfwTerminate();
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
	GLint maxVertexAttribs;
	GLCheckErrorCall(glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &maxVertexAttribs));
	GLCheckErrorCall(std::cout << "OpenGl version: " << glGetString(GL_VERSION) << '\n');
	GLCheckErrorCall(std::cout << "GLSL version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << '\n');
	std::cout << "Max vertex attributes supported: " << maxVertexAttribs << '\n';
	std::cout << std::endl;

	{
		scene::AbstractScene* currentScene = nullptr;
		scene::SceneMenu* menu = new scene::SceneMenu(currentScene);
		ImVec2 menuPosition(0.0f, 0.0f);
		ImVec2 menuSize(125.0f, 200.0f);
		currentScene = menu;

		const float r = 0.2f, g = 0.3f, b = 0.8f, a = 1.0f;
		Camera* const pMainCamera = &MainCamera;
		bool* const pUseMainCamera = &useMainCamera;
		menu->RegisterScene<scene::SceneHelloImGui>(scene::SceneHelloImGui::name);
		menu->RegisterScene<scene::SceneClearColor>(scene::SceneClearColor::name, r, g, b, a);
		menu->RegisterScene<scene::SceneHelloTriangle>(scene::SceneHelloTriangle::name);
		menu->RegisterScene<scene::SceneTwoTriangles>(scene::SceneTwoTriangles::name);
		menu->RegisterScene<scene::SceneBasicSquare>(scene::SceneBasicSquare::name);
		menu->RegisterScene<scene::SceneTexture2D>(scene::SceneTexture2D::name, WINDOW_WIDTH, WINDOW_HEIGHT);
		menu->RegisterScene<scene::SceneMixedTexture>(scene::SceneMixedTexture::name);
		menu->RegisterScene<scene::ScenePerspectiveProjection>(scene::ScenePerspectiveProjection::name, WINDOW_WIDTH, WINDOW_HEIGHT);
		menu->RegisterScene<scene::SceneCamera>(scene::SceneCamera::name, pMainCamera, pUseMainCamera);
		menu->RegisterScene<scene::SceneLight>(scene::SceneLight::name, pMainCamera, pUseMainCamera);

		float deltaTime = 0.0f;
		float lastFrameTimestamp = (float)glfwGetTime();

		/* Loop until the user closes the window */
		while (!glfwWindowShouldClose(window))
		{
			float currentFrameTimestamp = (float)glfwGetTime();
			deltaTime = currentFrameTimestamp - lastFrameTimestamp;
			lastFrameTimestamp = currentFrameTimestamp;

			/* React to user input */
			processUserInput(window, deltaTime);

			/* Start the Dear ImGui frame */
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();

			/* Take input from mouse */
			glfwSetCursorPosCallback(window, CursorPosCallback);
			glfwSetScrollCallback(window, ScrollCallback);

			if (currentScene) {
				currentScene->OnUpdate(deltaTime);
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

void processUserInput(GLFWwindow* window, float deltaTime) {
	if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_ESCAPE)) {
		glfwSetWindowShouldClose(window, true);
	}

	if (useMainCamera) {
		cameraMutex.lock();
		if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_W)) {
			MainCamera.ProcessKeyboard(Camera::MovementDirection::FORWARD, deltaTime);
		}
		if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_S)) {
			MainCamera.ProcessKeyboard(Camera::MovementDirection::BACKWARD, deltaTime);
		}
		if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_D)) {
			MainCamera.ProcessKeyboard(Camera::MovementDirection::RIGHT, deltaTime);
		}
		if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_A)) {
			MainCamera.ProcessKeyboard(Camera::MovementDirection::LEFT, deltaTime);
		}
		cameraMutex.unlock();
	}
}

void FramebufferSizeCallback(GLFWwindow* window, int width, int height) {
	GLCheckErrorCall(glViewport(0, 0, width, height));
}

void CursorPosCallback(GLFWwindow* window, double xpos, double ypos)
{
	if (useMainCamera) {
		float fxpos = (float)xpos;
		float fypos = (float)ypos;
		static float lastMouseX = fxpos;
		static float lastMouseY = fypos;

		float xoffset = fxpos - lastMouseX;
		float yoffset = -fypos + lastMouseY; /* Y screen coordinates returned by glfw are reversed */
		lastMouseX = fxpos;
		lastMouseY = fypos;

		cameraMutex.lock();
		MainCamera.ProcessMouseMovement(xoffset, yoffset);
		cameraMutex.unlock();
	}
}

void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
	ImGui_ImplGlfw_ScrollCallback(window, xoffset, yoffset);

	if (useMainCamera) {
		cameraMutex.lock();
		MainCamera.ProcessMouseScroll((float)yoffset);
		cameraMutex.unlock();
	}
}
