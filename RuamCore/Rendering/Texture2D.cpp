#include "Texture2D.h"
#define STB_IMAGE_STATIC
#include "stb_image.h"
#include "FileFunctions.h"
#include "ResourceManager.h"

namespace RuamEngine
{
	Texture2D::Texture2D(const std::string& relativePath)
		: m_localBuffer(nullptr)
	{
	    m_path = relativePath;
		GLCall(glGenTextures(1, &m_glName));
		GLCall(glBindTexture(GL_TEXTURE_2D, m_glName));

		stbi_set_flip_vertically_on_load(1);

		m_localBuffer = stbi_load(globalizePath(m_path).c_str(), &m_width, &m_height, &m_BPP, 4);

		if (m_localBuffer == NULL)
		{
			const char* reason = stbi_failure_reason();
			if (reason) {
				std::cerr << "Couldn't load texture at path : "<< m_path << "\n The reason was: " << reason << "\n";
			}
			else {
				std::cerr << "Couldn't load texture at path :" << m_path << "\n There was no apparent reason\n";
			}
		}
		else {
			std::cout << "Texture2D at path: " << m_path << " was loaded succesfully" << "\n";
		}

		GLCall(glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA8, m_width, m_height));
		GLCall(glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, m_width, m_height, GL_RGBA, GL_UNSIGNED_BYTE, m_localBuffer));
		// Turns around the texture, so that it is up-side down
		// We do this because OpenGL expects textures (0,0) position to be at the bottom-left corner,
		// not at the top-left corner.
		// The last variable are the desired channels we want. We put 4 because of the RGBA channels

		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT));


		if (!m_localBuffer) std::cout << "Error: image not found at relative path: " << relativePath  << "\n";
		ASSERT(m_localBuffer);
		stbi_image_free(m_localBuffer);
	}

	Texture2D::Texture2D(const std::string& modelTexPath, const aiTexture* aiTex)
	: m_width(0), m_height(0), m_localBuffer(nullptr)
	{
        m_path = modelTexPath;

        GLCall(glGenTextures(1, &m_glName));
		GLCall(glBindTexture(GL_TEXTURE_2D, m_glName));

		stbi_set_flip_vertically_on_load(1);

		bool heightZero = false;
	    if (aiTex->mHeight == 0)
    	{
            heightZero = true;
            m_localBuffer = stbi_load_from_memory(
                reinterpret_cast<const stbi_uc*>(aiTex->pcData),
                aiTex->mWidth,   // cantidad de bytes
                &m_width,
                &m_height,
                &m_BPP,
                STBI_rgb_alpha
            );
    	}
		else
		{
            m_width = aiTex->mWidth;
            m_height = aiTex->mHeight;
		}

		if (heightZero)
		{
    		if (m_localBuffer == NULL)
    		{
    			const char* reason = stbi_failure_reason();
    			if (reason) {
    				std::cerr << "Couldn't load Texture2D from aiTexture at path : "<< m_path << "\n The reason was: " << reason << "\n";
    			}
    			else {
    				std::cerr << "Couldn't load Texture2D from aiTexture at path :" << m_path << "\n There was no apparent reason\n";
    			}
    		}
    		else {
    			std::cout << "Texture2D from aiTexture at path: " << m_path << " was loaded succesfully" << "\n";
    		}
		}

		GLCall(glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA8, m_width, m_height));
		const void* pixels = heightZero
            ? static_cast<const void*>(m_localBuffer)
            : static_cast<const void*>(aiTex->pcData);

		GLCall(glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, m_width, m_height, GL_RGBA, GL_UNSIGNED_BYTE, pixels));

		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));

		if (heightZero)
		{
    		ASSERT(m_localBuffer);
    		stbi_image_free(m_localBuffer);
		}
	}

	Texture2D::~Texture2D()
	{
	    ResourceManager::RemoveTexture2DIfExpired(m_path);
		GLCall(glDeleteTextures(1, &m_glName));
	}

	void Texture2D::bind(unsigned int slot /*= 0*/) const
	{
		// OpenGL has slots for textures. "Put this texture into slot 3 please"
		GLCall(glActiveTexture(GL_TEXTURE0 + slot));
		GLCall(glBindTexture(GL_TEXTURE_2D, m_glName));
	}

	void Texture2D::unbind() const
	{
		GLCall(glBindTexture(GL_TEXTURE_2D, 0));
	}
}
