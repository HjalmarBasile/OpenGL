#pragma once

#include <GL/glew.h>

#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"

#ifdef _PR_DEBUG
#define ASSERT_AND_BREAK(x) if(!(x)) __debugbreak();

#define GLCheckErrorCall(glFun) GLClearError();\
		glFun;\
		ASSERT_AND_BREAK(GLLogCall(#glFun, __FILE__, __LINE__))

void GLClearError();
bool GLLogCall(const char* function, const char* file, int line);

#else
#define GLCheckErrorCall(glFun) glFun
#endif

class Renderer
{
public:
	static void Clear();
	static void Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader);
};
