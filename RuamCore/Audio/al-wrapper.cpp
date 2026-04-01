#include "al-wrapper.h"
#include <iostream>

namespace AudioSystem {

namespace AL {

void clearErrorStack() {
	alGetError();
	alGetError();
}

void Source::generate() {
	clearErrorStack();
	alGenSources(1, &m_id);
	GetError();
}

void Source::setParam(ALenum param, ALfloat v) {
	alSourcef(m_id, param, v);
	GetError();
}

void Source::setParam(ALenum param, const glm::vec3& v) {
	alSource3f(m_id, param, v.x, v.y, v.z);
	GetError();
}

void Source::setParam(ALenum param, ALint v) {
	alSourcei(m_id, param, v);
	GetError();
}

ALuint Source::id() const {
	return m_id;
}

void Source::bind(const Buffer& buf) {
	return setParam(AL_BUFFER, static_cast<int>(buf.id()));
}

void Source::bind(ALuint buf) {
	return setParam(AL_BUFFER, static_cast<int>(buf));
}

void Source::play() {
	alSourcePlay(m_id);
	GetError();
}

void Source::stop() {
	alSourceStop(m_id);
	GetError();
}

void Source::pause() {
	alSourcePause(m_id);
	GetError();
}

void Source::destroy() {
	alDeleteSources(1, &m_id);
}

Source::~Source() {
	destroy();
}

void Source::get(ALenum param, ALint* out) {
	alGetSourcei(m_id, param, out);
	GetError();
}

void Source::get(ALenum param, float* out) {
	alGetSourcef(m_id, param, out);
	GetError();
}

ALint Source::state() {
	int res;
	get(AL_SOURCE_STATE, &m_state);
	return m_state;
}

//

void GetError() {
	switch (alGetError()) {
		case AL_NO_ERROR:
			return;
		case AL_INVALID_NAME:
			throw al_error("Invalid name(id)");
		case AL_INVALID_ENUM:
			throw al_error("Invalid enum");
		case AL_INVALID_VALUE:
			throw al_error("Invalid value");
		case AL_INVALID_OPERATION:
			throw al_error("Invalid operation");
		case AL_OUT_OF_MEMORY:
			throw al_error("Out of memory");
	}
}

void Buffer::generate() {
	alGenBuffers(1, &m_id);
	GetError();
}

ALuint Buffer::id() const {
	return m_id;
}

void Buffer::setData(ALenum format, char* data, ALsizei size, ALuint freq) {
	alBufferData(m_id, format, data, size, freq);
	GetError();
}

void Buffer::setData(ALenum format, std::vector<char> data, ALuint freq) {
	alBufferData(m_id, format, data.data(), static_cast<ALsizei>(data.size()), freq);
	GetError();
}

Buffer::~Buffer() {
	destroy();
}

void Buffer::destroy() {
	alDeleteBuffers(1, &m_id);
}

namespace Listener {
void setParam(ALenum param, ALfloat v) {
	alListenerf(param, v);
	GetError();
}

void setParam(ALenum param, glm::vec3& v) {
	alListener3f(param, v.x, v.y, v.z);
	GetError();
}

void setParam(ALenum param, ALint v) {
	alListeneri(param, v);
	GetError();
}
}

al_error::al_error(char const* const msg) throw() : std::runtime_error(msg) {}

char const* al_error::what() const throw() {
	return std::runtime_error::what();
}

}
}
