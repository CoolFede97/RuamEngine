#include "Cubemap.h"
#include "RenderingCore.h"

namespace RuamEngine
{


    void Cubemap::Bind(unsigned int slot) const
    {
        GLCall(glActiveTexture(GL_TEXTURE0 + slot));
		GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, m_rendererId));
    }

    void Cubemap::Unbind() const
    {
		GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, 0));
    }

}
