#include "Texture.h"
#include "stb_image.h"

Texture::Texture(const std::string& relativePath)
	: m_RendererID(0), m_FilePath(relativePath), m_LocalBuffer(nullptr), m_Width(0), m_Height(0), m_BPP(0)
{
	// Turns around the texture, so that it is up-side down
	// We do this because OpenGL expects textures (0,0) position to be at the bottom-left corner,
	// not at the top-left corner.
	stbi_set_flip_vertically_on_load(1);

	std::string absolutePath = std::string(PROJECT_ROOT_DIR) + "/" + relativePath;
	// The last variable are the desired channels we want. We put 4 because of the RGBA channels
	m_LocalBuffer = stbi_load(absolutePath.c_str(), &m_Width, &m_Height, &m_BPP, 4);

	if (!m_LocalBuffer) std::cout << "Error: image not found at relative path: " << relativePath  << "\n";
	
	GLCall(glGenTextures(1, &m_RendererID));
	GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));

	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

	GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_LocalBuffer));
	GLCall(glBindTexture(GL_TEXTURE_2D, 0));

	if (m_LocalBuffer)
		stbi_image_free(m_LocalBuffer);
}

Texture::~Texture()
{
	GLCall(glDeleteTextures(1, &m_RendererID));
}

void Texture::Bind(unsigned int slot /*= 0*/) const
{
	// OpenGL has slots for textures. "Put this texture into slot 3 please"
	GLCall(glActiveTexture(GL_TEXTURE0 + slot));	
	GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));
}

void Texture::Unbind()
{
	GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}
