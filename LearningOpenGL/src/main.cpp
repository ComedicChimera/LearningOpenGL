#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>

#include "Shader.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "BufferLayout.h"
#include "Texture.h"
#include "Renderer.h"
#include "Camera.h"

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f), 400, 300);

void APIENTRY callback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam) {
	std::cout << message << std::endl;
}

void mouseCallback(GLFWwindow *window, double x, double y);

int main()
{
	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit()) {
		std::cout << "Failed to initialize GLFW." << std::endl;
		return -1;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);

	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(640, 480, "Learning OpenGL", NULL, NULL);
	if (!window)
	{
		std::cout << "Failed to create window." << std::endl;
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	// init glew
	if (glewInit() != GLEW_OK) {
		std::cout << "Failed to initialize GLEW." << std::endl;
		glfwTerminate();
		return -1;
	}

	std::cout << glGetString(GL_VERSION) << std::endl;

	glEnable(GL_DEPTH_TEST);

	glDebugMessageCallback(callback, nullptr);

	float vertices[] = {
		-0.5f, -0.5f,  0.0f, 0.0f, 0.0f,
		 0.5f, -0.5f,  0.0f, 1.0f, 0.0f,
		 0.5f,  0.5f,  0.0f, 1.0f, 1.0f,
		-0.5f,  0.5f,  0.0f, 0.0f, 1.0f,

		-0.5f, -0.5f, -1.0f, 0.0f, 0.0f,
		 0.5f, -0.5f, -1.0f, 1.0f, 0.0f,
		 0.5f,  0.5f, -1.0f, 1.0f, 1.0f,
		-0.5f,  0.5f, -1.0f, 0.0f, 1.0f,
	};

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);

	VertexArray vao;
	VertexBuffer vbo(vertices, 40 * sizeof(float));

	BufferLayout layout;
	layout.Push({GL_FLOAT, 3, false, 3 * sizeof(float)});
	layout.Push({GL_FLOAT, 2, false, 2 * sizeof(float)});

	vao.AddBuffer(vbo, layout);

	unsigned int indices[] = {
		0, 1, 2,
		2, 3, 0,
		4, 5, 6,
		6, 7, 4,
		4, 0, 3,
		3, 7, 4,
		5, 1, 2,
		2, 6, 5,
		3, 2, 6,
		6, 7, 3,
		0, 1, 5,
		5, 4, 0
	};

	IndexBuffer ibo(indices, 36);

	ShaderProgram shaderProgram;
	shaderProgram.AddShader(GL_VERTEX_SHADER, "shaders/vertex.glsl");
	shaderProgram.AddShader(GL_FRAGMENT_SHADER, "shaders/fragment.glsl");

	shaderProgram.CompileProgram();	
	shaderProgram.Bind();

	Texture texture("assets/textures/test.png");
	texture.Bind();

	// matrices
	glm::mat4 proj_mat = glm::perspective(glm::radians(45.0f), (float)(640 / 480), 0.1f, 100.0f);

	glm::mat4 model_mat(1.0f);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, mouseCallback);

	// uniforms
	int proj_location = shaderProgram.GetUniform("u_Proj");
	glUniformMatrix4fv(proj_location, 1, GL_FALSE, &proj_mat[0][0]);

	int model_location = shaderProgram.GetUniform("u_Model");
	glUniformMatrix4fv(model_location, 1, GL_FALSE, &model_mat[0][0]);

	int texture_location = shaderProgram.GetUniform("u_Texture");
	glUniform1i(texture_location, 0);
	
	// render loop
	Renderer renderer;

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		renderer.Clear();

		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			break;

		camera.updatePosition(window);

		int viewLocation = shaderProgram.GetUniform("u_View");
		glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &camera.getViewMatrix()[0][0]);

		renderer.Draw(vao, ibo, shaderProgram);

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	glfwTerminate();

	return 0;
}

void mouseCallback(GLFWwindow *window, double x, double y) {
	camera.update(x, y);
}