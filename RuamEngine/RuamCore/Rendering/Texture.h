#pragma once

#include "Renderer.h"
#include "FileFunctions.h"
class Texture
{
private:
	unsigned int m_RendererID;
	std::string m_FilePath;
	unsigned char* m_LocalBuffer;
	int m_Width, m_Height, m_BPP; // Bits per pixel

public:
	Texture(const std::string& relativePath); // Relative path from the project root directory
	~Texture();

	void Bind(unsigned int slot = 0) const;
	void Unbind();

	inline int GetWidth() const { return m_Width; }
	inline int GetHeight() const { return m_Height; }
};