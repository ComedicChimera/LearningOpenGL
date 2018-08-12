#include "VertexArray.h"

#include <GL/glew.h>

VertexArray::VertexArray() {
	glGenVertexArrays(1, &m_ID);
}

VertexArray::~VertexArray() {
	glDeleteVertexArrays(1, &m_ID);
}

void VertexArray::AddBuffer(const VertexBuffer &vb, const BufferLayout &layout) {
	Bind();
	vb.Bind();

	const auto& elements = layout.GetElements();
	
	unsigned int offset = 0;
	for (unsigned int i = 0; i < elements.size(); i++) {
		const auto &element = elements[i];

		glVertexAttribPointer(i, element.count, element.type, element.normalized ? GL_TRUE : GL_FALSE, layout.GetStride(), (const void*)offset);
		glEnableVertexAttribArray(i);

		offset += element.size;
	}
}

void VertexArray::Bind() const {
	glBindVertexArray(m_ID);
}

void VertexArray::Unbind() const {
	glBindVertexArray(0);
}