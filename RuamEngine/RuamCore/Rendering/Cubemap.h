#pragma once

#include "RenderingCore.h"
#include "Texture.h"
#include <vector>

namespace RuamEngine
{
	class Cubemap : public Texture
	{
	private:
		int m_faceLength, m_BPP; //The dimensions of each side are expected to be the same. BPP means bits per pixel
		// In case the cubemap is created with 6 individual textures, this member will have 6 elements.
		std::vector<std::string> m_filePaths;
	public:
		std::vector<unsigned char*> m_localBuffers;
		Cubemap(const std::vector<std::string>& relativePaths); // Relative paths from the project root directory
		Cubemap(const std::string& relativePath); // Relative path from the project root directory
		~Cubemap();

		void bind(unsigned int slot = 0) const override;
		void unbind() const override;

		GLenum texType() const override { return GL_TEXTURE_CUBE_MAP; }

		int faceLength() const { return m_faceLength; }
		std::vector<std::string> paths() const { return m_filePaths; }
	};
	using CubemapPtr = std::shared_ptr<Cubemap>;
}
