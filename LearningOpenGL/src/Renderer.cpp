#include "Renderer.h"

void Renderer::Draw(const VertexArray &vao, const IndexBuffer &ibo, const ShaderProgram &shaderProgram) const {
	vao.Bind();
	ibo.Bind();
	shaderProgram.Bind();

	glDrawElements(GL_TRIANGLES, ibo.GetCount(), GL_UNSIGNED_INT, nullptr);
}

void Renderer::Clear() const {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}