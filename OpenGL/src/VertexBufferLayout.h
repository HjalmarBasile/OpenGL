#pragma once

#include <vector>
#include <GL/glew.h>

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

	template<typename T> void Push(GLint count);
	template<>			 void Push<unsigned char>(GLint count);
	template<>			 void Push<unsigned int>(GLint count);
	template<>			 void Push<float>(GLint count);

	inline GLsizei GetStride() const { return m_Stride; }
	inline const std::vector<VertexBufferElement>& GetElements() const { return m_Elements; }
};
