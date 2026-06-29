#pragma once
#include "ComponentField.h"
#include "ComponentsInitializer.h"
#include "nlohmann/json.hpp"
#include <map>
#include <memory>
#include <string>

#include "Editor.h"

#define DECL_REGISTER_COMPONENT(ComponentName) \
	static void ComponentName##Register(); \
	std::string name() const override { return #ComponentName; } \
	friend class ComponentsInitializer;

#define DEF_REGISTER_COMPONENT(ComponentName) \
	void ComponentName::ComponentName##Register() \
	{ \
		Component::componentRegistry.insert(std::make_pair(	\
		#ComponentName, \
		ComponentFactory{ \
			[](Entity* entity) -> Component* \
			{ \
				return entity->addComponent<ComponentName>(); \
			} \
		})); \
	} \

namespace RuamEngine
{
	class Entity;
	class Transform;
	class Component;

	struct ComponentFactory
	{
		std::function<Component*(Entity*)> addComponent;
		std::function<Component*(const nlohmann::json&, Entity*)> addComponentWithJson;
	};

	class Component {
	public:

		inline static std::map<std::string, ComponentFactory> componentRegistry;
		static constexpr bool s_unique = false;
		virtual ~Component() = default;
		explicit Component(const unsigned int entityId) : m_entityId(entityId), m_id(s_idCount++) {
		}
		explicit Component(nlohmann::json componentData, const unsigned int entityId) : m_entityId(entityId), m_id(s_idCount++) {
		}
		virtual void start() {};  // This is called only in playmode
		virtual void update() {}; // This is called only in playmode
		virtual void renderStart() {};
		virtual void renderUpdate() {}; // This is called regardless of the state of the engine (this means it's called even in edit mode)

		bool operator==(const Component& other) const;
		bool operator==(const std::unique_ptr<Component>& other) const;

		unsigned int id() const;
		Entity* entity() const;
		Transform* transform() const; // returns the entity's transform

		bool enabled() const { return m_enabled; }
		bool destroyFlag() const { return m_destroyFlag; }
		bool createdOnThisFrame() const { return m_createdOnThisFrame; }

		void destroy() { m_destroyFlag = true; }
		void markNotCreatedOnThisFrame() { m_createdOnThisFrame = false; }
		void setEnabled(bool status) { m_enabled = status; };

		virtual std::vector<FieldInfo> fields() { return {}; };

		virtual operator nlohmann::json() const {
            return nlohmann::json{
                {"m_id", m_id},
                {"TYPE", "Component"}
            };
        }

        // Doesn't use the macro IMPL_DRAW_SERIALIZED_MEMBERS since this is the virtual one.
        virtual void drawSerializedMembers();
	protected:
		const unsigned int m_entityId;
		const unsigned int m_id;
		static unsigned int s_idCount;
		bool m_enabled = true;
		bool m_createdOnThisFrame = true;
		bool m_startedFlag = false;
		bool m_destroyFlag = false;

	public:
        static void ComponentRegister();
    	virtual std::string name() const { return "Component"; }
    	friend class ComponentsInitializer;
	};

	using ComponentUPtr = std::unique_ptr<Component>;
}
