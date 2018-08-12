#pragma once

#include <GL/glew.h>

#include <string>
#include <unordered_map>

class ShaderProgram {
	std::unordered_map<GLenum, unsigned int> m_Shaders;
	unsigned int m_ShaderProgram;

public:
	// public for extension, not for use
	bool _compileShader(unsigned int shader);

	bool AddShader(GLenum type, const std::string &path);
	bool CompileProgram();

	bool Bind() const;
	void Unbind() const;

	int GetUniform(const std::string &name);

	~ShaderProgram();
};
