#include "Transform.h"
#include "Component.h"
#include "Entity.h"

#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/euler_angles.hpp"
#include "glm/gtc/matrix_transform.hpp"
namespace RuamEngine
{
	Transform::Transform(const unsigned int entityId) : Component(entityId), m_position(0, 0, 0) {}
	Transform::Transform(nlohmann::json transformData, const unsigned int entityId) : Component(entityId)
	{
		if (transformData.contains("m_position")) m_position = transformData["m_position"].get<glm::vec3>();
		if (transformData.contains("m_rotation")) m_rotation = transformData["m_rotation"].get<glm::vec3>();
		if (transformData.contains("m_scale")) m_scale = transformData["m_scale"].get<glm::vec3>();
	}
	Transform::~Transform()
	{
		if (m_parent!=nullptr) m_parent->m_children.remove(this);
		for (Transform* child : m_children)
		{
			if (child!=nullptr) child->m_parent = nullptr;
		}
	}

	void Transform::translate(const glm::vec3& vec)
	{
	    m_position += vec;
	}
	void Transform::translate(float x, float y, float z)
	{
	    m_position += glm::vec3(x,y,z);
	}

	void Transform::rotate(const glm::vec3& vec)
	{
        m_rotation+=vec;
	}
	void Transform::rotate(float x, float y, float z)
	{
	    m_rotation+=glm::vec3(x,y,z);
	}

	void Transform::scale(const glm::vec3& vec)
	{
	    m_scale*=vec;
	}
	void Transform::scale(float x, float y, float z)
	{
	    m_scale*=glm::vec3(x,y,z);
	}

	void Transform::setPosition(const glm::vec3& new_pos) {
		m_position = new_pos;
	}

	void Transform::setPosition(float x, float y, float z) {
		m_position = glm::vec3(x, y, z);
	}

	void Transform::setRotation(const glm::vec3& new_rot) {
		m_rotation = new_rot;
	}

	void Transform::setRotation(float x, float y, float z) {
		m_rotation = glm::vec3(x, y, z);
	}

	void Transform::setScale(const glm::vec3& new_scale) {
		m_scale = new_scale;
	}

	void Transform::setScale(float x, float y, float z) {
		m_scale = glm::vec3(x, y, z);
	}

	glm::vec3 Transform::globalPosition()
	{
	    Transform* lastParent = m_parent;
		glm::vec3 globalPos = m_position;
	    while (lastParent)
		{
            glm::vec3 parentRotation = lastParent->rotation();
            glm::mat4  parentRotationMatrix = glm::eulerAngleXYZ(
                glm::radians(parentRotation.x),
                glm::radians(parentRotation.y),
                glm::radians(parentRotation.z)
            );
            glm::vec3 scaledPos = lastParent->scale() * globalPos;
            glm::vec3 rotatedPos = glm::vec3(parentRotationMatrix * glm::vec4(scaledPos, 1.0f));

            globalPos=lastParent->position() + rotatedPos;
			lastParent=lastParent->parent();
		}
		return globalPos;
	}
	glm::vec3 Transform::globalScale()
	{
        Transform* lastParent = m_parent;
        glm::vec3 globalScale = m_scale;
        while (lastParent)
        {
            globalScale*=lastParent->scale();
            lastParent=lastParent->parent();
        }
        return globalScale;
	}
	glm::vec3 Transform::globalRotation()
	{
        Transform* lastParent = m_parent;
        glm::vec3 globalRot = m_rotation;
        while (lastParent)
        {
            globalRot+=lastParent->rotation();
            lastParent=lastParent->parent();
        }
        return globalRot;
	}
	void Transform::setParent(Transform* parent) {
		if (m_parent == parent) return;

		if (m_parent != nullptr) m_parent->m_children.remove(this);
		m_parent = parent;

		if (parent != nullptr)
		{
		    m_parentId = parent->entity()->id();
			auto it = std::find(parent->m_children.begin(), parent->m_children.end(), this);
			if (it == parent->m_children.end())
			{
				parent->m_children.push_back(this);
			}
		}
	}

	void Transform::addChild(Transform* child) {
	    if (child==nullptr)
		{
			std::cerr << "Error: Can't add child if child is nullptr!\n";
			return;
		}
		auto child_search = std::find(m_children.begin(), m_children.end(), child);
		if (child_search != m_children.end()) {
			throw std::logic_error("Repeated children");
			return;
		}
		m_children.push_back(child);
		child->setParent(this);
	}

	void Transform::removeChild(unsigned int idx) {
		auto iter = m_children.begin();
		std::advance(iter, idx);
		m_children.erase(iter);
	}

	const std::list<Transform*>& Transform::children() const {
		return m_children;
	}

	Transform* Transform::operator[](size_t idx) {
		if (idx >= m_children.size()) {
			throw std::out_of_range("Child out of range");
		}
		auto iter = m_children.begin();
		std::advance(iter, idx);
		return *iter;
	}
	DEF_REGISTER_COMPONENT(Transform);
}
