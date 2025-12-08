#pragma once

#include "RenderingCore.h"
#include "Texture.h"
#include <vector>

namespace RuamEngine
{
    class Cubemap : public Texture
    {
        private:
		std::vector<int> m_Widths, m_Heights, m_BPPs; // Bits per pixel

	public:
		unsigned char* m_localBuffer;
		Cubemap(const std::string& relativePaths); // Relative path from the project root directory
		Cubemap(const std::string* relativePaths); // Pointer to relative paths from the project root directory
		~Cubemap();

		void Bind(unsigned int slot = 0) const override;
		void Unbind() const override;

		GLenum GetType() const override { return GL_TEXTURE_CUBE_MAP; }

		std::vector<int> GetWidths() const { return m_Widths; }
		std::vector<int> GetHeights() const { return m_Heights; }
	};
	using CubemapPtr = std::shared_ptr<Cubemap>;
}
