#pragma once
#include "nlohmann/json.hpp"

class Object;
class Component {
public:
	using componentFactory = std::function<std::unique_ptr<Component>(const nlohmann::json&, Object*)>;
	virtual ~Component() = default;
    explicit Component(const unsigned int obj_id) : m_object_id(obj_id), m_id(s_id_count++) {};

	virtual void start() = 0;
	virtual void update() = 0;

	bool operator==(const Component& other) const;

	unsigned int id() const;
	Object* object() const;

	static std::map<std::string, componentFactory> componentRegistry;

	virtual operator nlohmann::json() const {
		return nlohmann::json{
							{"type", "Conpoent"},
							{"id", m_id}
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
