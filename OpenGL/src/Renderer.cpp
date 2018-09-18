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
