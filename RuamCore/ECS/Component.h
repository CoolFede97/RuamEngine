#pragma once
#include "ComponentField.h"
#include "ComponentsInitializer.h"
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
	class Transform;
	class Component;
	class Entity;
	class CollisionInfo;

	struct ComponentFactory
	{
		std::function<Component*(Entity*)> addComponent;
	};

	class Component {
	public:

		inline static std::map<std::string, ComponentFactory> componentRegistry;
		static constexpr bool s_unique = false;
		virtual ~Component() = default;
		explicit Component(const unsigned int entityId) : m_entity(nullptr), m_id(s_idCount++) {
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
		bool disabledManually() const;

		template<class Cmp>
		Cmp* getComponent() const;

		std::vector<Component*> getComponents() const;

		template<class Cmp>
		std::vector<Cmp*> getComponentsOfType() const;

		template<class Cmp>
		void removeComponent();

		template<class Cmp>
		void removeComponent(Component& comp);

		bool enabled() const { return m_enabled; }
		bool destroyFlag() const { return m_destroyFlag; }

		void destroy() { m_destroyFlag = true; }
		void setEnabled(bool state);

		virtual std::vector<FieldInfo> fields() { return {}; };

		virtual operator nlohmann::json() const {
            return nlohmann::json{
                {"m_id", m_id},
                {"m_enabled", m_enabled},
                {"TYPE", "Component"}
            };
        }

        // Doesn't use the macro IMPL_DRAW_SERIALIZED_MEMBERS since this is the virtual one.
        virtual void drawSerializedMembers();

        static void ComponentRegister();
    	virtual std::string name() const { return "Component"; }
    	friend class ComponentsInitializer;
        friend class Scene;
        friend class Entity;
	protected:
		Entity* m_entity;
		const unsigned int m_id;
		static unsigned int s_idCount;
		bool m_enabled = true;
		bool m_started = false;
		bool m_destroyFlag = false;
		bool m_disabledManually = false;
	private:
		virtual void onCollisionEnter(const CollisionInfo& col) {};
		virtual void onCollisionStay(const CollisionInfo& col) {};
		virtual void onCollisionExit(const CollisionInfo& col) {};

	    // Internal API: called only when the owning entity changes state
	    void setEnabledByEntity(bool entityEnabled);

		friend class Entity;
	};

	using ComponentUPtr = std::unique_ptr<Component>;
}
