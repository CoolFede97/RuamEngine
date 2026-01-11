#pragma once
#include <iostream>
#include "nlohmann/json.hpp"
#include <map>
#include <memory>
#include <string>
// ==== MACROS de Serialización ====
//
// Macro 1: SER_FIELD
#define SER_FIELD(x) {#x, x}

// Macro 2: IMPL_SIMPLE_SERIALIZE
//
#define IMPL_SIMPLE_SERIALIZE(ComponentClass) \
public: \
virtual operator Json() const override \
{ \
	return Json \
	{ \
		{"m_id", m_id}, \
		{"TYPE", #ComponentClass} \
	}; \
} \

// Macro 3: IMPL_SERIALIZE
//
#define IMPL_SERIALIZE(ComponentClass, ...) \
public: \
virtual operator Json() const override \
{ \
	return Json \
	{ \
		__VA_ARGS__, \
		{"m_id", m_id}, \
		{"TYPE", #ComponentClass} \
	}; \
} \

#define REGISTER_COMPONENT(ComponentClass) \
namespace \
{ \
	struct ComponentClass##Register \
	{ \
		ComponentClass##Register() \
		{ \
			Component::componentRegistry.insert(std::make_pair(#ComponentClass, \
			[](const Json& componentData, Entity* entity) -> Component* \
			{ \
				return entity->addComponentPtr<ComponentClass>(componentData); \
			})); \
		} \
	}; \
	static ComponentClass##Register global##ComponentClass##Register; \
} \

namespace RuamEngine
{
	class Entity;
	class Transform;
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
}
