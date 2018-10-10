#include "Renderer.h"

#include <iostream>

#ifdef _PR_DEBUG

void GLClearError()
{
	while (glGetError() != GL_NO_ERROR) {
		/* Just clearing the flags */
	}
}

bool GLLogCall(const char* function, const char* file, int line)
{
	GLenum error;
	while ((error = glGetError()) != GL_NO_ERROR) {
		std::cout << "[OpenGL Error] (" << error << "):" << function << " in " << file << ":" << line << std::endl;
		return false;
	}
	return true;
}

#endif

void Renderer::ClearColorSetBlack()
{
	GLCheckErrorCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
}

void Renderer::Clear()
{
	GLCheckErrorCall(glClear(GL_COLOR_BUFFER_BIT));
}

void Renderer::Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader)
{
	va.Bind();
	ib.Bind();
	shader.Use();

	/* Draw call */
	GLCheckErrorCall(glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr));
}
