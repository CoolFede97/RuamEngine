#pragma once

#include "Component.h"

#include "Serial.h"
#include "glm/glm.hpp"
#include "JsonConverters.h"
#include <list>

#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

namespace RuamEngine
{
	#define TRANSFORM_SERIALIZED_MEMBERS(X, ...) \
	X(m_position, glm::vec3, glm::vec3(0,0,0), nullptr)__VA_ARGS__	\
	X(m_rotation, glm::vec3, glm::vec3(0,0,0), nullptr)__VA_ARGS__	\
	X(m_scale, glm::vec3, glm::vec3(1,1,1), nullptr)__VA_ARGS__	\
	X(m_parentId, unsigned int, 0, nullptr)

	class Transform : public Component
	{
	public:
		using Component::Component;

		Transform(unsigned int entityId);
		Transform(Json transformData, const unsigned int entityId);
		~Transform();

		void translate(const glm::vec3& vec);
		void translate(float x, float y, float z);

		void rotate(const glm::vec3& vec);
		void rotate(float x, float y, float z);

		void scale(const glm::vec3& vec);
		void scale(float x, float y, float z);

		void setPosition(const glm::vec3& new_pos);
		void setPosition(float x, float y, float z);

		void setRotation(const glm::vec3& new_rot);
		void setRotation(float x, float y, float z);

		void setScale(const glm::vec3& new_scale);
		void setScale(float x, float y, float z);

		void setParent(Transform* parent);
		void addChild(Transform* child);
		void removeChild(unsigned int idx);
		const std::list<Transform*>& children() const;
		inline Transform* parent() const { return m_parent; };

		inline const glm::vec3& position() { return m_position;} // returns local pos. stored value.
		inline const glm::vec3& scale() { return m_scale; } // returns local scale. stored value.
		inline const glm::vec3& rotation() { return m_rotation; }; // returns local rotation. stored value.

		glm::vec3 globalPosition(); // returns global pos. computed value
		glm::vec3 globalScale(); // returns global scale. computed value
		glm::vec3 globalRotation(); // returns global rotation. computed value
		Transform* operator[](size_t idx);

		std::list<Transform*> m_children;
		Transform* m_parent = nullptr; // If null, there is no parent

		IMPL_DRAW_SERIALIZED_MEMBERS(TRANSFORM_SERIALIZED_MEMBERS(CALL_INSPECTOR_DRAWER))

	protected:
		TRANSFORM_SERIALIZED_MEMBERS(DECL_MEMBER)

	private:
		DECL_REGISTER_COMPONENT(Transform)
	public:

	IMPL_SERIALIZE(Transform,
		TRANSFORM_SERIALIZED_MEMBERS(SER_FIELD, ,))
	friend class Serial;
	};
}
