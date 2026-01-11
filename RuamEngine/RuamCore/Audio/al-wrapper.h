#pragma once

#include <cstdint>
#include <type_traits>
#include <algorithm>
#include <iostream>
#include <vector>

#include "glm/glm.hpp"
#include "al.h"

namespace AudioSystem {

namespace AL {

void clearErrorStack();

class al_error : public std::runtime_error {
public:
	al_error(char const* const msg) throw();
	char const* what() const throw();
};

class Buffer;

class Source {
public:
	~Source();

	void generate();

	void setParam(ALenum param, ALfloat v);
	void setParam(ALenum param, const glm::vec3& v);
	void setParam(ALenum param, ALint v);

	void bind(const Buffer& buf);
	void bind(ALuint buf);

	void get(ALenum param, ALint* out);
	void get(ALenum param, float* out);

	void play();
	void stop();
	void pause();

	ALint state();

	void destroy();

	ALuint id() const;
private:
	ALuint m_id;
	ALint m_state;
};

class Buffer {
public:
	~Buffer();
	void generate();
	void setData(ALenum format, char* data, ALsizei size, ALuint freq);
	void setData(ALenum format, std::vector<char> data, ALuint freq);

	void destroy();

	ALuint id() const;


private:
	ALuint m_id;
};

namespace Listener {
void setParam(ALenum param, ALfloat v);
void setParam(ALenum param, glm::vec3& v);
void setParam(ALenum param, ALint v);
}

void GetError();

}
}
