#pragma once

#include "BufferLayout.h"
#include "VertexBuffer.h"

class VertexArray {
	unsigned int m_ID;
public:
	VertexArray();
	~VertexArray();

	void AddBuffer(const VertexBuffer &vb, const BufferLayout &layout);

	void Bind() const;
	void Unbind() const;
};