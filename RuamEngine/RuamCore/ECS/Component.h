#pragma once
#include "nlohmann/json.hpp"
#include <map>
#include <memory>
#include <string>
#include "../../RuamCore/Input/Input.h"
#include "RuamTime.h"

#include "Editor.h"
#include "Entity.h"

// ==== MACROS de Serialización ====
//
// Macro 1: SER_FIELD
#define SER_FIELD(name, type, initialValue /* unused */, callbackOnInspectorChange /* unused */) {#name, name}

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
			Component::componentRegistry.insert(std::make_pair(	\
			#ComponentName, \
			ComponentFactory{ \
				[](Entity* entity) -> Component* \
				{ \
					return entity->addComponent<ComponentName>(); \
				}, \
				[](const Json& componentData, Entity* entity) -> Component* \
				{ \
					return entity->addComponentWithJson<ComponentName>(componentData); \
				} \
			})); \
		} \
	}; \
	static ComponentName##Register global##ComponentName##Register; \
} \

// Macros para declaración de miembros
#define DECL_MEMBER(name, type, initialValue, callbackOnInspectorChange /* unused */) type name = initialValue;


// Creates the forEachSerializedField function
#define CALL_INSPECTOR_DRAWER(name, type, initialValue, callbackOnInspectorChange) \
    if constexpr (!std::is_same_v<decltype(callbackOnInspectorChange), std::nullptr_t>) \
    {   \
        temp_callback = callbackOnInspectorChange;  \
        callbacks[#name] = &temp_callback;  \
    }   \
    Editor::DrawMemberInInspector(#name, typeid(type), &name, callbacks[#name]);

// Draw serialized members in the inspector
#define IMPL_DRAW_SERIALIZED_MEMBERS(inspectorDrawerCalls)	\
    friend class Editor;    \
    std::unordered_map<std::string, std::function<void()>*> callbacks = {};  \
    std::function<void()> temp_callback;    \
	inline void drawSerializedMembers() override	\
	{	\
		inspectorDrawerCalls	\
	}

namespace RuamEngine
{
	class Entity;
	class Transform;

	struct ComponentFactory
	{
		std::function<Component*(Entity*)> addComponent;
		std::function<Component*(const Json&, Entity*)> addComponentWithJson;
	};

	class Component {
	public:

		inline static std::map<std::string, ComponentFactory> componentRegistry;

		virtual ~Component() = default;
		explicit Component(const unsigned int entityId) : m_entityId(entityId), m_id(s_idCount++) {
		}
		explicit Component(Json componentData, const unsigned int entityId) : m_entityId(entityId), m_id(s_idCount++) {
		}
		virtual void start() {};  // This is called only in playmode
		virtual void update() {}; // This is called only in playmode
		virtual void renderStart() {};
		virtual void renderUpdate() {}; // This is called regardless of the state of the engine (this means it's called even in edit mode)

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

        // Doesn't use the macro IMPL_DRAW_SERIALIZED_MEMBERS since this is the virtual one.
        virtual inline void drawSerializedMembers() {;};
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
