#pragma once

#include "Component.h"

#include "glm/glm.hpp"
#include "JsonConverters.h"
#include <list>

#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

namespace RuamEngine
{

	#define TRANSFORM_SERIALIZED_MEMBERS(X, ...) \
	X(m_position, glm::vec3, glm::vec3(0,0,0), Transform)__VA_ARGS__	\
	X(m_rotation, glm::vec3, glm::vec3(0,0,0), Transform)__VA_ARGS__	\
	X(m_scale, glm::vec3, glm::vec3(1,1,1), Transform)	\

	class Transform : public Component
	{
	public:
		using Component::Component;

		Transform(unsigned int entityId);
		Transform(Json transformData, const unsigned int entityId);
		~Transform();

		void start();
		void update();

		void setPosition(glm::vec3 new_pos);
		void setPosition(float x, float y, float z);
		void setRotation(glm::vec3 new_rot);
		void setRotation(float x, float y, float z);

		void setScale(glm::vec3 new_scale);
		void setScale(float x, float y, float z);

		void setParent(Transform* parent);
		void addChild(Transform* child);
		void removeChild(unsigned int idx);
		const std::list<Transform*>& children() const;

		glm::vec3& position();
		glm::vec3& scale();
		glm::vec3& rotation();

		const glm::vec3& position() const;
		const glm::vec3& scale() const;
		const glm::vec3& rotation() const;

		Transform* operator[](size_t idx);

		std::list<Transform*> m_children;
		Transform* m_parent = nullptr; // If null, there is no parent

		IMPL_forEachSerializedField(TRANSFORM_SERIALIZED_MEMBERS(CALL_INSPECTOR_DRAWER))
		std::string name() override { return "Transform"; }

	protected:
		TRANSFORM_SERIALIZED_MEMBERS(DECL_MEMBER)

	public:

	IMPL_SERIALIZE(Transform,
		TRANSFORM_SERIALIZED_MEMBERS(SER_FIELD, ,))
	};

}
