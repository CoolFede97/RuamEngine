#pragma once
#include <iostream>
#include "nlohmann/json.hpp"
#include <map>
#include <memory>
#include <string>

namespace RuamEngine
{
	#define REGISTER_COMPONENT(ComponentClass) \
	namespace { \
	struct ComponentClass##Registrar { \
		ComponentClass##Registrar() { \
			Component::componentRegistry.insert(std::make_pair( \
				#ComponentClass, [](const nlohmann::json &j, Entity *o) -> Component* { \
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
		operator nlohmann::json() const { \
			return nlohmann::json{			\
				__VA_ARGS__, \
				{"id", m_id}, \
				{"type", #ComponentClass}, \
			}; \
		}


	#define IMPL_SIMPLE_SERIALIZE(ComponentClass)\
	public: \
		operator nlohmann::json() const { \
		return nlohmann::json{			\
				{"id", m_id}, \
				{"type", #ComponentClass} \
			}; \
		}

	#define SER_FIELD(x) {#x, x}

	class Entity;
	class Component {
	public:
		using componentFactory = std::function<Component*(const nlohmann::json&, Entity*)>;
		virtual ~Component() = default;
		explicit Component(const unsigned int obj_id) : m_object_id(obj_id), m_id(s_id_count++) {
		}
		virtual void start() {};
		virtual void update() {};

		bool operator==(const Component& other) const;
		bool operator==(const std::unique_ptr<Component>& other) const;

		unsigned int id() const;
		Entity* entity() const;

		inline static std::map<std::string, componentFactory> componentRegistry;

		virtual operator nlohmann::json() const {
			return nlohmann::json{
				{"type", "Component"}
			};
		}

		bool enabled() const { return m_enabled; }
		bool destroyFlag() const { return m_destroyFlag; }
		bool createdOnThisFrame() const { return m_createdOnThisFrame; }

		void destroy() { m_destroyFlag = true; }
		void markNotCreatedOnThisFrame() { m_createdOnThisFrame = false; }

	protected:
		const unsigned int m_object_id;
		const unsigned int m_id;
		static unsigned int s_id_count;
		bool m_enabled = true;
		bool m_createdOnThisFrame = true;
		bool m_startedFlag = false;
		bool m_destroyFlag = false;
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
