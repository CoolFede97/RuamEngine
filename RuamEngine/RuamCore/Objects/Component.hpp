#pragma once
#include <iostream>
#include "nlohmann/json.hpp"
#include <map>
#include <string>
#define REGISTER_COMPONENT(ComponentClass) \
namespace { \
struct ComponentClass##Registrar { \
	ComponentClass##Registrar() { \
		Component::componentRegistry.insert(std::make_pair( \
			#ComponentClass, [](const nlohmann::json &j, Object *o) -> Component* { \
				return o->addComponentPtr<ComponentClass>(j); \
			} \
		)); \
	} \
}; \
static ComponentClass##Registrar global_##ComponentClass##_registrar; \
}

#define CREATE_COMPONENT(ComponentClass, deserialiseCode, startCode, updateCode) \
class ComponentClass : public Component { \
public: \
	using Component::Component; \
	ComponentClass(const nlohmann::json& j, unsigned int obj_id) : Component(obj_id) \
		deserialiseCode \
	void start() override startCode \
	void update() override updateCode

#define IMPL_SERIALIZE(ComponentClass, ...) \
public: \
	operator nlohmann::json() const override { \
		return nlohmann::json{			\
			__VA_ARGS__, \
			{"id", m_id}, \
			{"type", #ComponentClass}, \
		}; \
	} \

class Object;
class Component {
public:
	using componentFactory = std::function<Component*(const nlohmann::json&, Object*)>;
	virtual ~Component() = default;
	explicit Component(const unsigned int obj_id) : m_object_id(obj_id), m_id(s_id_count++) {
	}

	virtual void start() = 0;
	virtual void update() {
		if (!m_started) {
			start();
			m_started = true;
		}
	}

	bool operator==(const Component& other) const;

	unsigned int id() const;
	Object* object() const;

	inline static std::map<std::string, componentFactory> componentRegistry;

	virtual operator nlohmann::json() const {
		return nlohmann::json{
			{"type", "Component"}
		};
	}

protected:
	const unsigned int m_object_id;
	const unsigned int m_id;
	static unsigned int s_id_count;
	bool m_started = false;
};

class BaseRenderer : public Component {
public:
	using Component::Component;

	virtual void render() = 0;

	void start() {}

	void update() {
		Component::update();
		render();
	}
};
