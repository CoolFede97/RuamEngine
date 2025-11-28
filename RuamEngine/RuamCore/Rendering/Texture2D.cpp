#include "Texture2D.h"
#include "stb_image.h"
#include "FileFunctions.h"

namespace RuamEngine
{
	Texture2D::Texture2D(const std::string& relativePath)
		: m_localBuffer(nullptr)
	{
	    m_filePath = GlobalizePath(relativePath);
		GLCall(glCreateTextures(GL_TEXTURE_2D, 1, &m_rendererId));

		stbi_set_flip_vertically_on_load(1);

		m_localBuffer = stbi_load(m_filePath.c_str(), &m_Width, &m_Height, &m_BPP, 4);

		if (m_localBuffer == NULL)
		{
			const char* reason = stbi_failure_reason();
			if (reason) {
				std::cerr << "Couldn't load texture at path : "<< m_filePath << "\n The reason was: " << reason << "\n";
			}
			else {
				std::cerr << "Couldn't load texture at path :" << m_filePath << "\n There was no apparent reason\n";
			}
		}
		else {
			std::cout << "Texture2D at path: " << m_filePath << " was loaded succesfully" << "\n";
		}

		GLCall(glTextureStorage2D(m_rendererId, 1, GL_RGBA8, m_Width, m_Height));
		GLCall(glTextureSubImage2D(m_rendererId, 0, 0, 0, m_Width, m_Height, GL_RGBA, GL_UNSIGNED_BYTE, m_localBuffer));
		glGenerateTextureMipmap(m_rendererId);
		// Turns around the texture, so that it is up-side down
		// We do this because OpenGL expects textures (0,0) position to be at the bottom-left corner,
		// not at the top-left corner.
		// The last variable are the desired channels we want. We put 4 because of the RGBA channels

		GLCall(glTextureParameteri(m_rendererId, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
		GLCall(glTextureParameteri(m_rendererId, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
		GLCall(glTextureParameteri(m_rendererId, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT));
		GLCall(glTextureParameteri(m_rendererId, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT));


		if (!m_localBuffer) std::cout << "Error: image not found at relative path: " << relativePath  << "\n";
		ASSERT(m_localBuffer);
		stbi_image_free(m_localBuffer);
	}

	Texture2D::~Texture2D()
	{
		std::cout << "Texture2D Destroyed!\n";
		GLCall(glDeleteTextures(1, &m_rendererId));
	}

	void Texture2D::Bind(unsigned int slot /*= 0*/) const
	{
		// OpenGL has slots for textures. "Put this texture into slot 3 please"
		GLCall(glActiveTexture(GL_TEXTURE0 + slot));
		GLCall(glBindTexture(GL_TEXTURE_2D, m_rendererId));
	}

	void Texture2D::Unbind() const
	{
		GLCall(glBindTexture(GL_TEXTURE_2D, 0));
	}
}
