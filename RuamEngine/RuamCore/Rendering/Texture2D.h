#pragma once

#include "RenderingCore.h"
#include "FileFunctions.h"

namespace RuamEngine
{
	class Texture2D
	{
	private:
		unsigned int m_rendererId;
		int m_Width, m_Height, m_BPP; // Bits per pixel

	public:
		std::string m_filePath;
		unsigned char* m_localBuffer;
		Texture2D(const std::string& relativePath); // Relative path from the project root directory
		~Texture2D();

		void Bind(unsigned int slot = 0) const;
		void Unbind();

		inline GLuint GetId() const { return m_rendererId; }
		inline int GetWidth() const { return m_Width; }
		inline int GetHeight() const { return m_Height; }
	};
	using Texture2DPtr = std::shared_ptr<Texture2D>;
}
