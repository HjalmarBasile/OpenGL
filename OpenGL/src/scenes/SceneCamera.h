#pragma once

#include <memory>
#include "Scene.h"
#include "primitives/Cube.h"
#include <GLFW/glfw3.h>

namespace scene {

	typedef struct EulerAngles {
		float pitch;
		float yaw;
		float roll;
	} EulerAngles;

	typedef struct CameraContext {
		float fov;
		EulerAngles offset;
	} CameraContext;

	class SceneCamera : public AbstractScene
	{
	public:
		static constexpr const char* name = "Camera";
		SceneCamera(GLFWwindow* window, int windowWidth, int windowHeight);
		~SceneCamera();

		std::string GetName() const override;

		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;

	private:
		static void mouseCallback(GLFWwindow* window, double xpos, double ypos);
		static void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);

		void processUserInput(float deltaTime);

		GLFWwindow* m_Window;
		const float m_ASPECT_RATIO;
		static constexpr int TOTAL_CUBES = 13;

		std::unique_ptr<Cube> cube;

		const glm::vec3 m_CubesPositions[TOTAL_CUBES] = {
			/* H */
			glm::vec3(-3.0f, -1.0f, 0.0f),
			glm::vec3(-3.0f,  0.0f, 0.0f),
			glm::vec3(-3.0f,  1.0f, 0.0f),
			glm::vec3(-2.0f,  0.0f, 0.0f),
			glm::vec3(-1.0f, -1.0f, 0.0f),
			glm::vec3(-1.0f,  0.0f, 0.0f),
			glm::vec3(-1.0f,  1.0f, 0.0f),

			/* J */
			glm::vec3( 1.0f, -1.0f, 0.0f),
			glm::vec3( 1.0f,  1.0f, 0.0f),
			glm::vec3( 2.0f, -1.0f, 0.0f),
			glm::vec3( 2.0f,  0.0f, 0.0f),
			glm::vec3( 2.0f,  1.0f, 0.0f),
			glm::vec3( 3.0f,  1.0f, 0.0f)
		};

		glm::mat4 m_Model;
		glm::mat4 m_View;
		glm::mat4 m_Proj;
		glm::mat4 m_MVP;

		glm::vec3 m_Eye;
		glm::vec3 m_CameraFront;
		glm::vec3 m_Center;
		glm::vec3 m_WorldUp;

		float m_FOV;
		float m_CameraSpeed;
		float m_Pitch;
		float m_Yaw;
	};

}
