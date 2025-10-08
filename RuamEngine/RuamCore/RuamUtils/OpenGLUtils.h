#pragma once

#define GLCall(x) do { GLClearError();\
    x; \
    ASSERT(GLLogCall(#x, __FILE__, __LINE__));\
	} while(false)

void GLClearError();

bool GLLogCall(const char* function, const char* file, int line);