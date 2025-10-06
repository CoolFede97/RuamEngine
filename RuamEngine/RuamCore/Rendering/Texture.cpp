#include "Texture.h"
#include "stb_image.h"

namespace RuamEngine
{
	Texture::Texture(const std::string& relativePath)
		: m_FilePath(relativePath), m_LocalBuffer(nullptr)
	{
		GLCall(glCreateTextures(GL_TEXTURE_2D, 1, &m_id));

		stbi_set_flip_vertically_on_load(1);

		std::string absolutePath = std::string(PROJECT_ROOT_DIR) + "/" + relativePath;
		m_LocalBuffer = stbi_load(absolutePath.c_str(), &m_Width, &m_Height, &m_BPP, 4);

		GLCall(glTextureStorage2D(m_id, 1, GL_RGBA8, m_Width, m_Height));
		GLCall(glTextureSubImage2D(m_id, 0, 0, 0, m_Width, m_Height, GL_RGBA, GL_UNSIGNED_BYTE, m_LocalBuffer));
		glGenerateTextureMipmap(m_id);
		// Turns around the texture, so that it is up-side down
		// We do this because OpenGL expects textures (0,0) position to be at the bottom-left corner,
		// not at the top-left corner.
		// The last variable are the desired channels we want. We put 4 because of the RGBA channels

		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));


		if (!m_LocalBuffer) std::cout << "Error: image not found at relative path: " << relativePath  << "\n";
		ASSERT(m_LocalBuffer);
		stbi_image_free(m_LocalBuffer);
		GLCall(glBindTexture(GL_TEXTURE_2D, 0));
	}

	Texture::~Texture()
	{
		std::cout << "Texture Destroyed!\n";
		GLCall(glDeleteTextures(1, &m_id));
	}

	void Texture::Bind(unsigned int slot /*= 0*/) const
	{
		// OpenGL has slots for textures. "Put this texture into slot 3 please"
		GLCall(glActiveTexture(GL_TEXTURE0 + slot));
		GLCall(glBindTexture(GL_TEXTURE_2D, m_id));
	}

	void Texture::Unbind()
	{
		GLCall(glBindTexture(GL_TEXTURE_2D, 0));
	}
}

