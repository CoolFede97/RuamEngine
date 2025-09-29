#pragma once

#include "glm/glm.hpp"
#include <iostream>

#if defined(_WIN32)
#define ASSERT(x) do {if(!(x)) __debugbreak();} while(false)
#endif
#ifdef linux
#include <csignal>
#include <cstdio>
#define ASSERT(x) do {if(!(x)) { fprintf(stderr, #x " (%s:%d)\n", __FILE__, __LINE__); raise(SIGTRAP);}} while (false)
#endif


inline std::ostream& operator<<(std::ostream& os, const glm::vec2& vec) {
	return os << '(' << vec.x << ", " << vec.y << ')';
}
inline std::ostream& operator<<(std::ostream& os, const glm::vec3& vec) {
	return os << '(' << vec.x << ", " << vec.y << ", " << vec.z << ')';
}
inline std::ostream& operator<<(std::ostream& os, const glm::vec4& vec) {
	return os << '(' << vec.x << ", " << vec.y << ", " << vec.z << ", " << vec.w << ')';
}

#ifdef linux
#include "easy/profiler.h"
#define EASY_PROFILER_DUMP() profiler::dumpBlocksToFile("log.perf");
#else
#define EASY_BLOCK(...)
#define EASY_FUNCTION(...)
#define EASY_END_BLOCK
#define EASY_PROFILER_DUMP()
#define EASY_PROFILER_ENABLE
#endif