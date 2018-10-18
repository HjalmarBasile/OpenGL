#pragma once

class VertexBuffer
{
private:
	unsigned int m_RendererID;
public:
	VertexBuffer(const void* data, size_t size);
	~VertexBuffer();

	void Bind() const;
	static void Unbind();
	bool IsBound() const;
};
