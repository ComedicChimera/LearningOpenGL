#pragma once

#include <vector>

struct LayoutComponent {
	unsigned int type;
	unsigned int count;
	bool normalized;

	unsigned int size;
};

class BufferLayout {
	std::vector<LayoutComponent> m_Elements;
	unsigned int m_Stride;

public:
	BufferLayout()
		: m_Stride(0)
	{}

	void Push(LayoutComponent elem) {
		m_Elements.push_back(elem);
		m_Stride += elem.size;
	}

	inline const unsigned int GetStride() const { return m_Stride; }
	inline const std::vector<LayoutComponent>& GetElements() const { return m_Elements; }
};