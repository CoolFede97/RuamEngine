#include "Skybox.h"
#include "Cubemap.h"

namespace RuamEngine
{
    Skybox* Skybox::s_skybox = nullptr;

    void Skybox::SetSkybox(std::vector<std::string> paths)
    {
        s_skybox->m_cubemap = new Cubemap(paths);
    }

    void Skybox::update()
    {
        Component::update();
    }

    void Skybox::start()
    {
        if (s_skybox == nullptr) s_skybox = this;
		else if (s_skybox != this) object()->removeComponent<Skybox>();
    }
}
