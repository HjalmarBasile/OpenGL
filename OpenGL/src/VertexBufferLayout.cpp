#include "VertexBufferLayout.h"

VertexBufferLayout::VertexBufferLayout() : m_Stride(0) {}

template<typename T>
void VertexBufferLayout::Push(GLint count)
{
	static_assert(false, "Appropriate template specialization not found for VertexBufferLayout::Push");
}

template<>
void VertexBufferLayout::Push<unsigned char>(GLint count)
{
	size_t size = count * sizeof(GLubyte);
	m_Elements.push_back({ count, GL_UNSIGNED_BYTE, GL_TRUE, size});
	m_Stride += size;
}

template<>
void VertexBufferLayout::Push<unsigned int>(GLint count)
{
	size_t size = count * sizeof(GLuint);
	m_Elements.push_back({ count, GL_UNSIGNED_INT, GL_FALSE, size });
	m_Stride += size;
}

template<>
void VertexBufferLayout::Push<float>(GLint count)
{
	size_t size = count * sizeof(GLfloat);
	m_Elements.push_back({ count, GL_FLOAT, GL_FALSE, size });
	m_Stride += size;
}
