#pragma once

#include <vector>
#include <glad/glad.h>

struct VertexBufferElement
{
	GLint count;
	GLenum type;
	GLboolean normalized;
	size_t size;
};

class VertexBufferLayout
{
private:
	GLsizei m_Stride;
	std::vector<VertexBufferElement> m_Elements;

public:
	VertexBufferLayout();

	template<typename T>
	void Push(GLint count)
	{
		static_assert(false, "Appropriate template specialization not found for VertexBufferLayout::Push");
	}

	template<>
	void Push<unsigned char>(GLint count)
	{
		size_t size = count * sizeof(GLubyte);
		m_Elements.push_back({ count, GL_UNSIGNED_BYTE, GL_TRUE, size });
		m_Stride += size;
	}

	template<>
	void Push<unsigned int>(GLint count)
	{
		size_t size = count * sizeof(GLuint);
		m_Elements.push_back({ count, GL_UNSIGNED_INT, GL_FALSE, size });
		m_Stride += size;
	}

	template<>
	void Push<float>(GLint count)
	{
		size_t size = count * sizeof(GLfloat);
		m_Elements.push_back({ count, GL_FLOAT, GL_FALSE, size });
		m_Stride += size;
	}

	inline GLsizei GetStride() const { return m_Stride; }
	inline const std::vector<VertexBufferElement>& GetElements() const { return m_Elements; }
};
