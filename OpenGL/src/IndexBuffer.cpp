#include "IndexBuffer.h"

#include "Renderer.h"

IndexBuffer::IndexBuffer(const unsigned int* data, unsigned int count) : m_Count(count)
{
	GLCheckErrorCall(glGenBuffers(1, &m_RendererID));
	GLCheckErrorCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));
	GLCheckErrorCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW));
}

IndexBuffer::~IndexBuffer()
{
	GLCheckErrorCall(glDeleteBuffers(1, &m_RendererID));
}

void IndexBuffer::Bind() const
{
	GLCheckErrorCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));
}

void IndexBuffer::Unbind() const
{
	GLCheckErrorCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}

bool IndexBuffer::IsBound() const
{
	return 0 != m_RendererID;
}
