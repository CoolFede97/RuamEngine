#pragma once

#include "RenderingCore.h"
#include "FileFunctions.h"

namespace RuamEngine
{
	class Texture
	{
	private:
		unsigned int m_id;
		std::string m_FilePath;
		int m_Width, m_Height, m_BPP; // Bits per pixel

	public:
		unsigned char* m_LocalBuffer;
		Texture(const std::string& relativePath); // Relative path from the project root directory
		~Texture();

		void Bind(unsigned int slot = 0) const;
		void Unbind();

		inline GLuint GetId() const { return m_id; }
		inline int GetWidth() const { return m_Width; }
		inline int GetHeight() const { return m_Height; }
	};
	using TexturePtr = std::shared_ptr<Texture>;
}
