#pragma once
#include <iostream>
#include "nlohmann/json.hpp"
#include <map>
#include <memory>
#include <string>
#include <typeinfo>
// ==== MACROS de Serialización ====
//
// Macro 1: SER_FIELD
#define SER_FIELD(name, type, initialValue /* unused */, componentName /* unused */) {#name, name}

// Macro 2: IMPL_SIMPLE_SERIALIZE
//
#define IMPL_SIMPLE_SERIALIZE(ComponentName) \
public: \
virtual operator Json() const override \
{ \
	return Json \
	{ \
		{"m_id", m_id}, \
		{"TYPE", #ComponentName} \
	}; \
} \

// Macro 3: IMPL_SERIALIZE
//
#define IMPL_SERIALIZE(ComponentName, ...) \
public: \
virtual operator Json() const override \
{ \
	return Json \
	{ \
		__VA_ARGS__ \
		,{"m_id", m_id}, \
		{"TYPE", #ComponentName} \
	}; \
} \

// Registers a function in Component::componentRegistry that add a component to the entity parameter
#define REGISTER_COMPONENT(ComponentName) \
namespace \
{ \
	struct ComponentName##Register \
	{ \
		ComponentName##Register() \
		{ \
			Component::componentRegistry.insert(std::make_pair(#ComponentName, \
			[](const Json& componentData, Entity* entity) -> Component* \
			{ \
				return entity->addComponentPtr<ComponentName>(componentData); \
			})); \
		} \
	}; \
	static ComponentName##Register global##ComponentName##Register; \
} \

// Macros para declaración de miembros
#define DECL_MEMBER(name, type, initialValue, componentName /* unused */) type name = initialValue;
#define DECL_STATIC_MEMBER(name, type, initialValue, componentName /* unused */) static type name;
#define DEF_STATIC_MEMBER(name, type, initialValue, componentName) type componentName::name = initialValue;
#define CALL_INSPECTOR_DRAWER(name, type, initialValue /* unused */, componentName /* unused */) fn(#name, typeid(type), &name);

// Creates the forEachSerializedField function
#define IMPL_forEachSerializedField(inspectorDrawerCalls)	\
	inline void forEachSerializedField(SerializedFieldFunction fn) override	\
	{	\
		inspectorDrawerCalls	\
	}

namespace RuamEngine
{
	class Entity;
	class Transform;

	using SerializedFieldFunction = std::function<void(const std::string&, const std::type_info&, void*)>;

	class Component {
	public:
		using ComponentFactory = std::function<Component*(const Json&, Entity*)>;
		inline static std::map<std::string, ComponentFactory> componentRegistry;

		virtual ~Component() = default;
		explicit Component(const unsigned int entityId) : m_entityId(entityId), m_id(s_idCount++) {
		}
		explicit Component(Json componentData, const unsigned int entityId) : m_entityId(entityId), m_id(s_idCount++) {
		}
		virtual void start() {};
		virtual void update() {};

		bool operator==(const Component& other) const;
		bool operator==(const std::unique_ptr<Component>& other) const;

		unsigned int id() const;
		Entity* entity() const;
		Transform& transform() const; // returns the entity's transform

		bool enabled() const { return m_enabled; }
		bool destroyFlag() const { return m_destroyFlag; }
		bool createdOnThisFrame() const { return m_createdOnThisFrame; }

		void destroy() { m_destroyFlag = true; setEnabled(false); }
		void markNotCreatedOnThisFrame() { m_createdOnThisFrame = false; }
		void setEnabled(bool status) { m_enabled = status; };

		virtual operator Json() const {
            return Json{
                {"m_id", m_id},
                {"TYPE", "Component"}
            };
        }

        // Doesn't use the macro IMPL_forEachSerializedField since this is the virtual one.
        virtual inline void forEachSerializedField(SerializedFieldFunction fn) {;};
        virtual inline std::string name() { return "Component"; }
	protected:
		const unsigned int m_entityId;
		const unsigned int m_id;
		static unsigned int s_idCount;
		bool m_enabled = true;
		bool m_createdOnThisFrame = true;
		bool m_startedFlag = false;
		bool m_destroyFlag = false;

	public:
	};

	using ComponentUPtr = std::unique_ptr<Component>;

	// class BaseRenderer : public Component {
	// public:
	// 	using Component::Component;

	// 	virtual void render() = 0;

	// 	void start() {}

	// 	void update() {
	// 		Component::update();
	// 		render();
	// 	}
	// };

}
