#include "Shader.h"

#include <fstream>
#include <iostream>
#include <sstream>

bool ShaderProgram::_compileShader(unsigned int shader) {
	glCompileShader(shader);
	
	int vertexCompiled;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &vertexCompiled);

	if (vertexCompiled != GL_TRUE) {
		int length;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);

		char* message = (char*)alloca(length * sizeof(char));
		glGetShaderInfoLog(shader, length, &length, message);
		
		std::cout << message << std::endl;
		return false;
	}

	return true;
}

bool ShaderProgram::AddShader(GLenum type, const std::string &path) {
	std::ifstream file;
	file.open(path);

	if (!file) {
		std::cout << "Failed to load shader." << std::endl;
		return false;
	}

	std::stringstream ss;
	ss << file.rdbuf();

	std::string content = ss.str();

	ss.clear();
	file.close();

	const char* shaderSource = content.c_str();

	unsigned int shader = glCreateShader(type);
	glShaderSource(shader, 1, &shaderSource, nullptr);

	if (m_Shaders.find(type) == m_Shaders.end())
		m_Shaders.insert(std::make_pair(type, shader));
	else {
		glDeleteShader(m_Shaders.at(type));
		m_Shaders[type] = shader;
	}		

	return _compileShader(shader);
}

bool ShaderProgram::CompileProgram() {
	m_ShaderProgram = glCreateProgram();
	
	for (std::pair<GLenum, unsigned int>pair : m_Shaders) {
		glAttachShader(m_ShaderProgram, pair.second);
	}

	glLinkProgram(m_ShaderProgram);

	int programLinked;
	glGetProgramiv(m_ShaderProgram, GL_LINK_STATUS, &programLinked);

	if (programLinked != GL_TRUE) {
		int length;
		glGetProgramiv(m_ShaderProgram, GL_INFO_LOG_LENGTH, &length);

		char* message = (char*)alloca(length * sizeof(char));
		glGetProgramInfoLog(m_ShaderProgram, length, &length, message);

		std::cout << message << std::endl;
		return false;
	}

	return true;
}

bool ShaderProgram::Bind() const {
	if (!m_ShaderProgram)
		return false;

	glUseProgram(m_ShaderProgram);

	return true;
}

void ShaderProgram::Unbind() const {
	glUseProgram(0);
}

int ShaderProgram::GetUniform(const std::string &name) {
	if (!m_ShaderProgram)
		return -1;
	return glGetUniformLocation(m_ShaderProgram, name.c_str());
}

ShaderProgram::~ShaderProgram() {
	if (m_ShaderProgram) {
		for (std::pair<GLenum, unsigned int>pair : m_Shaders) {
			glDetachShader(m_ShaderProgram, pair.second);
		}

		glDeleteProgram(m_ShaderProgram);
	}

	for (std::pair<GLenum, unsigned int>pair : m_Shaders) {
		glDeleteShader(pair.second);
	}
}