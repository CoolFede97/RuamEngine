#pragma once

#include <iostream>
#include <array>
#include <memory>
#include <vector>

#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "RuamUtils.h"

#include "Vec3.h"
#include "Vec4.h"
#include "Vec2.h"

#define GLCall(x) do { GLClearError();\
    x; \
    ASSERT(GLLogCall(#x, __FILE__, __LINE__));\
	} while(false)

#ifndef RUAM_GL_CLEAR_ERROR_DEFINED
#define RUAM_GL_CLEAR_ERROR_DEFINED
inline void GLClearError()
{
    while (glGetError() != GL_NO_ERROR);
}
#endif

#ifndef RUAM_GL_LOG_CALL_DEFINED
#define RUAM_GL_LOG_CALL_DEFINED
inline bool GLLogCall(const char* function, const char* file, int line)
{
    while (GLenum error = glGetError())
    {
        std::cout << "[OpenGL Error] (" << error << "): " << "\n"
            << "in file: " << file << "\n"
            << "in line: " << line << "\n"
            << "in function: " << function << "\n";
        return false;
    }
    return true;
}
#endif
