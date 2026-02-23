#pragma once

#include "Entity.h"
#include "../../RuamCore/Input/Input.h"
#include "RuamTime.h"

#include "glm/glm.hpp"
#include "ModelRenderer.h"
#include <functional>

namespace RuamEngine
{
    class Bullet : public Component {

    	using Component::Component;
    public:
        ~Bullet();
    	Bullet(unsigned int obj_id, std::function<void()> callback)
    		: Component(obj_id), m_callback(callback) {

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
