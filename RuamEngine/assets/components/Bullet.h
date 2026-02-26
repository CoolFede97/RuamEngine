#pragma once

#include "Entity.h"

#include "glm/glm.hpp"
#include <functional>

namespace RuamEngine
{
    class Bullet : public Component
    {
    	using Component::Component;

    public:
        IMPL_SIMPLE_SERIALIZE(Bullet)
        std::string name() override { return "Bullet"; }
        IMPL_DRAW_SERIALIZED_MEMBERS(;)

        ~Bullet();
    	Bullet(unsigned int obj_id, std::function<void()> callback)
    		: Component(obj_id), m_callback(callback)
    	{
    	}

    	void start()
    	{
    	}

    	void update();

        bool m_isPlayerBullet = false;
    	float m_radius;
    	float m_speed;
    	glm::vec3 m_direction;
    	glm::vec3 m_target;

    private:
    	std::function<void()> m_callback;
    };
}
