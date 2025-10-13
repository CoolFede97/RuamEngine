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
			#ComponentClass, [](const nlohmann::json &j, unsigned int id) -> Component* { \
				Object* o = SceneManager::ActiveScene()->getObjectById(id); \
				return o->addComponentPtr<ComponentClass>(j); \
			} \
		)); \
		std::cout << "Registered " #ComponentClass << std::endl; \
	} \
}; \
static ComponentClass##Registrar global_##ComponentClass##_registrar; \
}

#define CREATE_COMPONENT(ComponentClass) \
class ComponentClass : public Component { \
public: \
	using Component::Component; \
// WIP
#define FIELD(type, name) \
// WIP


class Object;
class Component {
public:
	using componentFactory = std::function<Component*(const nlohmann::json&, unsigned int)>;
	virtual ~Component() = default;
    explicit Component(const unsigned int obj_id) : m_object_id(obj_id), m_id(s_id_count++) {
	    std::cout << "Created Component with id " << m_id << " for Object " << m_object_id << std::endl;
    };

	virtual void start() = 0;
	virtual void update() = 0;

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
};

class BaseRenderer : public Component {
public:
	using Component::Component;

	virtual void render() = 0;

	void start() {}

	void update() {
		render();
	}
};
