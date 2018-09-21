#include "VertexBuffer.h"

#include "Renderer.h"

VertexBuffer::VertexBuffer(const void * data, size_t size)
{
	/* Generate Vertex Buffer with modern OpenGL */
	GLCheckErrorCall(glGenBuffers(1, &m_RendererID));
	GLCheckErrorCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));

	/* Pass the data and specify that it must be drawn */
	GLCheckErrorCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
}

VertexBuffer::~VertexBuffer()
{
	GLCheckErrorCall(glDeleteBuffers(1, &m_RendererID));
}

void VertexBuffer::Bind() const
{
	GLCheckErrorCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
}

void VertexBuffer::Unbind() const
{
	GLCheckErrorCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}

bool VertexBuffer::IsBound() const
{
	return 0 != m_RendererID;
}
