#pragma once

#include "alc.h"
#include <string>
#include <cstdint>
#include <vector>

namespace AudioSystem {

namespace ALC {

using ALCerror = uint16_t;

class Device {
public:
	Device();
	Device(const char* device_name);
	Device(std::nullptr_t);
	Device(ALCdevice* dev) : m_device(dev) {};
	~Device();
	ALCdevice* device() const;
	ALCerror getError();
	bool close();

	void open(const char* device_name);
	void open(std::nullptr_t);


private:
	ALCdevice* m_device;
};

class Context {
public:
	Context();
	Context(const Device& dev, ALCint* attr_list = nullptr);
	~Context();
	
	void create(const Device& dev, ALCint* attr_list = nullptr);
	void destroy();
	ALCcontext* context() const;
	Device getDevice() const;
private:
	ALCcontext* m_context;
};

void GetDevices(std::vector<std::string>& devices);
bool MakeContextCurrent(Context& ctx);
bool MakeContextCurrent(std::nullptr_t);

}
}
