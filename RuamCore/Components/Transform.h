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
	class Transform : public Component
	{
	public:
		using Component::Component;

		Transform(unsigned int entityId);
		Transform(nlohmann::json transformData, const unsigned int entityId);
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
		static constexpr bool s_unique = true;
		std::vector<FieldInfo> fields() override
		{
		    return
    		{
                makeFieldInfo<glm::vec3>("m_position", m_position),
                makeFieldInfo<glm::vec3>("m_rotation", m_rotation),
                makeFieldInfo<glm::vec3>("m_scale", m_scale)
    		};
		}

	private:
	    glm::vec3 m_position = glm::vec3(0,0,0);
	    glm::vec3 m_rotation = glm::vec3(0,0,0);
	    glm::vec3 m_scale = glm::vec3(1,1,1);
		unsigned int m_parentId = 0;
		DECL_REGISTER_COMPONENT(Transform)
	public:

	friend class Serial;
	};
}
