#pragma once

#include "RenderingCore.h"
#include "Texture.h"
#include <vector>

namespace RuamEngine
{
	class Cubemap : public Texture
	{
	private:
		std::vector<int> m_widths, m_heights, m_BPPs; // Bits per pixel
		std::vector<std::string> m_filePaths;
	public:
		std::vector<unsigned char*> m_localBuffers;
		Cubemap(const std::vector<std::string>& relativePaths); // Relative paths from the project root directory
		~Cubemap();

		void Bind(unsigned int slot = 0) const override;
		void Unbind() const override;

		GLenum GetType() const override { return GL_TEXTURE_CUBE_MAP; }

		std::vector<int> GetWidth() const { return m_widths; }
		std::vector<int> GetHeight() const { return m_heights; }
		std::vector<std::string> GetPaths() const { return m_filePaths; }
	};
	using CubemapPtr = std::shared_ptr<Cubemap>;
}
