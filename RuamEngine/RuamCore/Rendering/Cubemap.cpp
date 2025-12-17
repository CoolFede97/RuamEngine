#include "Cubemap.h"
#include "DebugUtils.h"
#include "FileFunctions.h"
#include "OpenGLUtils.h"
#define STB_IMAGE_STATIC
#include "stb_image.h"
#include "RenderingCore.h"

namespace RuamEngine
{

    Cubemap::Cubemap(const std::vector<std::string>& relativePaths)
    : m_localBuffers({})
    {
        m_filePaths = GlobalizePaths(relativePaths);
        GLCall(glCreateTextures(GL_TEXTURE_CUBE_MAP, 1, &m_rendererId));

        stbi_set_flip_vertically_on_load(1);

        for (unsigned int i = 0; i < relativePaths.size(); i++)
        {
            m_widths.push_back(0);
            m_heights.push_back(0);
            m_BPPs.push_back(0);
            m_localBuffers.push_back(0);

            m_localBuffers[i] = stbi_load(m_filePaths[i].c_str(), &m_widths[i], &m_heights[i], &m_BPPs[i], 4);

            if (m_localBuffers[i] == nullptr)
            {
                const char* reason = stbi_failure_reason();
    			if (reason) {
    				std::cerr << "Couldn't load cubemap texture at path : "<< m_filePaths[i] << "\n The reason was: " << reason << "\n";
    			}
    			else {
    				std::cerr << "Couldn't load cubemap texture at path :" << m_filePaths[i] << "\n There was no apparent reason\n";
    			}
            }
            else
            {
                std::cout << "Cubemap texture at path: " << m_filePaths[i] << " was loaded succesfully" << "\n";
            }

            ASSERT(m_localBuffers[i]);
        }

        GLCall(glTextureStorage2D(m_rendererId, 1, GL_RGBA8, m_widths[0], m_heights[0]));

        for (unsigned int i = 0; i < relativePaths.size(); i++)
        {
            GLCall(glTextureSubImage3D(m_rendererId, 0, 0, 0, i, m_widths[i], m_heights[i], 1, GL_RGBA, GL_UNSIGNED_BYTE, m_localBuffers[i]));
            stbi_image_free(m_localBuffers[i]);
        }

        glTexParameteri(m_rendererId, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(m_rendererId, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(m_rendererId, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(m_rendererId, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(m_rendererId, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    }

    void Cubemap::Bind(unsigned int slot) const
    {
        GLCall(glActiveTexture(GL_TEXTURE0 + slot));
        GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, m_rendererId));
    }
    void Cubemap::Unbind() const
    {
        GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, 0));
    }
    Cubemap::~Cubemap()
    {
        GLCall(glDeleteTextures(1, &m_rendererId));
    }

    std::string Cubemap::GetPath() const
    {
        std::string finalPath = "";
        for (std::string path : m_filePaths)
        {
            finalPath += path + " ";
        }
        return finalPath;
    }
}
