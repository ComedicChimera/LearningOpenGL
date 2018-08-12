#pragma once

#include <glm/glm.hpp>
#include <GLFW/glfw3.h>

class Camera
{
	float m_DeltaTime, m_LastFrame;

	float m_Pitch, m_Yaw;
	float m_LastX, m_LastY;
	bool m_FirstMouse;

	glm::vec3 m_CameraPos, m_CameraUp, m_CameraFront;

public:
	Camera(glm::vec3 pos, float startX, float startY);

	glm::mat4 getViewMatrix();
	void update(double x, double y);
	void updatePosition(GLFWwindow *window);
};

