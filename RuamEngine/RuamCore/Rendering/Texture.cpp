#include "Texture.h"
#include "stb_image.h"

namespace RuamEngine
{
	Texture::Texture(const std::string& relativePath)
		: m_filePath(GlobalizePath(relativePath)), m_localBuffer(nullptr)
	{
		GLCall(glCreateTextures(GL_TEXTURE_2D, 1, &m_rendererId));

		stbi_set_flip_vertically_on_load(1);

		m_localBuffer = stbi_load(m_filePath.c_str(), &m_Width, &m_Height, &m_BPP, 4);

		std::cout << "Absolute path: " << m_filePath << "\n";
		std::cout << "Width: " << m_Width << ", Height: " << m_Height << ", BPP: " << m_BPP << "\n";

		GLCall(glTextureStorage2D(m_rendererId, 1, GL_RGBA8, m_Width, m_Height));
		GLCall(glTextureSubImage2D(m_rendererId, 0, 0, 0, m_Width, m_Height, GL_RGBA, GL_UNSIGNED_BYTE, m_localBuffer));
		glGenerateTextureMipmap(m_rendererId);
		// Turns around the texture, so that it is up-side down
		// We do this because OpenGL expects textures (0,0) position to be at the bottom-left corner,
		// not at the top-left corner.
		// The last variable are the desired channels we want. We put 4 because of the RGBA channels

		GLCall(glTextureParameteri(m_rendererId, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
		GLCall(glTextureParameteri(m_rendererId, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
		GLCall(glTextureParameteri(m_rendererId, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
		GLCall(glTextureParameteri(m_rendererId, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));


		if (!m_localBuffer) std::cout << "Error: image not found at relative path: " << relativePath  << "\n";
		ASSERT(m_localBuffer);
		stbi_image_free(m_localBuffer);
	}

	Texture::~Texture()
	{
		std::cout << "Texture Destroyed!\n";
		GLCall(glDeleteTextures(1, &m_rendererId));
	}

	void Texture::Bind(unsigned int slot /*= 0*/) const
	{
		// OpenGL has slots for textures. "Put this texture into slot 3 please"
		GLCall(glActiveTexture(GL_TEXTURE0 + slot));
		GLCall(glBindTexture(GL_TEXTURE_2D, m_rendererId));
	}

	void Texture::Unbind()
	{
		GLCall(glBindTexture(GL_TEXTURE_2D, 0));
	}
}

