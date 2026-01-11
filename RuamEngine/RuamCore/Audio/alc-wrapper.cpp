#include "alc-wrapper.h"
#include <stdexcept>

namespace AudioSystem {

namespace ALC {

Device::Device() {
	m_device = nullptr;
}

Device::Device(const char* device_name) {
	open(device_name);
}

Device::Device(std::nullptr_t) {
	open(nullptr);
}

ALCdevice* Device::device() const {
	return m_device;
}

ALCerror Device::getError() {
	return alcGetError(m_device);
}

bool Device::close() {
	return alcCloseDevice(m_device);
}

Device::~Device() {
	close();
}

void Device::open(const char* device_name) {
	m_device = alcOpenDevice(device_name);
	if (getError()) {
		throw std::runtime_error("Could not open device");
	}
}

void Device::open(std::nullptr_t) {
	open(static_cast<char*>(nullptr));
}

Context::Context() {
	m_context = nullptr;
}

Context::Context(const Device& dev, ALCint* attr_list) {
	create(dev, attr_list);
}

void Context::destroy() {
	if (alcGetCurrentContext() == m_context)
		MakeContextCurrent(nullptr);
	alcDestroyContext(m_context);
}

Context::~Context() {
	destroy();
}

ALCcontext* Context::context() const {
	return m_context;
}

Device Context::getDevice() const {
	return alcGetContextsDevice(m_context);
}

void Context::create(const Device& dev, ALCint* attr_list) {
	m_context = alcCreateContext(dev.device(), attr_list);
}

void GetDevices(std::vector<std::string>& devices); // TODO:

bool MakeContextCurrent(Context& ctx) {
	return alcMakeContextCurrent(ctx.context());
}

bool MakeContextCurrent(std::nullptr_t) {
	return alcMakeContextCurrent(nullptr);
}

}
}
