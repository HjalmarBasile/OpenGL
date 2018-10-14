#include "VertexArray.h"

#include "Renderer.h"

VertexArray::VertexArray()
{
	/* Retrieve an Id for the vao */
	GLCheckErrorCall(glGenVertexArrays(1, &m_RendererID));

	/* Bind the vertex array vao */
	this->Bind();
}

VertexArray::~VertexArray()
{
	GLCheckErrorCall(glDeleteVertexArrays(1, &m_RendererID));
}

void VertexArray::Bind() const
{
	GLCheckErrorCall(glBindVertexArray(m_RendererID));
}

void VertexArray::Unbind() const
{
	GLCheckErrorCall(glBindVertexArray(0));
}

void VertexArray::AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout)
{
	this->Bind();

	if (!vb.IsBound()) {
		vb.Bind();
	}

	size_t offset = 0;
	const std::vector<VertexBufferElement>& elements = layout.GetElements();
	for (unsigned int index = 0; index < elements.size(); ++index) {
		const VertexBufferElement& element = elements[index];

		/* Enable the new coordinates attribute */
		GLCheckErrorCall(glEnableVertexAttribArray(index));

		/* Define the attribute for the position array */
		GLCheckErrorCall(glVertexAttribPointer(index, element.count, element.type, element.normalized,
			layout.GetStride(), reinterpret_cast<const GLvoid*>(offset)));

		offset += element.size;
	}
}
