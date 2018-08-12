#include "Camera.h"

#include <glm/gtc/matrix_transform.hpp>

Camera::Camera(glm::vec3 pos, float startX, float startY)
	: m_CameraUp(glm::vec3(0.0f, 1.0f, 0.0f))
	, m_CameraFront(glm::vec3(0.0f, 0.0f, -1.0f))
{
	m_Pitch, m_Yaw = 0.0f, -90.0f;

	m_FirstMouse = true;

	m_CameraPos = pos;
	m_LastX, m_LastY = startX, startY;

	m_DeltaTime, m_LastFrame = 0, 0;
}


glm::mat4 Camera::getViewMatrix() {
	return glm::lookAt(m_CameraPos, m_CameraPos + m_CameraFront, m_CameraUp);
}

void Camera::update(double x, double y) {
	if (m_FirstMouse) // this bool variable is initially set to true
	{
		m_LastX = x;
		m_LastY = y;
		m_FirstMouse = false;
	}

	float xOffset = x - m_LastX;
	float yOffset = m_LastY - y;

	m_LastX = x;
	m_LastY = y;

	float sensitivity = 0.05f;
	xOffset *= sensitivity;
	yOffset *= sensitivity;

	m_Yaw += xOffset;
	m_Pitch += yOffset;

	if (m_Pitch < -89.0f)
		m_Pitch = -89.0f;
	else if (m_Pitch > 89.0f)
		m_Pitch = 89.0f;

	glm::vec3 front;
	front.x = cos(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
	front.y = sin(glm::radians(m_Pitch));
	front.z = cos(glm::radians(m_Pitch)) * sin(glm::radians(m_Yaw));
	m_CameraFront = glm::normalize(front);
}

void Camera::updatePosition(GLFWwindow *window) {
	float currentFrame = glfwGetTime();
	m_DeltaTime = currentFrame - m_LastFrame;
	m_LastFrame = currentFrame;

	float cameraSpeed = 2.5 * m_DeltaTime;

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		m_CameraPos += cameraSpeed * m_CameraFront;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		m_CameraPos -= cameraSpeed * m_CameraFront;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		m_CameraPos -= glm::normalize(glm::cross(m_CameraFront, m_CameraUp)) * cameraSpeed;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		m_CameraPos += glm::normalize(glm::cross(m_CameraFront, m_CameraUp)) * cameraSpeed;

	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
		m_CameraPos += cameraSpeed * m_CameraUp;
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
		m_CameraPos -= cameraSpeed * m_CameraUp;
}
