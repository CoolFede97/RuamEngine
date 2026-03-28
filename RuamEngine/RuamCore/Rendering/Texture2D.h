#pragma once

#include "RenderingCore.h"
#include "Texture.h"

namespace RuamEngine
{
	class Texture2D : public Texture
	{
	private:
		int m_width, m_height, m_BPP; // Bits per pixel

	public:
		unsigned char* m_localBuffer;
		Texture2D(const std::string& relativePath); // Relative path from the project root directory
		~Texture2D();

		void bind(unsigned int slot = 0) const override;
		void unbind() const override;

		GLenum texType() const override { return GL_TEXTURE_2D; }

		int GetWidth() const { return m_width; }
		int GetHeight() const { return m_height; }
	};
	using Texture2DPtr = std::shared_ptr<Texture2D>;
}
